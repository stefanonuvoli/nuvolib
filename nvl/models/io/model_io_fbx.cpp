#include "model_io_fbx.h"

#ifdef NVL_FBXSDK_LOADED

#include <fbxsdk.h>

#include <unordered_map>
#include <stack>

#include <nvl/math/euler_angles.h>
#include <nvl/models/algorithms/model_pose_propagation.h>
#include <nvl/models/algorithms/model_deformation.h>
#include <nvl/models/algorithms/model_transformations.h>

#include <nvl/utilities/file_utils.h>

namespace nvl {

namespace internal {
template<class M, class S, class W, class A>
struct FBXImporterData {
    typedef M Mesh;
    typedef S Skeleton;
    typedef W SkinningWeights;
    typedef A Animation;
    typedef typename Mesh::Point Point;
    typedef typename Mesh::VertexNormal VertexNormal;
    typedef typename Mesh::VertexUV VertexUV;
    typedef typename Mesh::VertexColor VertexColor;
    typedef typename Mesh::PolylineColor PolylineColor;
    typedef typename Mesh::FaceNormal FaceNormal;
    typedef typename Mesh::Material Material;
    typedef typename SkinningWeights::Scalar SkinningWeightsScalar;
    typedef typename Skeleton::Transformation SkeletonTransformation;
    typedef typename Skeleton::JointId JointId;
    typedef typename Animation::Transformation AnimationTransformation;

    typedef IOMeshData<
                Point,
                VertexNormal,
                VertexUV,
                VertexColor,
                PolylineColor,
                FaceNormal,
                Material> MeshData;
    typedef IOSkinningWeightsData<SkinningWeightsScalar> SkinningWeightsData;
    typedef IOSkeletonData<SkeletonTransformation> SkeletonData;
    typedef IOAnimationData<AnimationTransformation> AnimationData;

    //Data to insert in the mesh
    MeshData meshData;
    SkeletonData skeletonData;
    SkinningWeightsData skinningWeightsData;
    std::vector<AnimationData> animationDataVector;

    std::vector<SkeletonTransformation> bindPoseTransformations;

    //Useful mapping information
    std::unordered_map<FbxMesh*, std::vector<Index>> meshVertexMap;
    std::unordered_map<FbxNode*, Index> skeletonMap;
    std::vector<FbxAnimStack*> animationStacks;
    std::vector<std::vector<FbxTime>> animationTimes;
};

template<class FBXData>
void handleSkeletonInner(
        FbxNode* lNode,
        Index parentJointId,
        FBXData& data);
template<class FBXData>
void handleSkeleton(
        FbxNode* lNode,
        FBXData& data);
template<class FBXData>
void handleMesh(
        FbxNode* lNode,
        FBXData& data);
template<class FBXData>
void handleMeshAndSkeletonRecursive(
        FbxNode* lNode,
        FBXData& data);
template<class FBXData>
void handleDeformerAndAnimationsRecursive(
        FbxScene* lScene,
        FbxNode* lNode,
        FBXData& data);
template<class FBXData>
void handleBindPoseRecursive(
        FbxNode* lNode,
        FbxPose* lPose,
        FBXData& data);

FbxAMatrix nodeGlobalPosition(
       FbxNode* lNode,
       FbxPose* lPose);
FbxAMatrix nodePoseMatrix(FbxPose* lPose, int lNodeIndex);
FbxAMatrix nodeGeometryOffset(FbxNode* lNode);

template<class T>
T transformationFromFBXMatrix(const FbxAMatrix& fbxT);
inline Translation3d transformationFromFBXTranslation(const FbxDouble3& fbxT);
inline Rotation3d transformationFromFBXRotation(const FbxDouble3& fbxR);
}

template<class M, class S, class W, class A>
bool modelLoadDataFromFBX(
        const std::string& filename,
        IOModelData<M,S,W,A>& modelData,
        IOModelError& error,
        const IOModelMode& mode)
{
    typedef internal::FBXImporterData<M,S,W,A> FBXData;

    typedef typename FBXData::AnimationData AnimationData;

    typedef typename FBXData::JointId JointId;
    typedef typename FBXData::SkeletonTransformation SkeletonTransformation;
    typedef typename FBXData::Animation Animation;
    typedef typename FBXData::AnimationTransformation AnimationTransformation;

    error = IO_MODEL_SUCCESS;

    //Initialize the SDK manager. This object handles memory management.
    FbxManager* lSdkManager = FbxManager::Create();

    //Create the IO settings object.
    FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);

    //Create an importer using the SDK manager.
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager,"");

    //Use the first argument as the filename for the importer.
    bool imported = lImporter->Initialize(filename.c_str(), -1, lSdkManager->GetIOSettings());

    if (!imported) {
        error = IO_MODEL_FILE_ERROR;
        return false;
    }

    //Create a new scene so that it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

    //Import the contents of the file into the scene.
    lImporter->Import(lScene);

    //The file is imported, so get rid of the importer.
    lImporter->Destroy();

    //Data for import into the model
    std::string modelName = filenameName(filename);

    //Data needed by FBX importer
    FBXData fbxData;

    FbxNode* lRootNode = lScene->GetRootNode();
    if (lRootNode) {
        FbxAMatrix lDummyGlobalPosition;
        internal::handleMeshAndSkeletonRecursive(lRootNode, fbxData);

        Index animationStackNumber = static_cast<Index>(lScene->GetSrcObjectCount<FbxAnimStack>());

        for (Index i = 0; i < animationStackNumber; i++) {
            FbxAnimStack* lAnimStack = lScene->GetSrcObject<FbxAnimStack>(i);
            FbxString animStackName = lAnimStack->GetName();
            FbxTakeInfo* takeInfo = lScene->GetTakeInfo(animStackName);

            if (takeInfo) {
                FbxTime animationStartTime = takeInfo->mLocalTimeSpan.GetStart();
                FbxTime animationStolTime = takeInfo->mLocalTimeSpan.GetStop();
                FbxLongLong animationFrameNumber = animationStolTime.GetFrameCount(FbxTime::eDefaultMode) - animationStartTime.GetFrameCount(FbxTime::eDefaultMode) + 1;

                AnimationData animationData;
                animationData.name = animStackName;
                animationData.times.resize(animationFrameNumber);
                animationData.transformations.resize(animationFrameNumber);

                double offsetTime = static_cast<double>(animationStartTime.GetMilliSeconds()) / 1000;
                std::vector<FbxTime> fbxTimes(animationFrameNumber);
                for (FbxLongLong fId = 0; fId < animationFrameNumber; ++fId) {
                    FbxTime currTime;
                    currTime.SetFrame(animationStartTime.GetFrameCount(FbxTime::eDefaultMode) + fId, FbxTime::eDefaultMode);

                    fbxTimes[fId] = currTime;

                    double time = static_cast<double>(currTime.GetMilliSeconds()) / 1000;
                    time -= offsetTime;
                    animationData.times[fId] = time;
                    animationData.transformations[fId].resize(fbxData.skeletonData.joints.size());
                }

                fbxData.animationTimes.push_back(fbxTimes);
                fbxData.animationStacks.push_back(lAnimStack);
                fbxData.animationDataVector.push_back(animationData);
            }
        }

        internal::handleDeformerAndAnimationsRecursive(lScene, lRootNode, fbxData);

        if (mode.deformToBindPose) {
            FbxPose* lBindPose = nullptr;
            const int lPoseCount = lScene->GetPoseCount();
            for (int i = 0; i < lPoseCount && lBindPose == nullptr; ++i) {
                FbxPose* currentPose = lScene->GetPose(i);
                if (currentPose->IsBindPose()) {
                    lBindPose = currentPose;
                }
            }
            for (int i = 0; i < lPoseCount && lBindPose == nullptr; ++i) {
                FbxPose* currentPose = lScene->GetPose(i);
                if (currentPose->IsRestPose()) {
                    lBindPose = currentPose;
                }
            }

            if (lBindPose) {
                fbxData.bindPoseTransformations.resize(fbxData.skeletonData.joints.size());
                handleBindPoseRecursive(lRootNode, lBindPose, fbxData);
            }
        }
    }



    //Fill data
    modelData.name = modelName;

    //Load mesh and skeleton
    meshLoadData(modelData.mesh, fbxData.meshData, mode.meshMode);
    skeletonLoadData(modelData.skeleton, fbxData.skeletonData, mode.skeletonMode);

    //Initialize skinning weights
    modelData.skinningWeights.initialize(modelData.mesh.nextVertexId(), modelData.skeleton.jointNumber());

    //Load skinning weights
    skinningWeightsLoadData(modelData.skinningWeights, fbxData.skinningWeightsData, mode.skinningWeightsMode);

    //Load animation bind pose if to compute
    bool computeBindPose = false;
    if (!fbxData.bindPoseTransformations.empty()) {
        for (JointId jId = 0; jId < fbxData.bindPoseTransformations.size() && !computeBindPose; ++jId) {
            const SkeletonTransformation& t = fbxData.bindPoseTransformations[jId];

            Rotation3d rot(t.rotation());
            Vector3d tra(t.translation());

            const double eps = 1e-3;

            Vector3d eulerAngles = eulerAnglesFromRotationXYZ(rot);
            for (EigenId i = 0; i < eulerAngles.size(); ++i) {
                eulerAngles[i] = eulerAngles[i] / M_PI * 180.0;

                if (!epsEqual(eulerAngles[i], 0.0, eps)) {
                    computeBindPose = true;
                }
            }

            for (EigenId i = 0; i < tra.size(); ++i) {
                if (!epsEqual(tra[i], 0.0, eps)) {
                    computeBindPose = true;
                }
            }
        }
    }

    if (computeBindPose) {
        modelDeformDualQuaternionSkinning(modelData.mesh, modelData.skeleton, modelData.skinningWeights, modelData.animations, fbxData.bindPoseTransformations);

        for (JointId jId = 0; jId < modelData.skeleton.jointNumber(); ++jId) {
            modelData.skeleton.joint(jId).setBindPose(fbxData.bindPoseTransformations[jId] * fbxData.skeletonData.joints[jId]);
        }
    }

    //Compute local bind poses
    std::vector<SkeletonTransformation> localBindPose = skeletonLocalFromGlobalBindPose(modelData.skeleton);

    //Load animations
    for (AnimationData& animationData : fbxData.animationDataVector) {
        Animation animation;

        animationLoadData(animation, animationData, mode.animationMode);

        nvl::animationFrameLocalFromGlobal(modelData.skeleton, animation.keyframes());

        bool isIdentity = true;
        for (nvl::Index frameId = 0; frameId < animation.keyframeNumber(); ++frameId) {
            for (JointId jId = 0; jId < modelData.skeleton.jointNumber(); ++jId) {
                AnimationTransformation& t = animation.keyframe(frameId).transformation(jId);

                t = localBindPose[jId].inverse() * t;

                if (isIdentity) {
                    Rotation3d rot(t.rotation());
                    Vector3d tra(t.translation());

                    const double eps = 1e-3;

                    Vector3d eulerAngles = eulerAnglesFromRotationXYZ(rot);
                    for (EigenId i = 0; i < eulerAngles.size(); ++i) {
                        eulerAngles[i] = eulerAngles[i] / M_PI * 180.0;

                        if (!epsEqual(eulerAngles[i], 0.0, eps)) {
                            isIdentity = false;
                        }
                    }

                    for (EigenId i = 0; i < tra.size(); ++i) {
                        if (!epsEqual(tra[i], 0.0, eps)) {
                            isIdentity = false;
                        }
                    }
                }
            }
        }

        if (!isIdentity) {
            modelData.animations.push_back(animation);
        }
    }

    nvl::modelRemoveRotationInBindPose(modelData.skeleton, modelData.animations);

    return true;
}

namespace internal {

template<class FBXData>
void handleSkeletonInner(
        FbxNode* lNode,
        Index parentJointId,
        FBXData& data)
{
    typedef typename FBXData::SkeletonTransformation SkeletonTransformation;

    FbxSkeleton* lSkeleton = lNode->GetSkeleton();
    bool isSkeleton = lSkeleton != nullptr ? true : false;

    FbxAMatrix lJointGlobalTransform = lNode->EvaluateGlobalTransform();
    SkeletonTransformation jointTransformation = transformationFromFBXMatrix<SkeletonTransformation>(lJointGlobalTransform);

    const std::string jointName = lNode->GetName();

    Index jId = data.skeletonData.joints.size();
    data.skeletonData.joints.push_back(jointTransformation);
    data.skeletonData.hidden.push_back(!isSkeleton);
    data.skeletonData.names.push_back(jointName);
    data.skeletonData.parents.push_back(parentJointId);

    data.skeletonMap.insert(std::make_pair(lNode, jId));

    const int lChildCount = lNode->GetChildCount();
    for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex) {
        handleSkeletonInner(lNode->GetChild(lChildIndex), jId, data);
    }
}

template<class FBXData>
void handleSkeleton(
        FbxNode* lNode,
        FBXData& data)
{
    FbxSkeleton* lSkeleton = lNode->GetSkeleton();

    if (lSkeleton->IsSkeletonRoot()) {
        handleSkeletonInner(lNode, NULL_ID, data);
    }
}


template<class FBXData>
void handleMesh(
        FbxNode* lNode,
        FBXData& data)
{
    typedef typename FBXData::Point Point;
    typedef typename FBXData::VertexNormal VertexNormal;
    typedef typename FBXData::VertexUV VertexUV;
    typedef typename FBXData::Material Material;

    FbxMesh* lMesh = lNode->GetMesh();
    assert(lMesh);

    int lVertexCount = lMesh->GetControlPointsCount();
    int lFaceCount = lMesh->GetPolygonCount();

    //Empty mesh
    if (lVertexCount == 0) {
        return;
    }

    std::vector<Index> vertexMap(lVertexCount, NULL_ID);

    Size lastVertexId = data.meshData.vertices.size();
    Size lastFaceId = data.meshData.faces.size();
    Size lastNormalId = data.meshData.vertexNormals.size();
    Size lastUVId = data.meshData.vertexUVs.size();


    //Vertices

    data.meshData.vertices.resize(lastVertexId + lVertexCount);

    FbxVector4* lControlPoints = lMesh->GetControlPoints();
    FbxAMatrix lGlobalTransform = lNode->EvaluateGlobalTransform();

    for (int i = 0; i < lVertexCount; i++) {
        FbxVector4 controlPoint = lGlobalTransform.MultT(lControlPoints[i]);

        Index vId = lastVertexId + i;

        data.meshData.vertices[vId] = Point(controlPoint[0], controlPoint[1], controlPoint[2]);

        vertexMap[i] = vId;
    }


    //Faces

    data.meshData.faces.resize(lastFaceId + lFaceCount);

    for (int i = 0; i < lFaceCount; i++) {
        int lFaceSize = lMesh->GetPolygonSize(i);

        Index fId = lastFaceId + i;

        std::vector<Index>& newFace = data.meshData.faces[fId];
        newFace.resize(lFaceSize);

        for (int j = 0; j < lFaceSize; j++) {
            int lControlPointIndex = lMesh->GetPolygonVertex(i, j);
            newFace[j] = vertexMap[lControlPointIndex];
        }
    }


    //Materials

    int materialCount = lNode->GetMaterialCount();

    std::vector<Index> materialMap(materialCount);

    for (int i = 0; i < materialCount; i++) {
        FbxSurfaceMaterial* lMaterial = lNode->GetMaterial(i);

        Material material;

        material.setName(lMaterial->GetName());

        if (lMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
            FbxSurfaceLambert* lLambert = static_cast<FbxSurfaceLambert*>(lMaterial);
            assert(lLambert != nullptr);

            FbxDouble3 lDiffuseColor = lLambert->Diffuse;
            Color diffuseColor(lDiffuseColor[0],lDiffuseColor[1],lDiffuseColor[2]);
            material.setDiffuseColor(diffuseColor);

            FbxDouble3 lAmbientColor = lLambert->Ambient;
            Color ambientColor(lAmbientColor[0],lAmbientColor[1],lAmbientColor[2]);
            material.setAmbientColor(ambientColor);

            FbxDouble transparencyFactor = lLambert->TransparencyFactor;
            material.setTransparency(static_cast<float>(transparencyFactor));

            material.setShadingModel(Material::SHADING_LAMBERTIAN);
        }
        else if (lMaterial->GetClassId().Is(FbxSurfacePhong::ClassId)) {
            FbxSurfacePhong* lPhong = static_cast<FbxSurfacePhong*>(lMaterial);
            assert(lPhong != nullptr);

            FbxDouble3 lDiffuseColor = lPhong->Diffuse;
            Color diffuseColor(lDiffuseColor[0],lDiffuseColor[1],lDiffuseColor[2]);
            material.setDiffuseColor(diffuseColor);

            FbxDouble3 lAmbientColor = lPhong->Ambient;
            Color ambientColor(lAmbientColor[0],lAmbientColor[1],lAmbientColor[2]);
            material.setAmbientColor(ambientColor);

            FbxDouble3 lSpecularColor = lPhong->Specular;
            Color specularColor(lSpecularColor[0],lSpecularColor[1],lSpecularColor[2]);
            material.setSpecularColor(specularColor);

            FbxDouble transparencyFactor = lPhong->TransparencyFactor;
            material.setTransparency(static_cast<float>(transparencyFactor));

            material.setShadingModel(Material::SHADING_PHONG);
        }


        //Textures
        for(int lTextureIndex = 0; lTextureIndex < FbxLayerElement::sTypeTextureCount; lTextureIndex++) {
            FbxProperty lProperty = lMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[lTextureIndex]);

            if (!lProperty.IsValid())
                continue;

            std::string propertyName(lProperty.GetName());

            std::vector<FbxTexture*> textures;

            int lMaterialTextureCount = lProperty.GetSrcObjectCount<FbxTexture>();

            for (int lMaterialTextureIndex = 0; lMaterialTextureIndex < lMaterialTextureCount; lMaterialTextureIndex++) {

                FbxLayeredTexture* lLayeredTexture = lProperty.GetSrcObject<FbxLayeredTexture>(lMaterialTextureIndex);
                if (lLayeredTexture) {
                    int lNbTextures = lLayeredTexture->GetSrcObjectCount<FbxTexture>();
                    for (int k = 0; k < lNbTextures; ++k) {
                        FbxTexture* lTexture = lLayeredTexture->GetSrcObject<FbxTexture>(k);
                        if (lTexture) {
                            textures.push_back(lTexture);
                        }
                    }
                }
                else {
                    FbxTexture* lTexture = lProperty.GetSrcObject<FbxTexture>(lMaterialTextureIndex);
                    if (lTexture) {
                        textures.push_back(lTexture);
                    }
                }
            }


            for (FbxTexture* lTexture : textures) {
                FbxFileTexture* lFileTexture = FbxCast<FbxFileTexture>(lTexture);
                if (propertyName == "DiffuseColor") {
                    material.setDiffuseMap(lFileTexture->GetFileName());
                }
                else if (propertyName == "AmbientColor") {
                    material.setAmbientMap(lFileTexture->GetFileName());
                }
                else if (propertyName == "SpecularColor") {
                    material.setSpecularMap(lFileTexture->GetFileName());
                }
                else if (propertyName == "NormalMap") {
                    material.setNormalMap(lFileTexture->GetFileName());
                }
                else if (propertyName == "TransparencyFactor") {
                    material.setTransparencyMap(lFileTexture->GetFileName());
                }

            }
        }

        materialMap[i] = data.meshData.materials.size();
        data.meshData.materials.push_back(material);
    }


    //Face materials

    FbxGeometryElementMaterial* lMaterialElement = lMesh->GetElementMaterial();
    if (lMaterialElement) {
        data.meshData.faceMaterials.resize(lastFaceId + lFaceCount);

        for(int i = 0; i < lFaceCount; i++) {
            Index fId = lastFaceId + i;

            int lMaterialIndex = 0;

            if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eAllSame) {
                lMaterialIndex = 0;
            }
            else if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygon) {
                int lMaterialIndex = 0;

                if(lMaterialElement->GetReferenceMode() == FbxGeometryElement::eDirect)
                    lMaterialIndex = i;

                if(lMaterialElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                    lMaterialIndex = lMaterialElement->GetIndexArray().GetAt(i);
            }

            data.meshData.faceMaterials[fId] = materialMap[lMaterialIndex];
        }
    }


    //Vertex normals

    FbxGeometryElementNormal* lNormalElement = lMesh->GetElementNormal();
    if (lNormalElement) {
        int lNormalCount = lNormalElement->GetDirectArray().GetCount();
        data.meshData.vertexNormals.resize(lastNormalId + lNormalCount);

        std::vector<Index> normalMap(lNormalCount);

        for (int i = 0; i < lNormalCount; i++) {
            Index nId = lastNormalId + i;

            FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(i);
            data.meshData.vertexNormals[nId] = VertexNormal(lNormal[0], lNormal[1], lNormal[2]);

            normalMap[i] = nId;
        }

        int lIndexByPolygonVertex = 0;

        data.meshData.faceVertexNormals.resize(lastFaceId + lFaceCount);
        for(int i = 0; i < lFaceCount; i++) {
            Index fId = lastFaceId + i;

            int lPolygonSize = lMesh->GetPolygonSize(i);

            data.meshData.faceVertexNormals[fId].resize(lPolygonSize);

            for(int j = 0; j < lPolygonSize; j++) {
                int lNormalIndex = 0;

                if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
                    int lControlPointIndex = lMesh->GetPolygonVertex(i, j);
                    if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
                        lNormalIndex = lControlPointIndex;
                    else if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                        lNormalIndex = lNormalElement->GetIndexArray().GetAt(lControlPointIndex);
                }
                else if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
                    if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
                        lNormalIndex = lIndexByPolygonVertex;
                    else if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                        lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndexByPolygonVertex);
                }

                data.meshData.faceVertexNormals[fId][j] = normalMap[lNormalIndex];

                lIndexByPolygonVertex++;
            }
        }
    }

    //Vertex UVs

    FbxGeometryElementUV* lUVElement = lMesh->GetElementUV();
    if (lUVElement) {
        int lUVCount = lUVElement->GetDirectArray().GetCount();
        data.meshData.vertexUVs.resize(lastUVId + lUVCount);

        std::vector<Index> UVMap(lUVCount);

        for (int i = 0; i < lUVCount; i++) {
            Index nId = lastUVId + i;

            FbxVector2 lUV = lUVElement->GetDirectArray().GetAt(i);
            data.meshData.vertexUVs[nId] = VertexUV(lUV[0], lUV[1]);

            UVMap[i] = nId;
        }

        int lIndexByPolygonVertex = 0;

        data.meshData.faceVertexUVs.resize(lastFaceId + lFaceCount);
        for(int i = 0; i < lFaceCount; i++) {
            Index fId = lastFaceId + i;

            int lPolygonSize = lMesh->GetPolygonSize(i);

            data.meshData.faceVertexUVs[fId].resize(lPolygonSize);

            for(int j = 0; j < lPolygonSize; j++) {
                int lUVIndex = 0;

                if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
                    int lControlPointIndex = lMesh->GetPolygonVertex(i, j);
                    if (lUVElement->GetReferenceMode() == FbxGeometryElement::eDirect)
                        lUVIndex = lControlPointIndex;
                    else if (lUVElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                        lUVIndex = lUVElement->GetIndexArray().GetAt(lControlPointIndex);
                }
                else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
                    if (lUVElement->GetReferenceMode() == FbxGeometryElement::eDirect)
                        lUVIndex = lIndexByPolygonVertex;
                    else if (lUVElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                        lUVIndex = lUVElement->GetIndexArray().GetAt(lIndexByPolygonVertex);
                }

                data.meshData.faceVertexUVs[fId][j] = UVMap[lUVIndex];

                lIndexByPolygonVertex++;
            }
        }
    }

    data.meshVertexMap.insert(std::make_pair(lMesh, vertexMap));
}

template<class FBXData>
void handleMeshAndSkeletonRecursive(
        FbxNode* lNode,
        FBXData& data)
{
    if (lNode->GetNodeAttribute()) {
        FbxNodeAttribute* lNodeAttribute = lNode->GetNodeAttribute();

        if (lNodeAttribute) {
            if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton) {
                handleSkeleton(lNode, data);
            }

            if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
                handleMesh(lNode, data);
            }
        }
    }

    const int lChildCount = lNode->GetChildCount();
    for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex) {
        handleMeshAndSkeletonRecursive(lNode->GetChild(lChildIndex), data);
    }
}

template<class FBXData>
void handleDeformerAndAnimationsRecursive(
        FbxScene* lScene,
        FbxNode* lNode,
        FBXData& data)
{
    typedef typename FBXData::AnimationData AnimationData;
    typedef typename FBXData::AnimationTransformation AnimationTransformation;
    typedef typename FBXData::SkeletonTransformation SkeletonTransformation;

    //If it is a mesh, handle the skinning weights
    if (lNode->GetNodeAttribute() && lNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh) {
        FbxMesh* lMesh = lNode->GetMesh();
        assert(lMesh);

        const std::vector<Index>& vertexMap = data.meshVertexMap.at(lMesh);

        int lVertexCount  = lMesh->GetControlPointsCount();
        const int lSkinCount = lMesh->GetDeformerCount(FbxDeformer::eSkin);

        for (int i = 0; i < lSkinCount; ++i) {
            FbxSkin* lSkinDeformer = (FbxSkin*) lMesh->GetDeformer(i, FbxDeformer::eSkin);

            int lClusterNumber = lSkinDeformer->GetClusterCount();
            for (int j = 0; j < lClusterNumber; ++j) {
                FbxCluster* lCluster = lSkinDeformer->GetCluster(j);

                FbxNode* linkedJoint = lCluster->GetLink();
                if (!linkedJoint)
                    continue;

                std::unordered_map<FbxNode*, Index>::iterator it = data.skeletonMap.find(linkedJoint);
                if (it != data.skeletonMap.end()) {
                    //Joint transformation link
                    Index jId = it->second;

                    FbxAMatrix transformLinkMatrix;
                    lCluster->GetTransformLinkMatrix(transformLinkMatrix);
                    SkeletonTransformation linkTransformation = transformationFromFBXMatrix<SkeletonTransformation>(transformLinkMatrix);

                    data.skeletonData.joints[jId] = linkTransformation;

                    //Skinning weights
                    int lClusterIndexNumber = lCluster->GetControlPointIndicesCount();
                    for (int k = 0; k < lClusterIndexNumber; ++k) {
                        int lIndex = lCluster->GetControlPointIndices()[k];

                        if (lIndex >= lVertexCount)
                            continue;

                        double lWeight = lCluster->GetControlPointWeights()[k];

                        if (lWeight == 0.0) {
                            continue;
                        }

                        assert(vertexMap.at(lIndex) != NULL_ID);
                        data.skinningWeightsData.weights.push_back(std::make_tuple(vertexMap.at(lIndex), jId, lWeight));
                    }
                }
            }
        }

        if (lSkinCount == 0) { //TODO CHECK IF CORRECT
            std::unordered_map<FbxNode*, Index>::iterator it = data.skeletonMap.find(lNode);
            if (it != data.skeletonMap.end()) {
                Index jId = it->second;

                int lMeshVertexCount = lMesh->GetControlPointsCount();

                for (int lIndex = 0; lIndex < lMeshVertexCount; ++lIndex) {
                    data.skinningWeightsData.weights.push_back(std::make_tuple(vertexMap.at(lIndex), jId, 1.0));
                }
            }
        }
    }

    //Handle animations
    std::unordered_map<FbxNode*, Index>::iterator it = data.skeletonMap.find(lNode);
    if (it != data.skeletonMap.end()) {
        Index jId = it->second;

        for (Index i = 0; i < data.animationDataVector.size(); i++) {
            AnimationData& animationData = data.animationDataVector[i];

            FbxAnimStack* lAnimStack = data.animationStacks[i];
            lScene->SetCurrentAnimationStack(lAnimStack);

            for (Index frameId = 0; frameId < animationData.times.size(); ++frameId) {
                const FbxTime& time = data.animationTimes[i][frameId];

                FbxAMatrix lAnimGlobalTransform = lNode->EvaluateGlobalTransform(time);
                AnimationTransformation animTransformation = transformationFromFBXMatrix<AnimationTransformation>(lAnimGlobalTransform);

                animationData.transformations[frameId][jId] = animTransformation;
            }
        }
    }

    const int lChildCount = lNode->GetChildCount();
    for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex) {
        handleDeformerAndAnimationsRecursive(lScene, lNode->GetChild(lChildIndex), data);
    }
}


template<class FBXData>
void handleBindPoseRecursive(
        FbxNode* lNode,
        FbxPose* lPose,
        FBXData& data)
{
    typedef typename FBXData::SkeletonTransformation SkeletonTransformation;

    //If it has a joint associated
    std::unordered_map<FbxNode*, Index>::iterator it = data.skeletonMap.find(lNode);
    if (it != data.skeletonMap.end()) {
        Index jId = it->second;

        FbxAMatrix geometryOffset = nodeGeometryOffset(lNode);
        FbxAMatrix posePosition = nodeGlobalPosition(lNode, lPose) * geometryOffset;

        SkeletonTransformation poseTransformation = transformationFromFBXMatrix<SkeletonTransformation>(posePosition);

        data.bindPoseTransformations[jId] = poseTransformation * data.skeletonData.joints[jId].inverse();
    }

    const int lChildCount = lNode->GetChildCount();
    for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex) {
        handleBindPoseRecursive(lNode->GetChild(lChildIndex), lPose, data);
    }
}

inline FbxAMatrix nodeGlobalPosition(
        FbxNode* lNode,
        FbxPose* lPose)
{
    FbxAMatrix lGlobalPosition;
    bool lPositionFound = false;

    if (lPose) {
        int lNodeIndex = lPose->Find(lNode);

        if (lNodeIndex > -1) {
            // The bind pose is always a global matrix.
            // If we have a rest pose, we need to check if it is
            // stored in global or local space.
            if (lPose->IsBindPose() || !lPose->IsLocalMatrix(lNodeIndex)) {
                lGlobalPosition = nodePoseMatrix(lPose, lNodeIndex);
            }
            else {
                FbxAMatrix lLocalPosition = nodePoseMatrix(lPose, lNodeIndex);

                // We have a local matrix, we need to convert it to
                // a global space matrix.
                FbxAMatrix currentParentGlobalPosition;

                if (lNode->GetParent()) {
                    currentParentGlobalPosition = nodeGlobalPosition(lNode->GetParent(), lPose);
                    lGlobalPosition = currentParentGlobalPosition * lLocalPosition;
                }
                else {
                    lGlobalPosition = lLocalPosition;
                }
            }

            lPositionFound = true;
        }
    }

    if (!lPositionFound) {
        lGlobalPosition = lNode->EvaluateGlobalTransform(FBXSDK_TIME_INFINITE);
    }

    return lGlobalPosition;
}

//Get the matrix of the given pose
inline FbxAMatrix nodePoseMatrix(FbxPose* lPose, int lNodeIndex)
{
    FbxAMatrix lPoseMatrix;
    FbxMatrix lMatrix = lPose->GetMatrix(lNodeIndex);

    memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));

    return lPoseMatrix;
}

//Get the geometry offset to a node. It is never inherited by the children.
inline FbxAMatrix nodeGeometryOffset(FbxNode* lNode)
{
    const FbxVector4 lT = lNode->GetGeometricTranslation(FbxNode::eSourcePivot);
    const FbxVector4 lR = lNode->GetGeometricRotation(FbxNode::eSourcePivot);
    const FbxVector4 lS = lNode->GetGeometricScaling(FbxNode::eSourcePivot);

    return FbxAMatrix(lT, lR, lS);
}

template<class T>
T transformationFromFBXMatrix(const FbxAMatrix& fbxT)
{
    FbxDouble3 fbxTranslation = fbxT.GetT();
    FbxDouble3 fbxRotation = fbxT.GetR();
    FbxDouble3 fbxScaling = fbxT.GetS();

    Rotation3d rot = transformationFromFBXRotation(fbxRotation);
    Translation3d tra = transformationFromFBXTranslation(fbxTranslation);

    T t(tra * rot);

    return t;
}

inline Translation3d transformationFromFBXTranslation(const FbxDouble3& fbxT)
{
    Vector3d traVec(fbxT[0], fbxT[1], fbxT[2]);
    Translation3d tra(traVec);

    return tra;
}


inline Rotation3d transformationFromFBXRotation(const FbxDouble3& fbxR)
{
    constexpr double degreesToRad = M_PI / 180.0;

    Vector3d angleVec(fbxR[0] * degreesToRad, fbxR[1] * degreesToRad, fbxR[2] * degreesToRad);
    Rotation3d rot = eulerAnglesToRotationXYZ(angleVec);

    return rot;
}

}

}

#endif

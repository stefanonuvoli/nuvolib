#include "model_io_fbx.h"

#ifdef NVL_FBXSDK_LOADED

#include <fbxsdk.h>

#include <unordered_map>
#include <stack>

#include <nvl/math/euler_angles.h>
#include <nvl/models/algorithms/animation_algorithms.h>

#include <nvl/utilities/file_utils.h>

namespace nvl {

template<class M, class S, class W, class A>
bool modelLoadDataFromFBX(
        const std::string& filename,
        IOModelData<M,S,W,A>& modelData,
        IOModelError& error,
        const IOModelMode& mode)
{
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

    constexpr double degreesToRad = M_PI / 180.0;

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

    MeshData meshData;
    SkeletonData skeletonData;
    SkinningWeightsData skinningWeightsData;
    std::vector<AnimationData> animationDataVector;

    //Handle skeleton mapping with strings
    std::unordered_map<std::string, Index> skeletonNameMap;

    Index animationStackNumber = static_cast<Index>(lScene->GetSrcObjectCount<FbxAnimStack>());

    std::vector<FbxAnimStack*> animationStacks;
    std::vector<std::vector<FbxTime>> animationTimes;
    for (Index i = 0; i < animationStackNumber; i++) {
        FbxAnimStack* lAnimStack = lScene->GetSrcObject<FbxAnimStack>(i);
        FbxString animStackName = lAnimStack->GetName();
        FbxTakeInfo* takeInfo = lScene->GetTakeInfo(animStackName);

        if (takeInfo) {
            FbxTime animationStartTime = takeInfo->mLocalTimeSpan.GetStart();
            FbxTime animationStopTime = takeInfo->mLocalTimeSpan.GetStop();
            FbxLongLong animationFrameNumber = animationStopTime.GetFrameCount(FbxTime::eDefaultMode) - animationStartTime.GetFrameCount(FbxTime::eDefaultMode) + 1;
    
            AnimationData animationData;
            animationData.name = animStackName;
            animationData.times.resize(animationFrameNumber);
            animationData.transformations.resize(animationFrameNumber);
    
            double offsetTime = static_cast<double>(animationStartTime.GetMilliSeconds()) / 1000;
            std::vector<FbxTime> fbxTimes(animationFrameNumber);
            for (FbxLongLong j = 0; j < animationFrameNumber; ++j) {
                FbxTime currTime;
                currTime.SetFrame(animationStartTime.GetFrameCount(FbxTime::eDefaultMode) + j, FbxTime::eDefaultMode);
    
                fbxTimes[j] = currTime;
    
                double time = static_cast<double>(currTime.GetMilliSeconds()) / 1000;
                time -= offsetTime;
                animationData.times[j] = time;
            }
    
            animationTimes.push_back(fbxTimes);
            animationStacks.push_back(lAnimStack);
            animationDataVector.push_back(animationData);
        }
    }

    std::unordered_map<FbxMesh*, std::vector<Index>> meshVertexMap;

    //Root node
    FbxNode* lRootNode = lScene->GetRootNode();
    modelName = lRootNode->GetName();

    std::stack<FbxNode*> nodeStack;

    for(int i = 0; i < lRootNode->GetChildCount(); i++) {
        nodeStack.push(lRootNode->GetChild(i));
    }

    while (!nodeStack.empty()) {
        FbxNode* currentNode = nodeStack.top();
        nodeStack.pop();

        if (!currentNode) {
            continue;
        }

        FbxNodeAttribute::EType type = FbxNodeAttribute::EType::eNull;

        if (currentNode->GetNodeAttribute() && currentNode->GetNodeAttribute()->GetAttributeType()) {
            type = currentNode->GetNodeAttribute()->GetAttributeType();
        }

        if (type == FbxNodeAttribute::eSkeleton) { //Skeleton
            FbxSkeleton* lSkeleton = currentNode->GetSkeleton();
            assert(lSkeleton);

            if (lSkeleton->IsSkeletonRoot()) {
                std::stack<FbxNode*> jointStack;
                jointStack.push(currentNode);

                while (!jointStack.empty()) {
                    FbxNode* jointNode = jointStack.top();
                    jointStack.pop();

                    const std::string jointName = jointNode->GetName();

                    FbxAMatrix lJointGlobalTransform = jointNode->EvaluateGlobalTransform();
                    FbxDouble3 lJointTranslation = lJointGlobalTransform.GetT();
                    FbxDouble3 lJointRotation = lJointGlobalTransform.GetR();
                    FbxDouble3 lJointScaling = lJointGlobalTransform.GetS();

                    Vector3d jointAngleVec(lJointRotation[0] * degreesToRad, lJointRotation[1] * degreesToRad, lJointRotation[2] * degreesToRad);
                    Vector3d jointTraVec(lJointTranslation[0], lJointTranslation[1], lJointTranslation[2]);

                    Rotation3d jointRot = eulerAnglesToRotationXYZ(jointAngleVec);
                    Translation3d jointTra(jointTraVec);
                    SkeletonTransformation jointTransformation(jointTra * jointRot);

                    Index parentJointId = NULL_ID;
                    if (jointNode != currentNode) { //It is not root
                        std::string parentName(jointNode->GetParent()->GetName());

                        std::unordered_map<std::string, Index>::iterator it = skeletonNameMap.find(parentName);

                        assert(it != skeletonNameMap.end());
                        parentJointId = it->second;
                    }

                    Index jId = skeletonData.joints.size();
                    skeletonData.joints.push_back(jointTransformation);
                    skeletonData.names.push_back(jointName);
                    skeletonData.parents.push_back(parentJointId);

                    skeletonNameMap.insert(std::make_pair(jointName, jId));

                    for (int i = 0; i < jointNode->GetChildCount(); i++) {
                        jointStack.push(jointNode->GetChild(i));
                    }
                }
            }
        }
        else if (type == FbxNodeAttribute::eMesh) { //Mesh
            FbxMesh* lMesh = currentNode->GetMesh();
            assert(lMesh);

            int lVertexNumber  = lMesh->GetControlPointsCount();
            int lFaceNumber = lMesh->GetPolygonCount();

            std::vector<Index> vertexMap(lVertexNumber, NULL_ID);

            Size lastVertexId = meshData.vertices.size();
            Size lastFaceId = meshData.faces.size();
            Size lastNormalId = meshData.vertexNormals.size();
            Size lastUVId = meshData.vertexUVs.size();


            //Vertices

            meshData.vertices.resize(lastVertexId + lVertexNumber);

            FbxVector4* lControlPoints = lMesh->GetControlPoints();
            FbxAMatrix t = currentNode->EvaluateGlobalTransform();

            for (int i = 0; i < lVertexNumber; i++) {
                FbxVector4 controlPoint = t.MultT(lControlPoints[i]);

                Index vId = lastVertexId + i;

                meshData.vertices[vId] = Point(controlPoint[0], controlPoint[1], controlPoint[2]);

                vertexMap[i] = vId;
            }


            //Faces

            meshData.faces.resize(lastFaceId + lFaceNumber);

            for (int i = 0; i < lFaceNumber; i++) {
                int lFaceSize = lMesh->GetPolygonSize(i);

                Index fId = lastFaceId + i;

                std::vector<Index>& newFace = meshData.faces[fId];
                newFace.resize(lFaceSize);

                for (int j = 0; j < lFaceSize; j++) {
                    int lControlPointIndex = lMesh->GetPolygonVertex(i, j);
                    newFace[j] = (vertexMap[lControlPointIndex]);
                }
            }


            //Materials

            int materialNumber = currentNode->GetMaterialCount();

            std::vector<Index> materialMap(materialNumber);

            for (int i = 0; i < materialNumber; i++) {
                FbxSurfaceMaterial* lMaterial = currentNode->GetMaterial(i);

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

                materialMap[i] = meshData.materials.size();
                meshData.materials.push_back(material);
            }


            //Face materials

            FbxGeometryElementMaterial* lMaterialElement = lMesh->GetElementMaterial();
            if (lMaterialElement) {
                meshData.faceMaterials.resize(lastFaceId + lFaceNumber);

                for(int i = 0; i < lFaceNumber; i++) {
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

                    meshData.faceMaterials[fId] = materialMap[lMaterialIndex];
                }
            }


            //Vertex normals

            FbxGeometryElementNormal* lNormalElement = lMesh->GetElementNormal();
            if (lNormalElement) {
                if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
                    meshData.vertexNormals.resize(lastNormalId + lVertexNumber);

                    for (int i = 0; i < lVertexNumber; i++) {
                        Index nId = lastNormalId + i;

                        int lNormalIndex = 0;

                        if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
                            lNormalIndex = i;
                        else if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                            lNormalIndex = lNormalElement->GetIndexArray().GetAt(i);

                        FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
                        meshData.vertexNormals[nId] = VertexNormal(lNormal[0], lNormal[1], lNormal[2]);
                    }
                }
                else if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
                    int lNormalNumber = lNormalElement->GetDirectArray().GetCount();
                    meshData.vertexNormals.resize(lastNormalId + lNormalNumber);

                    for (int i = 0; i < lNormalNumber; i++) {
                        Index nId = lastNormalId + i;

                        FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(i);
                        meshData.vertexNormals[nId] = VertexNormal(lNormal[0], lNormal[1], lNormal[2]);
                    }

                    int lIndexByPolygonVertex = 0;

                    meshData.faceVertexNormals.resize(lastFaceId + lFaceNumber);
                    for(int i = 0; i < lFaceNumber; i++) {
                        Index fId = lastFaceId + i;

                        int lPolygonSize = lMesh->GetPolygonSize(i);

                        meshData.faceVertexNormals[fId].resize(lPolygonSize);

                        for(int j = 0; j < lPolygonSize; j++) {
                            int lNormalIndex = 0;

                            if(lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
                                lNormalIndex = lIndexByPolygonVertex;

                            if(lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                                lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndexByPolygonVertex);

                            meshData.faceVertexNormals[fId][j] = lNormalIndex;

                            lIndexByPolygonVertex++;
                        }
                    }
                }
            }


            //Vertex UVs

            FbxGeometryElementUV* lUVElement = lMesh->GetElementUV();
            if (lUVElement) {
                if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
                    meshData.vertexUVs.resize(lastUVId + lVertexNumber);

                    for (int i = 0; i < lVertexNumber; i++) {
                        Index nId = lastUVId + i;

                        int lUVIndex = 0;

                        if (lUVElement->GetReferenceMode() == FbxGeometryElement::eDirect)
                            lUVIndex = i;
                        else if (lUVElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                            lUVIndex = lUVElement->GetIndexArray().GetAt(i);

                        FbxVector2 lUV = lUVElement->GetDirectArray().GetAt(lUVIndex);
                        meshData.vertexUVs[nId] = VertexUV(lUV[0], lUV[1]);
                    }
                }
                else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
                    int lUVNumber = lUVElement->GetDirectArray().GetCount();
                    meshData.vertexUVs.resize(lastUVId + lUVNumber);

                    for (int i = 0; i < lUVNumber; i++) {
                        Index nId = lastUVId + i;

                        FbxVector2 lUV = lUVElement->GetDirectArray().GetAt(i);
                        meshData.vertexUVs[nId] = VertexUV(lUV[0], lUV[1]);
                    }

                    int lIndexByPolygonVertex = 0;

                    meshData.faceVertexUVs.resize(lastFaceId + lFaceNumber);
                    for(int i = 0; i < lFaceNumber; i++) {
                        Index fId = lastFaceId + i;

                        int lPolygonSize = lMesh->GetPolygonSize(i);

                        meshData.faceVertexUVs[fId].resize(lPolygonSize);

                        for(int j = 0; j < lPolygonSize; j++) {
                            int lUVIndex = 0;

                            if(lUVElement->GetReferenceMode() == FbxGeometryElement::eDirect)
                                lUVIndex = lIndexByPolygonVertex;

                            if(lUVElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
                                lUVIndex = lUVElement->GetIndexArray().GetAt(lIndexByPolygonVertex);

                            meshData.faceVertexUVs[fId][j] = lUVIndex;

                            lIndexByPolygonVertex++;
                        }
                    }
                }

                meshVertexMap.insert(std::make_pair(lMesh, vertexMap));
            }
        }

        for (int i = 0; i < currentNode->GetChildCount(); i++) {
            nodeStack.push(currentNode->GetChild(i));
        }
    }

    for (AnimationData& animationData : animationDataVector) {
        for (nvl::Index frameId = 0; frameId < animationData.transformations.size(); ++frameId) {
            animationData.transformations[frameId].resize(skeletonData.joints.size());
        }
    }

    for(int i = 0; i < lRootNode->GetChildCount(); i++) {
        nodeStack.push(lRootNode->GetChild(i));
    }

    while (!nodeStack.empty()) {
        FbxNode* currentNode = nodeStack.top();
        nodeStack.pop();

        if (!currentNode) {
            continue;
        }

        FbxNodeAttribute::EType type = FbxNodeAttribute::EType::eNull;

        if (currentNode->GetNodeAttribute() && currentNode->GetNodeAttribute()->GetAttributeType()) {
            type = currentNode->GetNodeAttribute()->GetAttributeType();
        }

        if (type == FbxNodeAttribute::eSkeleton) { //Skeleton
            FbxSkeleton* lSkeleton = currentNode->GetSkeleton();
            assert(lSkeleton);

            if (lSkeleton->IsSkeletonRoot()) {
                std::stack<FbxNode*> jointStack;
                jointStack.push(currentNode);

                while (!jointStack.empty()) {
                    FbxNode* jointNode = jointStack.top();
                    jointStack.pop();

                    const std::string jointName = jointNode->GetName();

                    std::unordered_map<std::string, Index>::iterator it = skeletonNameMap.find(jointName);
                    if (it != skeletonNameMap.end()) {
                        Index jId = it->second;

                        for (Index i = 0; i < animationDataVector.size(); i++) {
                            AnimationData& animationData = animationDataVector[i];

                            FbxAnimStack* lAnimStack = animationStacks[i];
                            lScene->SetCurrentAnimationStack(lAnimStack);

                            for (Index frameId = 0; frameId < animationData.times.size(); ++frameId) {
                                const FbxTime& time = animationTimes[i][frameId];

                                FbxAMatrix lAnimGlobalTransform = jointNode->EvaluateGlobalTransform(time);
                                FbxVector4 lAnimRotation = lAnimGlobalTransform.GetR();
                                FbxVector4 lAnimTranslation = lAnimGlobalTransform.GetT();

                                Vector3d animAngleVec(lAnimRotation[0] * degreesToRad, lAnimRotation[1] * degreesToRad, lAnimRotation[2] * degreesToRad);
                                Vector3d animTraVec(lAnimTranslation[0], lAnimTranslation[1], lAnimTranslation[2]);

                                Rotation3d animRot = eulerAnglesToRotationXYZ(animAngleVec);
                                Translation3d animTra(animTraVec);
                                AnimationTransformation animTransformation(animTra * animRot);

                                animationData.transformations[frameId][jId] = animTransformation;
                            }
                        }

                        FbxNodeAttribute::EType jointType = FbxNodeAttribute::EType::eNull;

                        if (jointNode->GetNodeAttribute() && jointNode->GetNodeAttribute()->GetAttributeType()) {
                            jointType = jointNode->GetNodeAttribute()->GetAttributeType();
                        }

                        if (jointType == FbxNodeAttribute::eMesh) {
                            FbxMesh* lJointMesh = jointNode->GetMesh();
                            int lVertexNumber  = lJointMesh->GetControlPointsCount();
                            const std::vector<Index>& vertexMap = meshVertexMap.at(lJointMesh);

                            for (int lIndex = 0; lIndex < lVertexNumber; ++lIndex) {
                                skinningWeightsData.weights.push_back(std::make_tuple(vertexMap.at(lIndex), jId, 1.0));
                            }
                        }
                    }


                    for (int i = 0; i < jointNode->GetChildCount(); i++) {
                        jointStack.push(jointNode->GetChild(i));
                    }
                }
            }
            else {
                error = IO_MODEL_FORMAT_NON_RECOGNISED;
            }
        }
        else if (type == FbxNodeAttribute::eMesh) { //Mesh
            FbxMesh* lMesh = currentNode->GetMesh();
            assert(lMesh);

            int lVertexNumber  = lMesh->GetControlPointsCount();
            const std::vector<Index>& vertexMap = meshVertexMap.at(lMesh);


            //Get joint bind pose

            const int lSkinNumber = lMesh->GetDeformerCount(FbxDeformer::eSkin);

            for (int i = 0; i < lSkinNumber; ++i) {
                FbxSkin* lSkinDeformer = (FbxSkin*) lMesh->GetDeformer(i, FbxDeformer::eSkin);

                int lClusterNumber = lSkinDeformer->GetClusterCount();
                for (int j = 0; j < lClusterNumber; ++j) {
                    FbxCluster* lCluster = lSkinDeformer->GetCluster(j);

                    FbxNode* linkedJoint = lCluster->GetLink();
                    if (!linkedJoint)
                        continue;

                    std::string jointName = lCluster->GetLink()->GetName();
                    std::unordered_map<std::string, Index>::iterator it = skeletonNameMap.find(jointName);
                    if (it != skeletonNameMap.end()) {
                        //Joint bind pose
                        Index jId = it->second;

                        FbxAMatrix transformLinkMatrix;
                        lCluster->GetTransformLinkMatrix(transformLinkMatrix);

                        FbxDouble3 lJointTranslation = transformLinkMatrix.GetT();
                        FbxDouble3 lJointRotation = transformLinkMatrix.GetR();
                        FbxDouble3 lJointScaling = transformLinkMatrix.GetS();

                        Vector3d jointAngleVec(lJointRotation[0] * degreesToRad, lJointRotation[1] * degreesToRad, lJointRotation[2] * degreesToRad);
                        Vector3d jointTraVec(lJointTranslation[0], lJointTranslation[1], lJointTranslation[2]);

                        Rotation3d jointRot = eulerAnglesToRotationXYZ(jointAngleVec);
                        Translation3d jointTra(jointTraVec);
                        SkeletonTransformation jointTransformation(jointTra * jointRot);

                        skeletonData.joints[jId] = jointTransformation;


                        //Skinning weights
                        int lClusterIndexNumber = lCluster->GetControlPointIndicesCount();
                        for (int k = 0; k < lClusterIndexNumber; ++k) {
                            int lIndex = lCluster->GetControlPointIndices()[k];

                            if (lIndex >= lVertexNumber)
                                continue;

                            double lWeight = lCluster->GetControlPointWeights()[k];

                            if (lWeight == 0.0) {
                                continue;
                            }

                            std::string jointName = linkedJoint->GetName();

                            assert (vertexMap.at(lIndex) != NULL_ID);
                            skinningWeightsData.weights.push_back(std::make_tuple(vertexMap.at(lIndex), jId, lWeight));
                        }
                    }
                    else {
                        error = IO_MODEL_FORMAT_NON_RECOGNISED;
                    }
                }
            }
        }

        for (int i = 0; i < currentNode->GetChildCount(); i++) {
            nodeStack.push(currentNode->GetChild(i));
        }
    }

    modelData.name = modelName;

    meshLoadData(modelData.mesh, meshData, mode.meshMode);
    skeletonLoadData(modelData.skeleton, skeletonData, mode.skeletonMode);
    modelData.skinningWeights.initialize(modelData.mesh.nextVertexId(), modelData.skeleton.jointNumber());
    skinningWeightsLoadData(modelData.skinningWeights, skinningWeightsData, mode.skinningWeightsMode);

    std::vector<SkeletonTransformation> localBindPoses(modelData.skeleton.jointNumber());
    for (JointId jId = 0; jId < modelData.skeleton.jointNumber(); ++jId) {
        localBindPoses[jId] = modelData.skeleton.joint(jId).bindPose();
    }
    nvl::animationLocalFromGlobalTransformations(modelData.skeleton, localBindPoses);

    for (AnimationData& animationData : animationDataVector) {
        Animation animation;

        animationLoadData(animation, animationData, mode.animationMode);

        nvl::animationLocalFromGlobalFrames(modelData.skeleton, animation.keyframes());

        bool isIdentity = true;
        for (nvl::Index frameId = 0; frameId < animation.keyframeNumber(); ++frameId) {
            for (JointId jId = 0; jId < modelData.skeleton.jointNumber(); ++jId) {
                AnimationTransformation& t = animation.keyframe(frameId).transformation(jId);

                t = localBindPoses[jId].inverse() * t;

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

    return true;
}

}

#endif


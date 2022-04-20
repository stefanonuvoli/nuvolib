/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "model_io_fbx.h"

#ifdef NVL_FBXSDK_LOADED

#include <fbxsdk.h>

#include <unordered_map>
#include <stack>

#include <nvl/math/euler_angles.h>
#include <nvl/models/algorithms/model_deformation.h>
#include <nvl/models/algorithms/model_transformations.h>
#include <nvl/models/algorithms/animation_poses.h>
#include <nvl/models/structures/skeleton.h>

#include <nvl/utilities/file_utils.h>

namespace nvl {

namespace internal {

template<class M, class S, class W, class A>
struct FBXData {
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
    typedef typename Skeleton::Scalar SkeletonScalar;
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

    std::vector<std::vector<SkeletonTransformation>> bindPoseTransformations;
    std::vector<SkeletonTransformation> zeroPoseTransformations;
    std::vector<SkeletonTransformation> defaultPoseTransformations;

    //Useful mapping information
    std::unordered_map<FbxMesh*, std::vector<Index>> meshVertexMap;
    std::unordered_map<FbxNode*, Index> skeletonMap;
    std::vector<FbxAnimStack*> animationStacks;
    std::vector<std::vector<FbxTime>> animationTimes;
};


template<class FBXData>
void handleScene(
        FbxScene* lScene,
        FBXData& data,
        const IOModelMode& mode);
template<class FBXData>
void handleSkeleton(
        FbxNode* lNode,
        FBXData& data);
template<class FBXData>
void handleSkeletonInner(
        FbxNode* lNode,
        Index parentJointId,
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
        FBXData& data,
        const Index& poseId);
template<class FBXData>
void handleZeroPoseRecursive(
        FbxNode* lNode,
        FBXData& data);


template<class FBXData>
void createScene(
        FbxScene* lScene,
        const FBXData& data,
        const IOModelMode& mode);
template<class FBXData>
FbxNode* createMesh(
        FbxScene* lScene,
        const FBXData& data,
        const IOModelMode& mode);
template<class FBXData>
FbxNode* createSkeleton(
        FbxScene* lScene,
        const FBXData& data,
        const IOModelMode& mode,
        std::vector<FbxNode*>& jMap);
template<class FBXData>
FbxSkin* createSkin(
        FbxScene* lScene,
        const FBXData& data,
        const IOModelMode& mode,
        const std::vector<FbxNode*>& jMap);
template<class FBXData>
std::vector<FbxAnimStack*> createAnimations(
        FbxScene* lScene,
        const FBXData& data,
        const IOModelMode& mode,
        std::vector<FbxNode*>& jMap);


FbxAMatrix nodeGlobalPosition(
       FbxNode* lNode,
       FbxPose* lPose);
FbxAMatrix nodePoseMatrix(FbxPose* lPose, int lNodeIndex);
FbxAMatrix nodeGeometryOffset(FbxNode* lNode);

template<class T>
T transformationFromFBXMatrix(const FbxAMatrix& fbxT);
Translation3d translationFromFBXTranslation(const FbxVector4& fbxT);
Scaling3d scalingFromFBXScaling(const FbxVector4& fbxS);
Rotation3d rotationFromFBXRotation(const FbxVector4& fbxR);

template<class T>
FbxAMatrix FBXTransformationFromMatrix(const T& t);
FbxVector4 FBXTranslationFromTranslation(const Translation3d& tra);
FbxVector4 FBXScalingFromScaling(const Scaling3d& sca);
FbxVector4 FBXRotationFromRotation(const Rotation3d& rot);
template<class V>
FbxVector4 FBXVector4FromVector(const V& v);
template<class V>
FbxVector2 FBXVector2FromVector(const V& v);

}

/**
 * @brief Load model from FBX file
 * @param filename Filename
 * @param model Model
 * @param error Error
 * @param mode Mode
 * @return True if loading has been successful
 */
template<class M, class S, class W, class A>
bool modelLoadDataFromFBX(
        const std::string& filename,
        IOModelData<M,S,W,A>& modelData,
        IOModelError& error,
        const IOModelMode& mode)
{
    typedef internal::FBXData<M,S,W,A> FBXData;

    typedef typename FBXData::AnimationData AnimationData;

    typedef typename FBXData::SkeletonTransformation SkeletonTransformation;

    typedef typename FBXData::Animation Animation;

    error = IO_MODEL_SUCCESS;

    //Initialize the SDK manager. This object handles memory management.
    FbxManager* lSdkManager = FbxManager::Create();
    if (lSdkManager == nullptr) {
        FBXSDK_printf("Error: Unable to create FBX Manager!\n");
        return false;
    }

    //Create the IO settings object.
    FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);

    //Load plugins from the executable directory (optional)
    FbxString lPath = FbxGetApplicationDirectory();
    lSdkManager->LoadPluginsDirectory(lPath.Buffer());

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

    //Handle the scene
    handleScene(lScene, fbxData, mode);

    //Fill data
    modelData.name = modelName;

    //Load mesh and skeleton
    meshLoadData(modelData.mesh, fbxData.meshData, mode.meshMode);
    skeletonLoadData(modelData.skeleton, fbxData.skeletonData, mode.skeletonMode);

    //Initialize skinning weights
    modelData.skinningWeights.initialize(modelData.mesh.nextVertexId(), modelData.skeleton.jointNumber());

    //Load skinning weights
    skinningWeightsLoadData(modelData.skinningWeights, fbxData.skinningWeightsData, mode.skinningWeightsMode);

    //Load animations
    for (Index aId = 0; aId < fbxData.animationDataVector.size(); ++aId) {
        AnimationData& animationData = fbxData.animationDataVector[aId];
        Animation animation;

        animationLoadData(animation, animationData, mode.animationMode);

        modelData.animations.push_back(animation);
    }

    //Get the pose deformation
    std::vector<SkeletonTransformation> poseDeformation;
    if (mode.FBXDeformToPose == IOModelFBXPose::IO_FBX_POSE_DEFAULT) {
        //Deformation with default pose
        poseDeformation = fbxData.defaultPoseTransformations;
    }
    else if (mode.FBXDeformToPose == IOModelFBXPose::IO_FBX_POSE_ZERO) {
        //Deformation with pose at time zero
        poseDeformation = fbxData.zeroPoseTransformations;
    }
    else if (mode.FBXDeformToPose == IOModelFBXPose::IO_FBX_POSE_BIND && !fbxData.bindPoseTransformations.empty()) {
        //Deformation with last bind pose
        poseDeformation = fbxData.bindPoseTransformations[fbxData.bindPoseTransformations.size() - 1];
    }

    //Deform the model
    if (!poseDeformation.empty()) {
        nvl::skeletonPoseDeformationFromGlobal(modelData.skeleton, poseDeformation);
        modelDeformDualQuaternionSkinning(modelData.mesh, modelData.skeleton, modelData.skinningWeights, modelData.animations, poseDeformation, true, false);
    }

    return true;
}

/**
 * @brief Save model to FBX file
 * @param filename Filename
 * @param model Model
 * @param error Error
 * @param mode Mode
 * @return True if saving has been successful
 */
template<class M, class S, class W, class A>
bool modelSaveDataToFBX(
        const std::string& filename,
        const IOModelData<M,S,W,A>& modelData,
        IOModelError& error,
        const IOModelMode& mode)
{
    typedef internal::FBXData<M,S,W,A> FBXData;

    error = IO_MODEL_SUCCESS;

    //Initialize the SDK manager. This object handles memory management.
    FbxManager* lSdkManager = FbxManager::Create();
    if (lSdkManager == nullptr) {
        FBXSDK_printf("Error: Unable to create FBX Manager!\n");
        return false;
    }

    //Create the IO settings object.
    FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);

    //Load plugins from the executable directory (optional)
    FbxString lPath = FbxGetApplicationDirectory();
    lSdkManager->LoadPluginsDirectory(lPath.Buffer());

    //Create an FBX scene. This object holds most objects imported/exported from/to files.
    FbxScene* lScene = FbxScene::Create(lSdkManager, "My Scene");
    if (!lScene) {
        FBXSDK_printf("Error: Unable to create FBX scene!\n");
        lSdkManager->Destroy();
        return false;
    }

    //Load FBX data
    FBXData fbxData;
    meshSaveData(modelData.mesh, fbxData.meshData, mode.meshMode);
    skeletonSaveData(modelData.skeleton, fbxData.skeletonData, mode.skeletonMode);
    skinningWeightsSaveData(modelData.skinningWeights, fbxData.skinningWeightsData, mode.skinningWeightsMode);
    fbxData.animationDataVector.resize(modelData.animations.size());
    for (Index aId = 0; aId < modelData.animations.size(); ++aId) {
        animationSaveData(modelData.animations[aId], fbxData.animationDataVector[aId], mode.animationMode);
    }

    createScene(lScene, fbxData, mode);

    //Create an exporter.
    FbxExporter* lExporter = FbxExporter::Create(lSdkManager, "");
    int pFileFormat = lSdkManager->GetIOPluginRegistry()->GetNativeWriterFormat();

    //Set the export states. By default, the export states are always set to
    //true except for the option eEXPORT_TEXTURE_AS_EMBEDDED.
    (lSdkManager->GetIOSettings())->SetBoolProp(EXP_FBX_MATERIAL,        true);
    (lSdkManager->GetIOSettings())->SetBoolProp(EXP_FBX_TEXTURE,         true);
    (lSdkManager->GetIOSettings())->SetBoolProp(EXP_FBX_EMBEDDED,        false);
    (lSdkManager->GetIOSettings())->SetBoolProp(EXP_FBX_SHAPE,           true);
    (lSdkManager->GetIOSettings())->SetBoolProp(EXP_FBX_GOBO,            true);
    (lSdkManager->GetIOSettings())->SetBoolProp(EXP_FBX_ANIMATION,       true);
    (lSdkManager->GetIOSettings())->SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

    // Initialize the exporter by providing a filename.
    if(lExporter->Initialize(filename.c_str(), pFileFormat, lSdkManager->GetIOSettings()) == false) {
        FBXSDK_printf("Call to FbxExporter::Initialize() failed.\n");
        FBXSDK_printf("Error returned: %s\n\n", lExporter->GetStatus().GetErrorString());
        lSdkManager->Destroy();
        return false;
    }

    //Export the scene.
    bool lStatus = lExporter->Export(lScene);

    //Destroy the exporter.
    lExporter->Destroy();
    lSdkManager->Destroy();

    return lStatus;
}

namespace internal {

template<class FBXData>
void handleScene(
        FbxScene* lScene,
        FBXData& data,
        const IOModelMode& mode)
{
    typedef typename FBXData::AnimationData AnimationData;

    typedef typename FBXData::SkeletonTransformation SkeletonTransformation;

    typedef typename FBXData::Animation Animation;

    FbxNode* lRootNode = lScene->GetRootNode();
    if (lRootNode) {
        internal::handleMeshAndSkeletonRecursive(lRootNode, data);

        Index animationStackNumber = static_cast<Index>(lScene->GetSrcObjectCount<FbxAnimStack>());

        //Initialize animation stacks
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
                    animationData.transformations[fId].resize(data.skeletonData.joints.size());
                }

                data.animationTimes.push_back(fbxTimes);
                data.animationStacks.push_back(lAnimStack);
                data.animationDataVector.push_back(animationData);
            }
        }

        if (mode.FBXSavePoses) {
            //Add default pose
            AnimationData animationDataDefault;
            animationDataDefault.name = "Pose default";

            animationDataDefault.times.push_back(0.0);
            animationDataDefault.transformations.push_back(data.defaultPoseTransformations);

            data.animationDataVector.push_back(animationDataDefault);
        }


        if (mode.FBXSavePoses || mode.FBXDeformToPose == IOModelFBXPose::IO_FBX_POSE_ZERO) {
            data.zeroPoseTransformations.resize(data.skeletonData.joints.size());
            internal::handleZeroPoseRecursive(lRootNode, data);

            if (mode.FBXSavePoses) {
                //Add zero pose
                AnimationData animationDataZero;
                animationDataZero.name = "Pose zero";

                animationDataZero.times.push_back(0.0);
                animationDataZero.transformations.push_back(data.zeroPoseTransformations);

                data.animationDataVector.push_back(animationDataZero);
            }
        }

        if (mode.FBXSavePoses || mode.FBXDeformToPose == IOModelFBXPose::IO_FBX_POSE_BIND) {
            //Get bind pose of all the stacks
            for (Index i = 0; i < animationStackNumber; i++) {
                FbxAnimStack* lAnimStack = data.animationStacks[i];
                lScene->SetCurrentAnimationStack(lAnimStack);
                FbxString animStackName = lAnimStack->GetName();

                const int lPoseCount = lScene->GetPoseCount();
                for (int i = 0; i < lPoseCount; ++i) {
                    FbxPose* currentPose = lScene->GetPose(i);

                    data.bindPoseTransformations.push_back(std::vector<SkeletonTransformation>(data.skeletonData.joints.size()));

                    Index poseId = data.bindPoseTransformations.size() - 1;
                    handleBindPoseRecursive(lRootNode, currentPose, data, poseId);

                    if (mode.FBXSavePoses) {
                        if (currentPose->IsBindPose()) {
                            AnimationData animationDataBind;
                            animationDataBind.name = "Bind pose " + animStackName;

                            animationDataBind.times.push_back(0.0);
                            animationDataBind.transformations.push_back(data.bindPoseTransformations[poseId]);

                            data.animationDataVector.push_back(animationDataBind);
                        }
                        else if (currentPose->IsRestPose()) {
                            AnimationData animationDataBind;
                            animationDataBind.name = "Rest pose " + animStackName;

                            animationDataBind.times.push_back(0.0);
                            animationDataBind.transformations.push_back(data.bindPoseTransformations[poseId]);

                            data.animationDataVector.push_back(animationDataBind);
                        }
                    }
                }
            }
        }

        //Handle deformer and animations
        internal::handleDeformerAndAnimationsRecursive(lScene, lRootNode, data);
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
void handleSkeletonInner(
        FbxNode* lNode,
        Index parentJointId,
        FBXData& data)
{
    typedef typename FBXData::SkeletonTransformation SkeletonTransformation;

    FbxSkeleton* lSkeleton = lNode->GetSkeleton();
    bool isSkeleton = lSkeleton != nullptr ? true : false;

    FbxAMatrix geometryOffset = nodeGeometryOffset(lNode);
    FbxAMatrix lJointGlobalTransform = lNode->EvaluateGlobalTransform() * geometryOffset;
    SkeletonTransformation jointTransformation = transformationFromFBXMatrix<SkeletonTransformation>(lJointGlobalTransform);

    const std::string jointName = lNode->GetName();

    Index jId = data.skeletonData.joints.size();
    data.skeletonData.joints.push_back(jointTransformation);
    data.skeletonData.hidden.push_back(!isSkeleton);
    data.skeletonData.names.push_back(jointName);
    data.skeletonData.parents.push_back(parentJointId);

    data.skeletonMap.insert(std::make_pair(lNode, jId));

    data.defaultPoseTransformations.push_back(jointTransformation);

    const int lChildCount = lNode->GetChildCount();
    for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex) {
        handleSkeletonInner(lNode->GetChild(lChildIndex), jId, data);
    }
}

template<class FBXData>
void handleMesh(
        FbxNode* lNode,
        FBXData& data)
{
    typedef typename FBXData::Point Point;
    typedef typename FBXData::VertexNormal VertexNormal;
    typedef typename FBXData::VertexColor VertexColor;
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
    Size lastVertexColorId = data.meshData.vertexColors.size();


    //Vertices

    data.meshData.vertices.resize(lastVertexId + lVertexCount);

    FbxVector4* lControlPoints = lMesh->GetControlPoints();

    FbxAMatrix geometryOffset = nodeGeometryOffset(lNode);
    FbxAMatrix lGlobalTransform = lNode->EvaluateGlobalTransform() * geometryOffset;

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
            material.setTransparency(static_cast<float>(1.0 - transparencyFactor));

            material.setShadingModel(Material::SHADING_LAMBERT);
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
            material.setTransparency(static_cast<float>(1.0 - transparencyFactor));

            material.setShadingModel(Material::SHADING_PHONG);
        }

        //Textures
        for (int lTextureIndex = 0; lTextureIndex < FbxLayerElement::sTypeTextureCount; lTextureIndex++) {
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

        for (int i = 0; i < lFaceCount; i++) {
            Index fId = lastFaceId + i;

            int lMaterialIndex = 0;

            if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eAllSame) {
                lMaterialIndex = 0;
            }
            else if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygon) {
                if (lMaterialElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
                    lMaterialIndex = i;
                }
                else if (lMaterialElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
                    lMaterialIndex = lMaterialElement->GetIndexArray().GetAt(i);
                }
            }

            data.meshData.faceMaterials[fId] = materialMap[lMaterialIndex];
        }
    }


    //Vertex normals

    FbxGeometryElementNormal* lNormalElement = lMesh->GetElementNormal();
    if (lNormalElement) {

        if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
            data.meshData.vertexNormals.resize(lastVertexId + lVertexCount);

            for (int i = 0; i < lVertexCount; i++) {
                Index vId = lastVertexId + i;

                int lNormalIndex = 0;
                if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
                    lNormalIndex = i;
                }
                else if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
                    lNormalIndex = lNormalElement->GetIndexArray().GetAt(i);
                }

                FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);
                data.meshData.vertexNormals[vId] = VertexNormal(lNormal[0], lNormal[1], lNormal[2]);
            }

        }
        else if (lNormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
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
            for (int i = 0; i < lFaceCount; i++) {
                Index fId = lastFaceId + i;

                int lPolygonSize = lMesh->GetPolygonSize(i);

                data.meshData.faceVertexNormals[fId].resize(lPolygonSize);

                for (int j = 0; j < lPolygonSize; j++) {
                    int lNormalIndex = 0;

                    if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
                        lNormalIndex = lIndexByPolygonVertex;
                    }
                    else if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
                        lNormalIndex = lNormalElement->GetIndexArray().GetAt(lIndexByPolygonVertex);
                    }

                    data.meshData.faceVertexNormals[fId][j] = normalMap[lNormalIndex];

                    lIndexByPolygonVertex++;
                }
            }
        }
    }


    //Vertex UVs

    FbxGeometryElementUV* lUVElement = lMesh->GetElementUV();
    if (lUVElement) {

        if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
            data.meshData.vertexUVs.resize(lastVertexId + lVertexCount);

            for (int i = 0; i < lVertexCount; i++) {
                Index vId = lastVertexId + i;

                int lUVIndex = 0;
                if (lUVElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
                    lUVIndex = i;
                }
                else if (lUVElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
                    lUVIndex = lUVElement->GetIndexArray().GetAt(i);
                }

                FbxVector2 lUV = lUVElement->GetDirectArray().GetAt(lUVIndex);
                data.meshData.vertexUVs[vId] = VertexUV(lUV[0], lUV[1]);
            }

        }
        else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
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
            for (int i = 0; i < lFaceCount; i++) {
                Index fId = lastFaceId + i;

                int lPolygonSize = lMesh->GetPolygonSize(i);

                data.meshData.faceVertexUVs[fId].resize(lPolygonSize);

                for (int j = 0; j < lPolygonSize; j++) {
                    int lUVIndex = 0;

                    if (lUVElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
                        lUVIndex = lIndexByPolygonVertex;
                    }
                    else if (lUVElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
                        lUVIndex = lUVElement->GetIndexArray().GetAt(lIndexByPolygonVertex);
                    }

                    data.meshData.faceVertexUVs[fId][j] = UVMap[lUVIndex];

                    lIndexByPolygonVertex++;
                }
            }
        }
    }


    //Vertex colors

    FbxGeometryElementVertexColor* lVertexColorElement = lMesh->GetElementVertexColor();
    if (lVertexColorElement) {
        if (lVertexColorElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
            data.meshData.vertexColors.resize(lastVertexId + lVertexCount);

            for (int i = 0; i < lVertexCount; i++) {
                Index vId = lastVertexId + i;

                int lColorIndex = 0;
                if (lVertexColorElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
                    lColorIndex = i;
                }
                else if (lVertexColorElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
                    lColorIndex = lVertexColorElement->GetIndexArray().GetAt(i);
                }

                FbxColor lColor = lVertexColorElement->GetDirectArray().GetAt(lColorIndex);
                data.meshData.vertexColors[vId] = VertexColor(lColor[0], lColor[1], lColor[2]);
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

        for (Index i = 0; i < data.animationStacks.size(); i++) {
            AnimationData& animationData = data.animationDataVector[i];

            FbxAnimStack* lAnimStack = data.animationStacks[i];
            lScene->SetCurrentAnimationStack(lAnimStack);

            for (Index fId = 0; fId < animationData.times.size(); ++fId) {
                FbxTime time = data.animationTimes[i][fId];

                FbxAMatrix geometryOffset = nodeGeometryOffset(lNode);
                FbxAMatrix lAnimGlobalTransform = lNode->EvaluateGlobalTransform(time) * geometryOffset;
                AnimationTransformation animTransformation = transformationFromFBXMatrix<AnimationTransformation>(lAnimGlobalTransform);

                animationData.transformations[fId][jId] = animTransformation;
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
        FBXData& data,
        const Index& poseId)
{
    typedef typename FBXData::SkeletonTransformation SkeletonTransformation;

    //If it has a joint associated
    std::unordered_map<FbxNode*, Index>::iterator it = data.skeletonMap.find(lNode);
    if (it != data.skeletonMap.end()) {
        Index jId = it->second;

        FbxAMatrix geometryOffset = nodeGeometryOffset(lNode);
        FbxAMatrix posePosition = nodeGlobalPosition(lNode, lPose) * geometryOffset;

        SkeletonTransformation poseTransformation = transformationFromFBXMatrix<SkeletonTransformation>(posePosition);

        data.bindPoseTransformations[poseId][jId] = poseTransformation;
    }

    const int lChildCount = lNode->GetChildCount();
    for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex) {
        handleBindPoseRecursive(lNode->GetChild(lChildIndex), lPose, data, poseId);
    }
}

template<class FBXData>
void handleZeroPoseRecursive(
        FbxNode* lNode,
        FBXData& data)
{
    typedef typename FBXData::AnimationTransformation AnimationTransformation;

    //Handle animations
    std::unordered_map<FbxNode*, Index>::iterator it = data.skeletonMap.find(lNode);
    if (it != data.skeletonMap.end()) {
        Index jId = it->second;

        FbxAMatrix geometryOffset = nodeGeometryOffset(lNode);
        FbxAMatrix lAnimGlobalTransform = lNode->EvaluateGlobalTransform(0.0) * geometryOffset;
        AnimationTransformation animTransformation = transformationFromFBXMatrix<AnimationTransformation>(lAnimGlobalTransform);

        data.zeroPoseTransformations[jId] = animTransformation;
    }

    const int lChildCount = lNode->GetChildCount();
    for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex) {
        handleZeroPoseRecursive(lNode->GetChild(lChildIndex), data);
    }
}


template<class FBXData>
void createScene(
        FbxScene* lScene,
        const FBXData& data,
        const IOModelMode& mode)
{
    //Build the node tree.
    FbxNode* lRootNode = lScene->GetRootNode();
    std::vector<FbxNode*> jMap;
    FbxNode* lMesh = nullptr;
    FbxNode* lSkeleton = nullptr;

    if (mode.mesh) {
        lMesh = createMesh(lScene, data, mode);
        if (lMesh != nullptr) {
            lRootNode->AddChild(lMesh);
        }
    }

    if (mode.skeleton) {
        lSkeleton = createSkeleton(lScene, data, mode, jMap);
        if (lSkeleton != nullptr) {
            lRootNode->AddChild(lSkeleton);
        }
    }

    if (mode.skinningWeights && lMesh != nullptr && lSkeleton != nullptr) {
        FbxSkin* lSkin = createSkin(lScene, data, mode, jMap);
        if (lSkin != nullptr) {
            FbxGeometry* meshAttribute = (FbxGeometry*) lMesh->GetNodeAttribute();
            meshAttribute->AddDeformer(lSkin);
        }
    }

    if (mode.animations && lSkeleton != nullptr) {
        createAnimations(lScene, data, mode, jMap);
    }
}

template<class FBXData>
FbxNode* createMesh(
        FbxScene* lScene,
        const FBXData& data,
        const IOModelMode& mode)
{
    typedef typename FBXData::MeshData MeshData;
    typedef typename FBXData::Point Point;
    typedef typename FBXData::VertexNormal VertexNormal;
    typedef typename FBXData::VertexColor VertexColor;
    typedef typename FBXData::VertexUV VertexUV;
    typedef typename FBXData::VertexUV VertexUV;
    typedef typename FBXData::Material Material;

    static const char* uvSetName = "UVSet";
    const MeshData& meshData = data.meshData;

    FbxNode* lNode = FbxNode::Create(lScene, "Mesh");
    FbxMesh* lMesh = FbxMesh::Create(lScene,"Mesh");

    lNode->SetNodeAttribute(lMesh);
    lScene->AddNode(lNode);

    lMesh->InitControlPoints(meshData.vertices.size());

    bool normalsByVertex = meshData.vertexNormals.size() == meshData.vertices.size() && meshData.faceVertexNormals.empty();
    bool normalsByPolygon = !normalsByVertex && !meshData.vertexNormals.empty();
    bool uvsByVertex = meshData.vertexUVs.size() == meshData.vertices.size() && meshData.faceVertexUVs.empty();
    bool uvsByPolygon = !uvsByVertex && !meshData.vertexUVs.empty();
    bool materialByPolygon = !meshData.materials.empty() && !meshData.faceMaterials.empty();
    bool vertexColorsByVertex = meshData.vertexColors.size() == meshData.vertices.size();

    FbxGeometryElementNormal* lGeometryElementNormal = nullptr;
    FbxGeometryElementVertexColor* lGeometryElementVertexColor = nullptr;
    FbxGeometryElementUV* lGeometryElementUV = nullptr;
    FbxGeometryElementMaterial* lMaterialElement = nullptr;

    if (mode.meshMode.materials && materialByPolygon) {
        for (Index mId = 0; mId < meshData.materials.size(); ++mId) {
            const Material& m = meshData.materials[mId];
            FbxString lMaterialName(m.name().c_str());

            const Color& diffuseColor = m.diffuseColor();
            FbxDouble3 lDiffuseColor(diffuseColor.redF(), diffuseColor.greenF(), diffuseColor.blueF());

            const Color& ambientColor = m.ambientColor();
            FbxDouble3 lAmbientColor(ambientColor.redF(), ambientColor.greenF(), ambientColor.blueF());

            const Color& specularColor = m.specularColor();
            FbxDouble3 lSpecularColor(specularColor.redF(), specularColor.greenF(), specularColor.blueF());

            FbxSurfaceMaterial* lMaterial = FbxSurfacePhong::Create(lScene, lMaterialName.Buffer());

            FbxFileTexture* lDiffuseTexture = nullptr;
            if (!m.diffuseMap().empty()) {
                lDiffuseTexture = FbxFileTexture::Create(lScene,"Diffuse Texture");
                lDiffuseTexture->SetFileName(m.diffuseMap().c_str());
                lDiffuseTexture->SetTextureUse(FbxTexture::eStandard);
                lDiffuseTexture->SetMappingType(FbxTexture::eUV);
                lDiffuseTexture->SetMaterialUse(FbxFileTexture::eModelMaterial);
                lDiffuseTexture->SetSwapUV(false);
                lDiffuseTexture->SetTranslation(0.0, 0.0);
                lDiffuseTexture->SetScale(1.0, 1.0);
                lDiffuseTexture->SetRotation(0.0, 0.0);
                if (lGeometryElementUV) {
                    lDiffuseTexture->UVSet.Set(FbxString(uvSetName));
                }
            }

            FbxFileTexture* lAmbientTexture = nullptr;
            if (!m.ambientMap().empty()) {
                lAmbientTexture = FbxFileTexture::Create(lScene,"Ambient Texture");
                lAmbientTexture->SetFileName(m.ambientMap().c_str());
                lAmbientTexture->SetTextureUse(FbxTexture::eStandard);
                lAmbientTexture->SetMappingType(FbxTexture::eUV);
                lAmbientTexture->SetMaterialUse(FbxFileTexture::eModelMaterial);
                lAmbientTexture->SetSwapUV(false);
                lAmbientTexture->SetTranslation(0.0, 0.0);
                lAmbientTexture->SetScale(1.0, 1.0);
                lAmbientTexture->SetRotation(0.0, 0.0);
                if (lGeometryElementUV) {
                    lAmbientTexture->UVSet.Set(FbxString(uvSetName));
                }
            }

            FbxFileTexture* lSpecularTexture = nullptr;
            if (!m.specularMap().empty()) {
                lSpecularTexture = FbxFileTexture::Create(lScene,"Specular Texture");
                lSpecularTexture->SetFileName(m.specularMap().c_str());
                lSpecularTexture->SetTextureUse(FbxTexture::eStandard);
                lSpecularTexture->SetMappingType(FbxTexture::eUV);
                lSpecularTexture->SetMaterialUse(FbxFileTexture::eModelMaterial);
                lSpecularTexture->SetSwapUV(false);
                lSpecularTexture->SetTranslation(0.0, 0.0);
                lSpecularTexture->SetScale(1.0, 1.0);
                lSpecularTexture->SetRotation(0.0, 0.0);
                if (lGeometryElementUV) {
                    lSpecularTexture->UVSet.Set(FbxString(uvSetName));
                }
            }

            if (m.shadingModel() == Material::ShadingModel::SHADING_STANDARD || m.shadingModel() == Material::ShadingModel::SHADING_LAMBERT) {
                FbxSurfaceLambert* lLambertMaterial = FbxSurfaceLambert::Create(lScene, lMaterialName.Buffer());

                lLambertMaterial->Ambient.Set(lAmbientColor);
                lLambertMaterial->Diffuse.Set(lDiffuseColor);
                lLambertMaterial->TransparencyFactor.Set(0.0);
                lLambertMaterial->ShadingModel.Set("Lambert");

                if (lDiffuseTexture) {
                    lLambertMaterial->Diffuse.ConnectSrcObject(lDiffuseTexture);
                    lNode->SetShadingMode(FbxNode::eTextureShading);
                }
                if (lAmbientTexture) {
                    lLambertMaterial->Ambient.ConnectSrcObject(lAmbientTexture);
                    lNode->SetShadingMode(FbxNode::eTextureShading);
                }

                lMaterial = lLambertMaterial;
            }
            else if (m.shadingModel() == Material::ShadingModel::SHADING_PHONG) {
                FbxSurfacePhong* lPhongMaterial = FbxSurfacePhong::Create(lScene, lMaterialName.Buffer());

                lPhongMaterial->Specular.Set(lSpecularColor);
                lPhongMaterial->Ambient.Set(lAmbientColor);
                lPhongMaterial->Diffuse.Set(lDiffuseColor);
                lPhongMaterial->TransparencyFactor.Set(1.0 - m.transparency());
                lPhongMaterial->ShadingModel.Set("Phong");

                if (lDiffuseTexture) {
                    lPhongMaterial->Diffuse.ConnectSrcObject(lDiffuseTexture);
                    lNode->SetShadingMode(FbxNode::eTextureShading);
                }
                if (lAmbientTexture) {
                    lPhongMaterial->Ambient.ConnectSrcObject(lAmbientTexture);
                    lNode->SetShadingMode(FbxNode::eTextureShading);
                }
                if (lSpecularTexture) {
                    lPhongMaterial->Specular.ConnectSrcObject(lSpecularTexture);
                    lNode->SetShadingMode(FbxNode::eTextureShading);
                }

                lMaterial = lPhongMaterial;
            }

            //get the node of mesh, add material for it.
            lNode->AddMaterial(lMaterial);
        }
    }

    if (mode.meshMode.materials && materialByPolygon) {
        lMaterialElement = lMesh->CreateElementMaterial();
        lMaterialElement->SetMappingMode(FbxGeometryElement::eByPolygon);
        lMaterialElement->SetReferenceMode(FbxGeometryElement::eIndexToDirect);
    }
    if (mode.meshMode.vertexNormals && normalsByPolygon) {
        lGeometryElementNormal = lMesh->CreateElementNormal();
        lGeometryElementNormal->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
        lGeometryElementNormal->SetReferenceMode(FbxGeometryElement::eDirect);
    }
    if (mode.meshMode.vertexUVs && uvsByPolygon) {
        lGeometryElementUV = lMesh->CreateElementUV(FbxString(uvSetName));
        lGeometryElementUV->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
        lGeometryElementUV->SetReferenceMode(FbxGeometryElement::eDirect);
    }

    if (mode.meshMode.vertices) {
        if (mode.meshMode.vertexNormals && normalsByVertex) {
            lGeometryElementNormal = lMesh->CreateElementNormal();
            lGeometryElementNormal->SetMappingMode(FbxGeometryElement::eByControlPoint);
            lGeometryElementNormal->SetReferenceMode(FbxGeometryElement::eDirect);
        }
        if (mode.meshMode.vertexUVs && uvsByVertex) {
            lGeometryElementUV = lMesh->CreateElementUV(FbxString(uvSetName));
            lGeometryElementUV->SetMappingMode(FbxGeometryElement::eByControlPoint);
            lGeometryElementUV->SetReferenceMode(FbxGeometryElement::eDirect);
        }
        if (mode.meshMode.vertexColors && vertexColorsByVertex) {
            lGeometryElementVertexColor = lMesh->CreateElementVertexColor();
            lGeometryElementVertexColor->SetMappingMode(FbxGeometryElement::eByControlPoint);
            lGeometryElementVertexColor->SetReferenceMode(FbxGeometryElement::eDirect);
        }

        for (Index vId = 0; vId < meshData.vertices.size(); ++vId) {
            const Point& p = meshData.vertices[vId];
            lMesh->SetControlPointAt(FBXVector4FromVector(p), vId);
        }

        if (normalsByVertex && lGeometryElementNormal) {
            for (Index vId = 0; vId < meshData.vertices.size(); ++vId) {
                const VertexNormal& n = meshData.vertexNormals[vId];
                lGeometryElementNormal->GetDirectArray().Add(FBXVector4FromVector(n));
            }
        }

        if (uvsByVertex && lGeometryElementUV) {
            for (Index vId = 0; vId < meshData.vertices.size(); ++vId) {
                const VertexUV& uv = meshData.vertexUVs[vId];
                lGeometryElementUV->GetDirectArray().Add(FBXVector2FromVector(uv));
            }
        }

        if (vertexColorsByVertex && lGeometryElementVertexColor) {
            for (Index vId = 0; vId < meshData.vertices.size(); ++vId) {
                const VertexColor& c = meshData.vertexColors[vId];
                lGeometryElementVertexColor->GetDirectArray().Add(FbxColor(c.redF(), c.greenF(), c.blueF(), c.alphaF()));
            }
        }
    }

    if (mode.meshMode.faces) {
        int lIndexByPolygonVertex = 0;
        for (Index fId = 0; fId < meshData.faces.size(); ++fId) {

            if (lMaterialElement && materialByPolygon) {
                const Index& mId = meshData.faceMaterials[fId];
                if (mId != NULL_ID) {
                    lMesh->BeginPolygon(mId);
                }
                else {
                    lMesh->BeginPolygon(0);
                }
            }
            else {
                lMesh->BeginPolygon();
            }

            for (Index j = 0; j < meshData.faces[fId].size(); ++j) {
                lMesh->AddPolygon(meshData.faces[fId][j]);

                if (lGeometryElementNormal && normalsByPolygon) {
                    VertexNormal n(0,0,0);

                    if (!meshData.faceVertexNormals.empty() && j < meshData.faceVertexNormals[fId].size() && meshData.faceVertexNormals[fId][j] != NULL_ID) {
                        n = meshData.vertexNormals[meshData.faceVertexNormals[fId][j]];
                    }
                    else if (meshData.faces[fId][j] < meshData.vertexNormals.size()) {
                        n = meshData.vertexNormals[meshData.faces[fId][j]];
                    }

                    lGeometryElementNormal->GetDirectArray().Add(FBXVector4FromVector(n));
                }

                if (lGeometryElementUV && uvsByPolygon) {
                    VertexUV uv(0,0);

                    if (!meshData.faceVertexUVs.empty() && j < meshData.faceVertexUVs[fId].size() && meshData.faceVertexUVs[fId][j] != NULL_ID) {
                        uv = meshData.vertexUVs[meshData.faceVertexUVs[fId][j]];
                    }
                    else if (meshData.faces[fId][j] < meshData.vertexUVs.size()) {
                        uv = meshData.vertexUVs[meshData.faces[fId][j]];
                    }

                    lGeometryElementUV->GetDirectArray().Add(FBXVector2FromVector(uv));
                }

                lIndexByPolygonVertex++;
            }

            lMesh->EndPolygon();
        }
    }

    return lNode;
}

template<class FBXData>
FbxNode* createSkeleton(
        FbxScene* lScene,
        const FBXData& data,
        const IOModelMode& mode,
        std::vector<FbxNode*>& jMap)
{
    typedef typename FBXData::SkeletonData SkeletonData;
    typedef typename FBXData::SkeletonTransformation SkeletonTransformation;
    typedef typename FBXData::SkeletonScalar SkeletonScalar;

    NVL_SUPPRESS_UNUSEDVARIABLE(mode);

    std::vector<FbxNode*> rootNodes;

    const SkeletonData& skeletonData = data.skeletonData;

    std::string name = "Skeleton";

    jMap.resize(skeletonData.joints.size());

    for (Index i = 0; i < skeletonData.joints.size(); ++i) {
       FbxString nodeName(skeletonData.names[i].c_str());

       FbxSkeleton* lSkeletonAttribute = FbxSkeleton::Create(lScene, nodeName);
       if(skeletonData.parents[i] == -1) {
           lSkeletonAttribute->SetSkeletonType(FbxSkeleton::eRoot);
       }
       else {
           lSkeletonAttribute->SetSkeletonType(FbxSkeleton::eLimbNode);
       }

       lSkeletonAttribute->Size.Set(1.0);
       jMap[i] = FbxNode::Create(lScene, nodeName.Buffer());
       jMap[i]->SetNodeAttribute(lSkeletonAttribute);

       const SkeletonTransformation& t = skeletonData.joints[i];

       SkeletonTransformation localT = t;
       if(skeletonData.parents[i] != -1) {
           const SkeletonTransformation& parentT = skeletonData.joints[skeletonData.parents[i]];

           localT = parentT.inverse() * t;
       }

       typename SkeletonTransformation::LinearMatrixType scaMatrix, rotMatrix;
       localT.computeRotationScaling(&rotMatrix, &scaMatrix);

       Vector3<SkeletonScalar> scaVec(scaMatrix.diagonal());
       Scaling3<SkeletonScalar> sca(scaVec);
       Rotation3<SkeletonScalar> rot(rotMatrix);
       Translation3<SkeletonScalar> tra(localT.translation());

       if (!tra.isApprox(Translation3d::Identity())) {
           FbxVector4 fbxT = FBXTranslationFromTranslation(tra);
           jMap[i]->LclTranslation.Set(fbxT);
       }

       if (!rot.isApprox(Rotation3d::Identity())) {
           FbxVector4 fbxR = FBXRotationFromRotation(rot);
           jMap[i]->LclRotation.Set(fbxR);
       }

       if (!epsEqual(scaVec.x(), 0.0) || !epsEqual(scaVec.y(), 0.0) || !epsEqual(scaVec.z(), 0.0)) {
           FbxVector4 fbxS = FBXScalingFromScaling(sca);
           jMap[i]->LclScaling.Set(fbxS);
       }
    }

    for (Index i = 0; i < skeletonData.joints.size(); ++i) {
       if (skeletonData.parents[i] == -1) {
           rootNodes.push_back(jMap[i]);
       }
       else {
           jMap[skeletonData.parents[i]]->AddChild(jMap[i]);
       }
    }

    FbxNode* lRootNode = nullptr;
    if (rootNodes.size() == 1) {
        lRootNode = rootNodes[0];
    }
    else if (!rootNodes.empty()) {
        FbxNode* rNode = FbxNode::Create(lScene, "SkeletonRootNode");
        FbxSkeleton* rSkeleton = FbxSkeleton::Create(lScene, "SkeletonRootSkeleton");
        rNode->SetNodeAttribute(rSkeleton);
        for (FbxNode* child : rootNodes) {
            rNode->AddChild(child);
        }
        lRootNode = rNode;
    }

    if (lRootNode != nullptr) {
        lScene->AddNode(lRootNode);
    }

    return lRootNode;
}

template<class FBXData>
FbxSkin* createSkin(
        FbxScene* lScene,
        const FBXData& data,
        const IOModelMode& mode,
        const std::vector<FbxNode*>& jMap)
{
    typedef typename FBXData::SkinningWeightsScalar SkinningWeightsScalar;

    NVL_SUPPRESS_UNUSEDVARIABLE(mode);

    FbxSkin* lSkin = FbxSkin::Create(lScene, "");

    std::vector<FbxCluster*> clusters(data.skeletonData.joints.size(), nullptr);

    for (const std::tuple<Index, Index, typename FBXData::SkinningWeightsScalar>& tuple : data.skinningWeightsData.weights) {
        const Index& vId = std::get<0>(tuple);
        const Index& jId = std::get<1>(tuple);
        const SkinningWeightsScalar& weight = std::get<2>(tuple);

        FbxCluster *lCluster = clusters[jId];

        if (lCluster == nullptr) {
            lCluster = FbxCluster::Create(lScene,"");

            lCluster->SetLink(jMap[jId]);
            lCluster->SetLinkMode(FbxCluster::eTotalOne);
            lCluster->SetTransformLinkMatrix(FBXTransformationFromMatrix(data.skeletonData.joints[jId]));

            clusters[jId]  = lCluster;
        }

        if (!epsEqual(weight, 0.0)) {
            lCluster->AddControlPointIndex(vId, weight);
        }
    }

    for (FbxCluster* c : clusters) {
        if (c != nullptr)
            lSkin->AddCluster(c);
    }

    return lSkin;
}

template<class FBXData>
std::vector<FbxAnimStack*> createAnimations(
        FbxScene* lScene,
        const FBXData& data,
        const IOModelMode& mode,
        std::vector<FbxNode*>& jMap)
{
    typedef typename FBXData::AnimationData AnimationData;
    typedef typename FBXData::AnimationTransformation AnimationTransformation;
    typedef typename AnimationTransformation::LinearMatrixType LinearMatrixType;
    typedef typename AnimationTransformation::Scalar AnimationTransformationScalar;
    typedef typename FBXData::SkeletonData SkeletonData;
    typedef typename FBXData::SkeletonTransformation SkeletonTransformation;
    typedef typename FBXData::JointId JointId;    

    const SkeletonData& skeletonData = data.skeletonData;

    std::vector<FbxAnimStack*> stacks;


    //Pose stack

    FbxAnimStack* lPoseStack = FbxAnimStack::Create(lScene, "Pose");

    FbxAnimLayer* lPoseLayer = FbxAnimLayer::Create(lScene, "Pose_layer");    
    lPoseStack->AddMember(lPoseLayer);

    FbxTakeInfo* poseTakeInfo = new FbxTakeInfo();
    poseTakeInfo->mName = "Pose_takeinfo";
    FbxTime start, stop;
    start.SetSecondDouble(0);
    stop.SetSecondDouble(0);
    poseTakeInfo->mSelect = true;
    poseTakeInfo->mLocalTimeSpan = FbxTimeSpan(start, stop);
    poseTakeInfo->mReferenceTimeSpan = FbxTimeSpan(start, stop);
    poseTakeInfo->mCurrentLayer = 0;
    lPoseStack->Reset(poseTakeInfo);

    stacks.push_back(lPoseStack);

    for (JointId jId = 0; jId < skeletonData.joints.size(); ++jId) {
        JointId parentId = NULL_ID;
        if (skeletonData.parents[jId] != -1) {
            parentId = skeletonData.parents[jId];
        }

        FbxNode* jNode = jMap[jId];

        FbxAnimCurve* lCurveRX = jNode->LclRotation.GetCurve(lPoseLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
        FbxAnimCurve* lCurveRY = jNode->LclRotation.GetCurve(lPoseLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
        FbxAnimCurve* lCurveRZ = jNode->LclRotation.GetCurve(lPoseLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);
        FbxAnimCurve* lCurveTX = jNode->LclTranslation.GetCurve(lPoseLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
        FbxAnimCurve* lCurveTY = jNode->LclTranslation.GetCurve(lPoseLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
        FbxAnimCurve* lCurveTZ = jNode->LclTranslation.GetCurve(lPoseLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);
        FbxAnimCurve* lCurveSX = jNode->LclScaling.GetCurve(lPoseLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
        FbxAnimCurve* lCurveSY = jNode->LclScaling.GetCurve(lPoseLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
        FbxAnimCurve* lCurveSZ = jNode->LclScaling.GetCurve(lPoseLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);

        lCurveRX->KeyModifyBegin();
        lCurveRY->KeyModifyBegin();
        lCurveRZ->KeyModifyBegin();
        lCurveTX->KeyModifyBegin();
        lCurveTY->KeyModifyBegin();
        lCurveTZ->KeyModifyBegin();
        lCurveSX->KeyModifyBegin();
        lCurveSY->KeyModifyBegin();
        lCurveSZ->KeyModifyBegin();

        AnimationTransformation transformation = skeletonData.joints[jId];
        if (parentId != NULL_ID)
            transformation = skeletonData.joints[parentId].inverse() * transformation;

        const double& time = 0;

        LinearMatrixType rotMatrix, scalMatrix;
        transformation.computeRotationScaling(&rotMatrix, &scalMatrix);
        Vector3<AnimationTransformationScalar> traVec(transformation.translation());
        Rotation3<AnimationTransformationScalar> rot(rotMatrix);
        Vector3<AnimationTransformationScalar> scaVec(scalMatrix.diagonal());
        Vector3<AnimationTransformationScalar> ang = eulerAnglesXYZFromRotation(rot);

        constexpr double eps = 1e-5;

        for (EigenId i = 0; i < ang.size(); ++i) {
            ang[i] = ang[i] / M_PI * 180.0;

            if (epsEqual(ang[i], 0.0, eps)) {
                ang[i] = 0.0;
            }
        }

        for (EigenId i = 0; i < traVec.size(); ++i) {
            if (epsEqual(traVec[i], 0.0, eps)) {
                traVec[i] = 0.0;
            }
        }

        for (EigenId i = 0; i < scaVec.size(); ++i) {
            if (epsEqual(scaVec[i], 1.0, eps)) {
                scaVec[i] = 1.0;
            }
        }


        FbxTime lTime;
        lTime.SetSecondDouble(time);

        int lKeyIndex;

        lKeyIndex = lCurveRX->KeyAdd(lTime);
        lCurveRX->KeySetValue(lKeyIndex, ang.x());
        lCurveRX->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

        lKeyIndex = lCurveRY->KeyAdd(lTime);
        lCurveRY->KeySetValue(lKeyIndex, ang.y());
        lCurveRY->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

        lKeyIndex = lCurveRZ->KeyAdd(lTime);
        lCurveRZ->KeySetValue(lKeyIndex, ang.z());
        lCurveRZ->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

        lKeyIndex = lCurveTX->KeyAdd(lTime);
        lCurveTX->KeySetValue(lKeyIndex, traVec.x());
        lCurveTX->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

        lKeyIndex = lCurveTY->KeyAdd(lTime);
        lCurveTY->KeySetValue(lKeyIndex, traVec.y());
        lCurveTY->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

        lKeyIndex = lCurveTZ->KeyAdd(lTime);
        lCurveTZ->KeySetValue(lKeyIndex, traVec.z());
        lCurveTZ->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

        lKeyIndex = lCurveSX->KeyAdd(lTime);
        lCurveSX->KeySetValue(lKeyIndex, scaVec.x());
        lCurveSX->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

        lKeyIndex = lCurveSY->KeyAdd(lTime);
        lCurveSY->KeySetValue(lKeyIndex, scaVec.y());
        lCurveSY->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

        lKeyIndex = lCurveSZ->KeyAdd(lTime);
        lCurveSZ->KeySetValue(lKeyIndex, scaVec.z());
        lCurveSZ->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);


        lCurveRX->KeyModifyEnd();
        lCurveRY->KeyModifyEnd();
        lCurveRZ->KeyModifyEnd();
        lCurveTX->KeyModifyEnd();
        lCurveTY->KeyModifyEnd();
        lCurveTZ->KeyModifyEnd();
        lCurveSX->KeyModifyEnd();
        lCurveSY->KeyModifyEnd();
        lCurveSZ->KeyModifyEnd();
    }

    lScene->SetCurrentAnimationStack(lPoseStack);



    //Animation stacks
    for (AnimationData animationData : data.animationDataVector) {
        FbxAnimStack* lAnimStack = FbxAnimStack::Create(lScene, animationData.name.c_str());

        FbxAnimLayer* lAnimLayer = FbxAnimLayer::Create(lScene, (animationData.name + "_layer").c_str());
        lAnimStack->AddMember(lAnimLayer);

        FbxTakeInfo* takeInfo = new FbxTakeInfo();
        takeInfo->mName = (animationData.name + "_takeinfo").c_str();
        FbxTime start, stop;
        start.SetSecondDouble(animationData.times[0]);
        stop.SetSecondDouble(animationData.times[animationData.times.size() - 1]);
        takeInfo->mSelect = true;
        takeInfo->mLocalTimeSpan = FbxTimeSpan(start, stop);
        takeInfo->mReferenceTimeSpan = FbxTimeSpan(start, stop);
        takeInfo->mCurrentLayer = 0;

        lAnimStack->Reset(takeInfo);

        for (JointId jId = 0; jId < skeletonData.joints.size(); ++jId) {
            JointId parentId = NULL_ID;
            if (skeletonData.parents[jId] != -1) {
                parentId = skeletonData.parents[jId];
            }

            FbxNode* jNode = jMap[jId];

            FbxAnimCurve* lCurveRX = jNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
            FbxAnimCurve* lCurveRY = jNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
            FbxAnimCurve* lCurveRZ = jNode->LclRotation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);
            FbxAnimCurve* lCurveTX = jNode->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
            FbxAnimCurve* lCurveTY = jNode->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
            FbxAnimCurve* lCurveTZ = jNode->LclTranslation.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);
            FbxAnimCurve* lCurveSX = jNode->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
            FbxAnimCurve* lCurveSY = jNode->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
            FbxAnimCurve* lCurveSZ = jNode->LclScaling.GetCurve(lAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);

            lCurveRX->KeyModifyBegin();
            lCurveRY->KeyModifyBegin();
            lCurveRZ->KeyModifyBegin();
            lCurveTX->KeyModifyBegin();
            lCurveTY->KeyModifyBegin();
            lCurveTZ->KeyModifyBegin();
            lCurveSX->KeyModifyBegin();
            lCurveSY->KeyModifyBegin();
            lCurveSZ->KeyModifyBegin();

            for (Index kId = 0; kId < animationData.transformations.size(); ++kId) {
                AnimationTransformation transformation = animationData.transformations[kId][jId];
                if (parentId != NULL_ID)
                    transformation = animationData.transformations[kId][parentId].inverse() * transformation;

                const double& time = animationData.times[kId];

                LinearMatrixType rotMatrix, scalMatrix;
                transformation.computeRotationScaling(&rotMatrix, &scalMatrix);
                Vector3<AnimationTransformationScalar> traVec(transformation.translation());
                Rotation3<AnimationTransformationScalar> rot(rotMatrix);
                Vector3<AnimationTransformationScalar> scaVec(scalMatrix.diagonal());
                Vector3<AnimationTransformationScalar> ang = eulerAnglesXYZFromRotation(rot);

                constexpr double eps = 1e-5;

                for (EigenId i = 0; i < ang.size(); ++i) {
                    ang[i] = ang[i] / M_PI * 180.0;

                    if (epsEqual(ang[i], 0.0, eps)) {
                        ang[i] = 0.0;
                    }
                }

                for (EigenId i = 0; i < traVec.size(); ++i) {
                    if (epsEqual(traVec[i], 0.0, eps)) {
                        traVec[i] = 0.0;
                    }
                }

                for (EigenId i = 0; i < scaVec.size(); ++i) {
                    if (epsEqual(scaVec[i], 1.0, eps)) {
                        scaVec[i] = 1.0;
                    }
                }


                FbxTime lTime;
                lTime.SetSecondDouble(time);

                int lKeyIndex;

                lKeyIndex = lCurveRX->KeyAdd(lTime);
                lCurveRX->KeySetValue(lKeyIndex, ang.x());
                lCurveRX->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

                lKeyIndex = lCurveRY->KeyAdd(lTime);
                lCurveRY->KeySetValue(lKeyIndex, ang.y());
                lCurveRY->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

                lKeyIndex = lCurveRZ->KeyAdd(lTime);
                lCurveRZ->KeySetValue(lKeyIndex, ang.z());
                lCurveRZ->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

                lKeyIndex = lCurveTX->KeyAdd(lTime);
                lCurveTX->KeySetValue(lKeyIndex, traVec.x());
                lCurveTX->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

                lKeyIndex = lCurveTY->KeyAdd(lTime);
                lCurveTY->KeySetValue(lKeyIndex, traVec.y());
                lCurveTY->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

                lKeyIndex = lCurveTZ->KeyAdd(lTime);
                lCurveTZ->KeySetValue(lKeyIndex, traVec.z());
                lCurveTZ->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

                lKeyIndex = lCurveSX->KeyAdd(lTime);
                lCurveSX->KeySetValue(lKeyIndex, scaVec.x());
                lCurveSX->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

                lKeyIndex = lCurveSY->KeyAdd(lTime);
                lCurveSY->KeySetValue(lKeyIndex, scaVec.y());
                lCurveSY->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

                lKeyIndex = lCurveSZ->KeyAdd(lTime);
                lCurveSZ->KeySetValue(lKeyIndex, scaVec.z());
                lCurveSZ->KeySetInterpolation(lKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
            }

            lCurveRX->KeyModifyEnd();
            lCurveRY->KeyModifyEnd();
            lCurveRZ->KeyModifyEnd();
            lCurveTX->KeyModifyEnd();
            lCurveTY->KeyModifyEnd();
            lCurveTZ->KeyModifyEnd();
            lCurveSX->KeyModifyEnd();
            lCurveSY->KeyModifyEnd();
            lCurveSZ->KeyModifyEnd();
        }

        stacks.push_back(lAnimStack);
    }

    return stacks;
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
        lGlobalPosition = lNode->EvaluateGlobalTransform();
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
    FbxVector4 fbxTranslation = fbxT.GetT();
    FbxVector4 fbxRotation = fbxT.GetR();
    FbxVector4 fbxScaling = fbxT.GetS();

    Rotation3d rot = rotationFromFBXRotation(fbxRotation);
    Translation3d tra = translationFromFBXTranslation(fbxTranslation);
    Scaling3d sca = scalingFromFBXScaling(fbxScaling);

    T t(tra * rot * sca);

    return t;
}

inline Translation3d translationFromFBXTranslation(const FbxVector4& fbxT)
{
    Vector3d traVec(fbxT[0], fbxT[1], fbxT[2]);
    Translation3d tra(traVec);

    return tra;
}

inline Scaling3d scalingFromFBXScaling(const FbxVector4& fbxS)
{
    Vector3d scaVec(fbxS[0], fbxS[1], fbxS[2]);
    Scaling3d sca(scaVec);

    return sca;
}


inline Rotation3d rotationFromFBXRotation(const FbxVector4& fbxR)
{
    constexpr double degreesToRad = M_PI / 180.0;

    Vector3d angleVec(fbxR[0] * degreesToRad, fbxR[1] * degreesToRad, fbxR[2] * degreesToRad);
    Rotation3d rot = eulerAnglesToRotationXYZ(angleVec);

    return rot;
}

template<class T>
FbxAMatrix FBXTransformationFromMatrix(const T& t)
{
    FbxAMatrix matrix;

    typename T::LinearMatrixType scaMatrix, rotMatrix;
    t.computeRotationScaling(&rotMatrix, &scaMatrix);

    Scaling3d sca(scaMatrix.diagonal());
    Rotation3d rot(rotMatrix);
    Translation3d tra(t.translation());

    FbxVector4 fbxT = FBXTranslationFromTranslation(tra);
    FbxVector4 fbxR = FBXRotationFromRotation(rot);
    FbxVector4 fbxS = FBXScalingFromScaling(sca);

    matrix.SetTRS(fbxT, fbxR, fbxS);

    return matrix;
}

inline FbxVector4 FBXTranslationFromTranslation(const Translation3d& tra)
{
    const Vector3d& traVec = tra.vector();
    FbxVector4 fbxT(traVec.x(), traVec.y(), traVec.z(), 0.0);

    return fbxT;
}

inline FbxVector4 FBXScalingFromScaling(const Scaling3d& sca)
{
    const Vector3d& scaVec = sca.diagonal();
    FbxVector4 fbxS(scaVec.x(), scaVec.y(), scaVec.z(), 0.0);

    return fbxS;
}

inline FbxVector4 FBXRotationFromRotation(const Rotation3d& rot)
{
    constexpr double radToDegrees = 180.0 / M_PI;

    Vector3d angleVec = eulerAnglesXYZFromRotation(rot);
    FbxVector4 fbxR(angleVec[0] * radToDegrees, angleVec[1] * radToDegrees, angleVec[2] * radToDegrees, 0.0);

    return fbxR;
}

template<class V>
FbxVector4 FBXVector4FromVector(const V& v)
{
    FbxVector4 fbxV(v.x(), v.y(), v.z(), 0.0);

    return fbxV;
}

template<class V>
FbxVector2 FBXVector2FromVector(const V& v)
{
    FbxVector2 fbxV(v.x(), v.y());

    return fbxV;
}

}

}

#endif

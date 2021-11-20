#include "model_io_fbx.h"

#ifdef NVL_FBXSDK_LOADED

#include <fbxsdk.h>

#include <iostream> //TODO CANCEL

namespace nvl {

template<class Model>
bool modelLoadModelsFromFBX(
        const std::string& filename,
        std::vector<Model>& models,
        IOModelError& error,
        IOModelMode& mode)
{

    typedef typename Model::Mesh Mesh;
    typedef typename Model::Skeleton Skeleton;
    typedef typename Model::SkinningWeights SkinningWeights;
    typedef typename Model::Animation Animation;

    typedef IOMeshData<typename Mesh::Point,
            typename Mesh::VertexNormal,
            typename Mesh::VertexUV,
            typename Mesh::VertexColor,
            typename Mesh::PolylineColor,
            typename Mesh::FaceNormal,
            typename Mesh::Material> MeshData;
    typedef IOSkinningWeightsData<typename SkinningWeights::Scalar> SkinningWeightsData;
    typedef IOSkeletonData<typename Skeleton::Transformation> SkeletonData;
    typedef IOAnimationData<typename Animation::Transformation> AnimationData;

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
    FbxScene* lScene = FbxScene::Create(lSdkManager,"myScene");

    //Import the contents of the file into the scene.
    lImporter->Import(lScene);

    //The file is imported, so get rid of the importer.
    lImporter->Destroy();
    FbxNode* lRootNode = nullptr;

    lRootNode = lScene->GetRootNode();
    if (lRootNode) {
        for(int i = 0; i < lRootNode->GetChildCount(); i++) {
            FbxNode* lNode = lRootNode->GetChild(i);
            std::cout << lNode->GetName() << " - " << lNode->GetTypeName() << std::endl;
        }
    }

    return false;

    MeshData meshData;
    SkeletonData skeletonData;
    SkinningWeightsData skinningWeightsData;
    std::vector<AnimationData> animationDatas;

    //TODO

    Model model;

    if (mode.mesh) {
        meshLoadVertexData(model.mesh, meshData.vertices, meshData.vertexNormals, meshData.vertexUVs, meshData.vertexColors);
        meshLoadPolylineData(model.mesh, meshData.polylines, meshData.polylineColors);
        meshLoadFaceData(model.mesh, meshData.faces, meshData.faceNormals, meshData.materials, meshData.faceMaterials, meshData.vertexNormals, meshData.vertexUVs, meshData.faceVertexNormals, meshData.faceVertexUVs);
    }

    if (mode.skeleton) {
        skeletonLoadData(model.skeleton, skeletonData.joints, skeletonData.parents, skeletonData.names);
    }

    if (mode.skinningWeights) {
        skinningWeightsLoadData(model.skinningWeights, skinningWeightsData.weights);
    }

    if (mode.animations) {
        for (const AnimationData& animationData : animationDatas) {
            Animation animation;
            animationLoadData(animation, animationData.name, animationData.times, animationData.transformations);
            model.addAnimation(animation);
        }
    }

    models.push_back(model);

    return true;
}

}

#endif


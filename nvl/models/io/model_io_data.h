/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MODEL_IO_DATA_H
#define NVL_MODELS_MODEL_IO_DATA_H

#include <nvl/nuvolib.h>

#include <string>
#include <vector>
#include <utility>

#include <nvl/models/io/mesh_io_data.h>
#include <nvl/models/io/skeleton_io_data.h>
#include <nvl/models/io/skinning_weights_io_data.h>
#include <nvl/models/io/animation_io_data.h>

namespace nvl {

//Error messages
enum IOModelError { IO_MODEL_SUCCESS, IO_MODEL_FILE_ERROR, IO_MODEL_FORMAT_NON_RECOGNISED, IO_MODEL_EXTENSION_NON_SUPPORTED,
                    IO_MODEL_MESH_ERROR, IO_MODEL_SKELETON_ERROR, IO_MODEL_SKINNINGWEIGHTS_ERROR, IO_MODEL_ANIMATION_ERROR };

//FBX pose mode
enum IOModelFBXPose { IO_FBX_POSE_NONE, IO_FBX_POSE_DEFAULT, IO_FBX_POSE_BIND, IO_FBX_POSE_ZERO };

//IO mode
struct IOModelMode {
    IOModelMode();

    bool mesh;
    bool skeleton;
    bool skinningWeights;
    bool animations;

    IOMeshMode meshMode;
    IOSkeletonMode skeletonMode;
    IOSkinningWeightsMode skinningWeightsMode;
    IOAnimationMode animationMode;

    IOModelFBXPose FBXDeformToPose;
    bool FBXSavePoses;

    bool RIGComponentFolder;
};

template<class M, class S, class W, class A>
struct IOModelData {
    IOModelData();

    std::string name;

    M mesh;
    S skeleton;
    W skinningWeights;
    std::vector<A> animations;

    void clear();
};

namespace internal {

//Dummy variables for default arguments
static IOModelError dummyIOModelError;
static IOModelMode dummyIOModelMode;

}

}

#include "model_io_data.cpp"

#endif // NVL_MODELS_MODEL_IO_DATA_H

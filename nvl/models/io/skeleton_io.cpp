/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "skeleton_io.h"

#include <nvl/models/io/skeleton_io_skt.h>

#include <nvl/utilities/file_utils.h>

#include <nvl/utilities/string_utils.h>

namespace nvl {

template<class S>
bool skeletonLoadFromFile(
        const std::string& filename,
        S& skeleton,
        IOSkeletonError& error,
        const IOSkeletonMode& mode)
{
    IOSkeletonData<typename S::Transformation> skeletonData;

    std::string ext = stringToLower(filenameExtension(filename));

    skeleton.clear();

    bool success;
    if (ext == "skt") {
        success = skeletonLoadDataFromSKT(filename, skeletonData, error);
    }
    else {
        error = IO_SKELETON_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    if (success) {
        skeletonLoadData(skeleton, skeletonData, mode);
    }

    return success;
}

template<class S>
bool skeletonSaveToFile(
        const std::string& filename,
        S& skeleton,
        IOSkeletonError& error,
        const IOSkeletonMode& mode)
{
    std::string ext = stringToLower(filenameExtension(filename));

    bool success;
    if (ext == "skt") {
        IOSkeletonData<typename S::Transformation> skeletonData;

        skeletonSaveData(skeleton, skeletonData, mode);

        success = skeletonSaveDataToSKT(filename, skeletonData, error);
    }
    else {
        error = IO_SKELETON_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    return success;
}


template<class S, class SD>
void skeletonLoadData(
        S& skeleton,
        const SD& skeletonData,
        const IOSkeletonMode& mode)
{
    typedef typename S::JointId JointId;
    typedef typename S::Transformation Transformation;

    NVL_SUPPRESS_UNUSEDVARIABLE(mode);

    const std::vector<Transformation>& joints = skeletonData.joints;
    const std::vector<int>& parents = skeletonData.parents;
    const std::vector<std::string>& names = skeletonData.names;
    const std::vector<bool>& hidden = skeletonData.hidden;

    skeleton.clear();
    for (Index i = 0; i < joints.size(); ++i) {
        JointId parentId = NULL_ID;
        if (parents[i] >= 0)
            parentId = static_cast<JointId>(parents[i]);

        std::vector<JointId> children;
        for (Index j = 0; j < joints.size(); ++j) {
            if (parents[j] >= 0 && parents[j] == static_cast<int>(i)) {
                children.push_back(static_cast<JointId>(j));
            }
        }

        JointId jId = skeleton.addJoint(joints[i], parentId, children, names[i]);
        assert(jId == static_cast<JointId>(i));

        skeleton.setJointHidden(jId, hidden[i]);
    }
}

template<class S, class SD>
void skeletonSaveData(
        const S& skeleton,
        SD& skeletonData,
        const IOSkeletonMode& mode)
{
    typedef typename S::JointId JointId;
    typedef typename S::Joint Joint;
    typedef typename S::Transformation Transformation;

    NVL_SUPPRESS_UNUSEDVARIABLE(mode);

    std::vector<Transformation>& joints = skeletonData.joints;
    std::vector<bool>& hidden = skeletonData.hidden;
    std::vector<int>& parents = skeletonData.parents;
    std::vector<std::string>& names = skeletonData.names;

    joints.resize(skeleton.jointNumber());
    hidden.resize(skeleton.jointNumber());
    parents.resize(skeleton.jointNumber());
    names.resize(skeleton.jointNumber());

    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        const Joint& joint = skeleton.joint(jId);
        joints[jId] = Transformation(joint.bindPose());
        if (skeleton.isRoot(jId)) {
            parents[jId] = -1;
        }
        else {
            parents[jId] = skeleton.parentId(jId);
        }
        names[jId] = joint.name();
        hidden[jId] = joint.isHidden();
    }
}

}

#include "skeleton_io.h"

#include <nvl/models/io/skeleton_io_skt.h>

#include <nvl/utilities/file_utils.h>

namespace nvl {

template<class S>
bool skeletonLoadFromFile(
        const std::string& filename,
        S& skeleton,
        IOSkeletonError& error,
        const IOSkeletonMode& mode)
{
    IOSkeletonData<typename S::Transformation> skeletonData;

    std::string ext = filenameExtension(filename);

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
    std::string ext = filenameExtension(filename);

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

    const std::vector<Transformation>& joints = skeletonData.joints;
    const std::vector<int>& parents = skeletonData.parents;
    const std::vector<std::string>& names = skeletonData.names;

    std::vector<JointId> idMap(joints.size(), NULL_ID);

    for (Index i = 0; i < joints.size(); ++i) {
        if (parents[i] == -1) {
            idMap[i] = skeleton.addRoot(joints[i], names[i]);
        }
        else {
            assert(idMap[parents[i]] != NULL_ID);
            idMap[i] = skeleton.addChild(idMap[parents[i]], joints[i], names[i]);
        }
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

    std::vector<Transformation>& joints = skeletonData.joints;
    std::vector<int>& parents = skeletonData.parents;
    std::vector<std::string>& names = skeletonData.names;

    joints.resize(skeleton.jointNumber());
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
    }
}

}

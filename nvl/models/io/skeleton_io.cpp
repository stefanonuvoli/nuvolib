#include "skeleton_io.h"

#include <nvl/models/io/skeleton_io_skt.h>

#include <nvl/utilities/file_utils.h>

namespace nvl {

template<class S>
bool skeletonLoadFromFile(
        const std::string& filename,
        S& skeleton,
        IOSkeletonError& error,
        IOSkeletonMode& mode)
{
    IOSkeletonData<typename S::Transformation> data;

    std::string ext = filenameExtension(filename);

    skeleton.clear();

    bool success;
    if (ext == "skt") {
        success = skeletonLoadDataFromSkt(filename, data, error, mode);
    }
    else {
        error = IO_SKELETON_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    if (success) {
        skeletonLoadData(skeleton, data.joints, data.parents, data.names);
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
        IOSkeletonData<typename S::Transformation> data;
        skeletonSaveData(skeleton, data.joints, data.parents, data.names);

        success = skeletonSaveDataToSkt(filename, data, error, mode);
    }
    else {
        error = IO_SKELETON_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    return success;
}

template<class V, class T>
void skeletonLoadData(
        Skeleton<V>& skeleton,
        const std::vector<T>& joints,
        const std::vector<int>& parents,
        const std::vector<std::string>& names)
{
    std::vector<typename Skeleton<V>::JointId> idMap(joints.size(), NULL_ID);

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

template<class V, class T>
void skeletonSaveData(
        const Skeleton<V>& skeleton,
        std::vector<T>& joints,
        std::vector<int>& parents,
        std::vector<std::string>& names)
{
    typedef typename Skeleton<V>::Joint Joint;
    typedef typename Skeleton<V>::JointId JointId;

    joints.resize(skeleton.jointNumber());
    parents.resize(skeleton.jointNumber());
    names.resize(skeleton.jointNumber());

    for (JointId jId = 0; jId < skeleton.jointNumber(); ++jId) {
        const Joint& joint = skeleton.joint(jId);
        joints[jId] = T(joint.restPose());
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

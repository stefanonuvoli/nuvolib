#include "animation_io.h"

#include <nvl/models/animation_io_ska.h>

namespace nvl {

template<class A>
bool animationLoadFromFile(
        const std::string& filename,
        A& animation,
        IOAnimationError& error,
        IOAnimationMode& mode)
{
    IOAnimationData<typename A::Transformation> data;

    std::string ext = filenameExtension(filename);

    bool success;
    if (ext == "ska") {
        success = animationLoadDataFromSka(filename, data, error, mode);
    }
    else {
        error = IO_ANIMATION_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    if (success) {
        animationLoadData(animation, data.name, data.times, data.transformations);
    }

    return success;
}

template<class A>
bool animationSaveToFile(
        const std::string& filename,
        const A& animation,
        IOAnimationError& error,
        const IOAnimationMode& mode)
{
    std::string ext = filenameExtension(filename);

    bool success;
    if (ext == "ska") {
        IOAnimationData<typename A::Transformation> data;
        animationSaveData(animation, data.name, data.times, data.transformations);

        success = animationSaveDataToSka(filename, data, error, mode);
    }
    else {
        error = IO_ANIMATION_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    return success;
}

template<class T>
void animationLoadData(
        Animation<T>& animation,
        const std::string& name,
        const std::vector<double>& times,
        const std::vector<std::vector<T>>& transformations)
{
    animation.setName(name);

    for (Index i = 0; i < times.size(); ++i) {
        animation.addKeyframe(times[i], transformations[i]);
    }
}

template<class T>
void animationSaveData(
        const Animation<T>& animation,
        std::string& name,
        std::vector<double>& times,
        std::vector<std::vector<T>>& transformations)
{
    typedef typename Animation<T>::Frame Frame;

    name = animation.name();

    times.resize(animation.keyframeNumber());
    transformations.resize(animation.keyframeNumber());
    for (Index i = 0; i < animation.keyframeNumber(); ++i) {
        const Frame& keyframe = animation.keyframe(i);
        times[i] = keyframe.time();
        transformations[i] = keyframe.transformations();
    }
}


}

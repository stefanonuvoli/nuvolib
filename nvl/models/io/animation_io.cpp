#include "animation_io.h"

#include <nvl/models/io/animation_io_ska.h>

namespace nvl {

template<class A>
bool animationLoadFromFile(
        const std::string& filename,
        A& animation,
        IOAnimationError& error,
        const IOAnimationMode& mode)
{
    IOAnimationData<typename A::Transformation> animationData;

    std::string ext = filenameExtension(filename);

    bool success;
    if (ext == "ska") {
        success = animationLoadDataFromSKA(filename, animationData, error);
    }
    else {
        error = IO_ANIMATION_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    if (success) {
        animationLoadData(animation, animationData, mode);
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
        IOAnimationData<typename A::Transformation> animationData;

        animationSaveData(animation, animationData, mode);

        success = animationSaveDataToSKA(filename, animationData, error);
    }
    else {
        error = IO_ANIMATION_EXTENSION_NON_SUPPORTED;
        success = false;
    }

    return success;
}

template<class A, class AD>
void animationLoadData(
        A& animation,
        const AD& animationData,
        const IOAnimationMode& mode)
{
    typedef typename A::Transformation Transformation;

    const std::string& name = animationData.name;
    const std::vector<double>& times = animationData.times;
    const std::vector<std::vector<Transformation>>& transformations = animationData.transformations;

    animation.setName(name);
    for (Index i = 0; i < times.size(); ++i) {
        animation.addKeyframe(times[i], transformations[i]);
    }
}

template<class A, class AD>
void animationSaveData(
        const A& animation,
        AD& animationData,
        const IOAnimationMode& mode)
{
    typedef typename A::Frame Frame;
    typedef typename A::Transformation Transformation;

    std::string& name = animationData.name;
    std::vector<double>& times = animationData.times;
    std::vector<std::vector<Transformation>>& transformations = animationData.transformations;

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

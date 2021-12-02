#include "animation_blend.h"

#include <nvl/math/interpolation.h>

namespace nvl {

template<class F>
void animationBlendFrameTransformations(
        std::vector<F>& animationFrames,
        const double& fps,
        const double& speed,
        const bool& keepKeyframes)
{
    typedef F Frame;
    typedef typename Frame::Transformation Transformation;

    assert(speed > 0.0);

    if (animationFrames.empty())
        return;

    const double timeStep = 1.0 / (fps / speed);

    //Blending
    std::vector<Frame> blendedAnimationFrames;
    blendedAnimationFrames.reserve(animationFrames.size());

    //Add first frame
    blendedAnimationFrames.push_back(animationFrames[0]);

    Index currentFrameId = 0;
    double currentTime = timeStep;
    while (currentFrameId < animationFrames.size() - 1) {
        const Frame& frame1 = animationFrames[currentFrameId];
        const double& time1 = frame1.time();
        const Frame& frame2 = animationFrames[currentFrameId+1];
        const double& time2 = frame2.time();

        assert(time1 < time2);
        while (time1 > currentTime) {
            currentTime += timeStep;
        }

        if (time2 <= currentTime) {
            currentFrameId++;
            if (keepKeyframes && currentFrameId < animationFrames.size() - 1) {
                blendedAnimationFrames.push_back(animationFrames[currentFrameId]);

                if (epsEqual(time2, currentTime, 0.01)) {
                    currentTime += timeStep;
                }
            }
        }
        else {
            const std::vector<Transformation>& transformations1 = frame1.transformations();
            const std::vector<Transformation>& transformations2 = frame2.transformations();
            const double alpha = (currentTime - time1) / (time2 - time1);

            Frame blendedFrame;
            blendedFrame.setTime(currentTime);

            std::vector<Transformation>& transformations = blendedFrame.transformations();
            transformations.resize(transformations1.size());
            for (Index jId = 0; jId < transformations1.size(); ++jId) {
                transformations[jId] = interpolateAffine(transformations1[jId], transformations2[jId], alpha);
            }

            blendedAnimationFrames.push_back(blendedFrame);

            currentTime += timeStep;
        }
    }

    for (Index i = 0; i < blendedAnimationFrames.size() - 1; ++i) {
        assert(blendedAnimationFrames[i].time() < blendedAnimationFrames[i+1].time());
    }

    //Add last frame
    blendedAnimationFrames.push_back(animationFrames[animationFrames.size() - 1]);

    animationFrames = blendedAnimationFrames;
}

}
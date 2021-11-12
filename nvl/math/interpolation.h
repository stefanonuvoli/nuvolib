#ifndef NVL_MATH_INTERPOLATION_H
#define NVL_MATH_INTERPOLATION_H

#include <nvl/nuvolib.h>

#include <nvl/math/transformations.h>
#include <nvl/math/vector.h>

#include <vector>

namespace nvl {

template<class T, class W>
T interpolateLinear(const T& v1, const T& v2, const W& a);
template<class T, class W>
T interpolateLinear(const std::vector<T>& v, const std::vector<W>& weights);

template<class T, class W>
Affine3<T> interpolateAffine(const Affine3<T>& t1, const Affine3<T>& t2, const W& a);
template<class T, class W>
Affine3<T> interpolateAffine(const std::vector<Affine3<T>>& t, const std::vector<W>& weights);

template<class T, EigenId D, class W>
Vector<T, D> interpolateVectorLinear(const Vector<T, D>& v1, const Vector<T, D>& v2, const W& a);
template<class T, EigenId D, class W>
Vector<T, D> interpolateVectorLinear(const std::vector<Vector<T, D>>& v, const std::vector<W>& weights);

template<class T, EigenId D, class W>
Translation<T, D> interpolateTranslationLinear(const Translation<T, D>& t1, const Translation<T, D>& t2, const W& a);
template<class T, EigenId D, class W>
Translation<T, D> interpolateTranslationLinear(const std::vector<Translation<T, D>>& t, const std::vector<W>& weights);

template<class T, EigenId D, class W>
Scaling<T, D> interpolateScalingLinear(const Scaling<T, D>& s1, const Scaling<T, D>& s2, const W& a);
template<class T, EigenId D, class W>
Scaling<T, D> interpolateScalingLinear(const std::vector<Scaling<T, D>>& s, const std::vector<W>& weights);

template<class T, class W>
Rotation3<T> interpolateRotationSpherical(const Rotation3<T>& r1, const Rotation3<T>& r2, const W& a);
template<class T, class W>
Rotation3<T> interpolateRotationSpherical(const std::vector<Rotation3<T>>& r, const std::vector<W>& weights);

template<class T, class W>
Quaternion<T> interpolateRotationSpherical(const Quaternion<T>& q1, const Quaternion<T>& q2, const W& a);
template<class T, class W>
Quaternion<T> interpolateRotationSpherical(const std::vector<Quaternion<T>>& q, const std::vector<W>& weights);

template<class T, class W>
DualQuaternion<T> interpolateDualQuaternion(const DualQuaternion<T>& q1, const DualQuaternion<T>& q2, const W& a);
template<class T, class W>
DualQuaternion<T> interpolateDualQuaternion(const std::vector<DualQuaternion<T>>& q, const std::vector<W>& weights);

}

#include "interpolation.cpp"

#endif // NVL_MATH_INTERPOLATION_H

#include "interpolation.h"

#include <nvl/math/comparisons.h>

namespace nvl {

template<class T, class W>
T interpolateLinear(const T& v1, const T& v2, const W& a)
{
    if (epsEqual(a, 0.0))
        return v1;
    if (epsEqual(a, 1.0))
        return v2;

    return (a - 1) * v1 + a * v2;
}

template<class T, class W>
T /*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
interpolateLinear(const std::vector<T>& v, const std::vector<W>& weights)
{
    T result(0,0,0);

    if (v.size() == 0)
        return result;
    else if (v.size() == 1)
        return v[0];

    for (Index i = 0; i < v.size(); i++) {
        const T& value = v[i];
        const W& weight = weights[i];
        result += weight * value;
    }

    return result;
}

template<class T, class W>
Affine3<T> interpolateAffine(const Affine3<T>& t1, const Affine3<T>& t2, const W& a)
{
    if (epsEqual(a, 0.0))
        return t1;
    if (epsEqual(a, 1.0))
        return t2;

    Matrix33<T> s1Matrix, r1Matrix, s2Matrix, r2Matrix;
    t1.computeRotationScaling(&r1Matrix, &s1Matrix);
    t2.computeRotationScaling(&r2Matrix, &s2Matrix);

    const Vector3<T> v1(t1.translation());
    const Vector3<T> v2(t2.translation());
    const Quaternion<T> r1(r1Matrix);
    const Quaternion<T> r2(r2Matrix);
    const Vector3<T> s1 = s1Matrix.diagonal();
    const Vector3<T> s2 = s2Matrix.diagonal();

    Affine3<T> result;
    result.fromPositionOrientationScale(
        interpolateVectorLinear(v1, v2, a),
        interpolateRotationSpherical(r1, r2, a),
        interpolateVectorLinear(s1, s2, a)
    );

    return result;
}

template<class T, class W>
Affine3<T> interpolateAffine(const std::vector<Affine3<T>>& t, const std::vector<W>& weights)
{
    Affine3<T> result = Affine3<T>::Identity();

    if (t.size() == 0)
        return result;
    else if (t.size() == 1)
        return t[0];

    std::vector<Vector3<T>> translations(t.size());
    std::vector<Quaternion<T>> quaternions(t.size());
    std::vector<Vector3<T>> scaling(t.size());

    for (Index i = 0; i < t.size(); i++) {
        const Affine3<T>& transformation = t[i];

        Matrix33<T> sMatrix, rMatrix;
        transformation.computeRotationScaling(&rMatrix, &sMatrix);

        translations[i] = Vector3<T>(transformation.translation());
        quaternions[i] = Quaternion<T>(rMatrix);
        scaling[i] = Vector3<T>(sMatrix.diagonal());
    }

    result.fromPositionOrientationScale(
        interpolateVectorLinear(translations, weights),
        interpolateRotationSpherical(quaternions, weights),
        interpolateVectorLinear(scaling, weights)
    );

    return result;
}

template<class T, EigenId D, class W>
Vector<T, D> interpolateVectorLinear(const Vector<T, D>& v1, const Vector<T, D>& v2, const W& a)
{
    if (epsEqual(a, 0.0))
        return v1;
    if (epsEqual(a, 1.0))
        return v2;

    return (v1 * (1 - a)) + (v2 * a);
}

template<class T, EigenId D, class W>
Vector<T, D> interpolateVectorLinear(const std::vector<Vector<T, D>>& v, const std::vector<W>& weights)
{
    Vector<T, D> result(0,0,0);

    if (v.size() == 0)
        return result;
    else if (v.size() == 1)
        return v[0];

    for (Index i = 0; i < v.size(); i++) {
        const Vector<T, D>& vector = v[i];
        const W& weight = weights[i];
        result += weight * vector;
    }
    return result;
}

template<class T, EigenId D, class W>
Translation<T, D> interpolateTranslationLinear(const Translation<T, D>& t1, const Translation<T, D>& t2, const W& a)
{
    if (epsEqual(a, 0.0))
        return t1;
    if (epsEqual(a, 1.0))
        return t2;

    Vector3<T> v1(t1);
    Vector3<T> v2(t2);
    return Translation<T, D>(interpolateVectorLinear(v1, v2, a));
}

template<class T, EigenId D, class W>
Translation<T, D> interpolateTranslationLinear(const std::vector<Translation<T, D>>& t, const std::vector<W>& weights)
{
    std::vector<Vector<T, D>> vectors(t.size());

    for (Index i = 0; i < t.size(); i++) {
        const Translation<T, D>& translation = t[i];

        vectors[i] = translation.vector();
    }

    Vector<T, D> vec = interpolateVectorLinear(vectors, weights);
    Translation<T, D> interpolated(vec);
    return interpolated;
}

template<class T, EigenId D, class W>
Scaling<T, D> interpolateScalingLinear(const Scaling<T, D>& s1, const Scaling<T, D>& s2, const W& a)
{
    if (epsEqual(a, 0.0))
        return s1;
    if (epsEqual(a, 1.0))
        return s2;

    Vector3<T> v1(s1.diagonal());
    Vector3<T> v2(s2.diagonal());
    return Scaling<T, D>(interpolateVectorLinear(v1, v2, a));
}

template<class T, EigenId D, class W>
Scaling<T, D> interpolateScalingLinear(const std::vector<Scaling<T, D>>& s, const std::vector<W>& weights)
{
    std::vector<Vector3<T>> vectors(s.size());

    for (Index i = 0; i < s.size(); i++) {
        const Scaling<T, D>& scaling = s[i];

        vectors[i] = Vector3<T>(scaling.diagonal());
    }

    return interpolateVectorLinear(vectors, weights);
}

template<class T, class W>
Rotation3<T> interpolateRotationSpherical(const Rotation3<T>& r1, const Rotation3<T>& r2, const W& a)
{
    if (epsEqual(a, 0.0))
        return r1;
    if (epsEqual(a, 1.0))
        return r2;

    Quaterniond q1(r1.rotation());
    Quaterniond q2(r2.rotation());
    return Rotation3<T>(interpolateRotationSpherical(q1, q2, a));
}

template<class T, class W>
Rotation3<T> interpolateRotationSpherical(const std::vector<Rotation3<T>>& r, const std::vector<W>& weights)
{
    std::vector<Quaternion<T>> quaternions(r.size());

    for (Index i = 0; i < r.size(); i++) {
        const Rotation3<T>& rotation = r[i];

        quaternions[i] = Quaternion<T>(rotation);
    }

    return interpolateRotationSpherical(quaternions, weights);
}

template<class T, class W>
Quaternion<T> interpolateRotationSpherical(const Quaternion<T>& q1, const Quaternion<T>& q2, const W& a)
{
    if (epsEqual(a, 0.0))
        return q1;
    if (epsEqual(a, 1.0))
        return q2;

    return q1.slerp(a, q2);
}

template<class T, class W>
Quaternion<T> interpolateRotationSpherical(const std::vector<Quaternion<T>>& q, const std::vector<W>& weights)
{
    Quaternion<T> result = Quaternion<T>::Identity();

    if (q.size() == 0)
        return result;

    W currentSum = 0.0;
    for (Index i = 0; i < q.size(); ++i) {
        const Quaternion<T>& quaternion = q[i];

        if (i == 0) {
            result = quaternion;
        }
        else {
            result = interpolateRotationSpherical(quaternion, result, currentSum / (currentSum + weights[i]));
        }

        currentSum += weights[i];
    }

    return result;
}

template<class T, class W>
DualQuaternion<T> interpolateDualQuaternion(const DualQuaternion<T>& q1, const DualQuaternion<T>& q2, const W& a)
{
    if (epsEqual(a, 0.0))
        return q1;
    if (epsEqual(a, 1.0))
        return q2;

    return DualQuaternion<T>(interpolateRotationSpherical(q1.rotation(), q2.rotation(), a), interpolateTranslationLinear(q1.translation(), q2.translation(), a));
}

template<class T, class W>
DualQuaternion<T> interpolateDualQuaternion(const std::vector<DualQuaternion<T>>& dq, const std::vector<W>& weights)
{
    DualQuaternion<T> result = DualQuaternion<T>::Identity();

    if (dq.size() == 0)
        return result;

    W currentSum = 0.0;
    for (Index i = 0; i < dq.size(); ++i) {
        const DualQuaternion<T>& dualQuaternion = dq[i];

        if (i == 0) {
            result = dualQuaternion;
        }
        else {
            result = interpolateDualQuaternion(dualQuaternion, result, currentSum / (currentSum + weights[i]));
        }

        currentSum += weights[i];
    }

    return result;
}

}

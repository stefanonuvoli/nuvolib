/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "triangle.h"

namespace nvl {

template<class T>
inline Triangle<T>::Triangle() :
    p{}
{
}

template<class T>
inline Triangle<T>::Triangle(const T& p1, const T& p2, const T& p3):
    p{p1, p2, p3}
{
}

template<class T>
inline const T& Triangle<T>::p1() const
{
    return p[0];
}

template<class T>
inline const T& Triangle<T>::p2() const
{
    return p[1];
}

template<class T>
inline const T& Triangle<T>::p3() const
{
    return p[2];
}

template<class T>
inline const T& Triangle<T>::operator[](unsigned int i) const
{
    assert(i < 3);
    return p[i];
}

template<class T>
inline const std::array<T,3>& Triangle<T>::vertices() const
{
    return vertices;
}


template<class T>
inline T& Triangle<T>::p1()
{
    return p[0];
}

template<class T>
inline T& Triangle<T>::p2()
{
    return p[1];
}

template<class T>
inline T& Triangle<T>::p3()
{
    return p[2];
}

template<class T>
inline T& Triangle<T>::operator[](unsigned int i)
{
    assert(i < 3);
    return p[i];
}

template<class T>
inline void Triangle<T>::setP1(const T& p1)
{
    p[0] = p1;
}

template<class T>
inline void Triangle<T>::setP2(const T& p2)
{
    p[1] = p2;
}

template<class T>
inline void Triangle<T>::setP3(const T& p3)
{
    p[2] = p3;
}

template<class T>
inline void Triangle<T>::set(const T& p1, const T& p2, const T& p3)
{
    p[0] = p1;
    p[1] = p2;
    p[2] = p3;
}

template<class T>
inline Segment<T> Triangle<T>::side1() const
{
    return Segment<T>(p[0], p[1]);
}
template<class T>
inline Segment<T> Triangle<T>::side2() const
{
    return Segment<T>(p[1], p[2]);
}
template<class T>
inline Segment<T> Triangle<T>::side3() const
{
    return Segment<T>(p[2], p[0]);
}

template<class T>
inline std::vector<Segment<T>> Triangle<T>::sides() const
{
    std::vector<Segment<T>> sides(3);
    sides[0] = side1();
    sides[1] = side2();
    sides[2] = side3();
    return sides;
}


template<class T>
inline T Triangle<T>::normal() const
{
    T n = (p[1] - p[0]).cross(p[2] - p[0]);
    n.normalize();
    return n;
}

template<class T>
inline double Triangle<T>::area() const
{
    return (((p[2] - p[0]).cross(p[1] - p[0])).norm() / 2);
}

template<class T>
inline double Triangle<T>::perimeter() const
{
    return (p[0] - p[1]).norm() + (p[1] - p[2]).norm() + (p[2] - p[0]).norm();
}

template<class T>
inline T Triangle<T>::barycenter() const
{
    return (p[0] + p[1] + p[2]) / 3;
}

}

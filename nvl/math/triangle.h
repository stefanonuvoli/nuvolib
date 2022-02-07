/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_TRIANGLE_H
#define NVL_MATH_TRIANGLE_H

#include <nvl/nuvolib.h>

#include <nvl/math/segment.h>

#include <array>

namespace nvl {

/**
 * @brief The Triangle class
 * @tparam T Point type
 */
template<class T>
class Triangle
{
public:
    Triangle();
    Triangle(const T& p1, const T& p2, const T& p3);

    const T& p1() const;
    const T& p2() const;
    const T& p3() const;
    const T& operator[](unsigned int i) const;
    const std::array<T,3>& vertices() const;

    T& p1();
    T& p2();
    T& p3();
    T& operator[](unsigned int i);
    void setP1(const T& p1);
    void setP2(const T& p2);
    void setP3(const T& p3);
    void set(const T& p1, const T& p2, const T& p3);

    Segment<T> side1() const;
    Segment<T> side2() const;
    Segment<T> side3() const;
    std::vector<Segment<T>> sides() const;

    T normal() const;
    double area() const;
    double perimeter() const;
    T barycenter() const;

protected:

    std::array<T,3> p;
};

template<class T> using Triangle2 = Triangle<Point<T,2>>;
template<class T> using Triangle3 = Triangle<Point<T,3>>;
template<class T> using Triangle4 = Triangle<Point<T,4>>;

typedef Triangle2<float> Triangle2f;
typedef Triangle2<double> Triangle2d;
typedef Triangle2<int> Triangle2i;
typedef Triangle2<long long int> Triangle2l;

typedef Triangle3<float> Triangle3f;
typedef Triangle3<double> Triangle3d;
typedef Triangle3<int> Triangle3i;
typedef Triangle3<long long int> Triangle3l;

typedef Triangle4<float> Triangle4f;
typedef Triangle4<double> Triangle4d;
typedef Triangle4<int> Triangle4i;
typedef Triangle4<long long int> Triangle4l;

}

#include "triangle.cpp"

#endif // NVL_MATH_TRIANGLE_H

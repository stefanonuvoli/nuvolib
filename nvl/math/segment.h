/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MATH_SEGMENT_H
#define NVL_MATH_SEGMENT_H

#include <nvl/nuvolib.h>

#include <nvl/math/point.h>

namespace nvl {

template<class P>
class Segment {

public:

    Segment();
    Segment(const P& p1, const P& p2);

    const P& p1() const;
    P& p1();
    void setP1(const P& value);

    const P& p2() const;
    P& p2();
    void setP2(const P& value);

    bool operator<(const Segment<P>& vec) const;
    bool operator<=(const Segment<P>& vec) const;
    bool operator>(const Segment<P>& vec) const;
    bool operator>=(const Segment<P>& vec) const;
    bool operator==(const Segment<P>& vec) const;

private:

    void swapIfNecessary();

    P vP1;
    P vP2;
};

/* Common typedefs */

template<class T> using Segment2 = Segment<Point2<T>>;
typedef Segment2<float> Segment2f;
typedef Segment2<double> Segment2d;
typedef Segment2<int> Segment2i;
typedef Segment2<long long int> Segment2l;

template<class T> using Segment3 = Segment<Point3<T>>;
typedef Segment3<float> Segment3f;
typedef Segment3<double> Segment3d;
typedef Segment3<int> Segment3i;
typedef Segment3<long long int> SegmentSegment3l;

template<class T> using Segment4 = Segment<Point4<T>>;
typedef Segment4<float> Segment4f;
typedef Segment4<double> Segment4d;
typedef Segment4<int> Segment4i;
typedef Segment4<long long int> Segment4l;

}

#include "segment.cpp"

#endif //NVL_MATH_SEGMENT_H

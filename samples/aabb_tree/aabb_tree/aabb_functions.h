/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef AABB_FUNCTIONS_H
#define AABB_FUNCTIONS_H

#include <nvl/math/point.h>
#include <nvl/math/segment.h>
#include <nvl/structures/trees/aabbtree.h>

typedef nvl::AABBValueType AABBValueType;
typedef nvl::Point2<int> Point2D;
typedef nvl::Segment<Point2D> Segment2D;

double aabbValueExtractor(
		const Segment2D& segment,
		const AABBValueType& valueType,
		const int& dim);

bool segment2DIntersectionChecker(const Segment2D& segment1, const Segment2D& segment2);

bool segment2DCustomComparator(const Segment2D& o1, const Segment2D& o2);

#endif //AABB_FUNCTIONS_H

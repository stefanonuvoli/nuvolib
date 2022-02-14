/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef RANGE_TREE_FUNCTIONS_H
#define RANGE_TREE_FUNCTIONS_H

#include <nvl/math/point.h>

typedef nvl::Point2d Point2d;

/* ---- COMPARATORS FOR POINTERS ----- */

bool point2DPointerDimensionComparatorX(Point2d* const& o1, Point2d* const& o2);
bool point2DPointerDimensionComparatorY(Point2d* const& o1, Point2d* const& o2);

#endif // RANGETEST_H

/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "triangulate.h"

namespace nvl {

/**
 * @brief Triangulation of a convex polygon
 * @param points Points
 * @return Triangulation
 */
template<EigenId D, class T>
std::vector<std::vector<Index>> triangulateConvexPolygon(const std::vector<Point<T,D>>& points)
{
    //TODO TEST!

    std::vector<std::vector<Index>> triangulation;

    Size vertexNumber = points.size();

    triangulation.resize(vertexNumber - 2);

    for (Index j = 0; j < vertexNumber - 2; ++j) {
        std::vector<Index> newVertices(3);
        newVertices[0] = 0;
        newVertices[1] = j + 1;
        newVertices[2] = (j + 2) % vertexNumber;
        triangulation[j] = newVertices;
    }

    return triangulation;
}

}

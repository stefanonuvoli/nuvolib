/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "mesh_implicit_function.h"

#include <nvl/math/comparisons.h>

#include <set>

namespace nvl {

/**
 * @brief Given an implicit per-vertex function over a mesh, get the segments lying
 * on the zero values
 * @param mesh Mesh
 * @param vertexFunction Per-vertex function
 * @return Segments passing in the zero of the function
 */
template<class Mesh, class F>
std::vector<Segment<typename Mesh::Point>> meshImplicitFunction(
        const Mesh& mesh,
        const std::vector<F>& vertexFunction)
{
    typedef typename Mesh::Face Face;
    typedef typename Mesh::FaceId FaceId;
    typedef typename Mesh::VertexId VertexId;
    typedef typename Mesh::Point Point;

    std::vector<Segment<typename Mesh::Point>> functionSegments;

    std::set<Point> pointSet;
    for (FaceId fId = 0; fId < mesh.nextFaceId(); ++fId) {
        if (mesh.isFaceDeleted(fId))
            continue;

        const Face& face = mesh.face(fId);

        std::vector<bool> hasZeroValue(face.vertexNumber(), false);
        std::vector<Point> zeroPoint(face.vertexNumber());

        for (Size j = 0; j < face.vertexNumber(); ++j) {
            VertexId v1 = face.vertexId(j);
            VertexId v2 = face.nextVertexId(j);

            F f1 = vertexFunction[v1];
            F f2 = vertexFunction[v2];

            Point p0 = mesh.vertexPoint(v1);
            Point p1 = mesh.vertexPoint(v2);

            if (epsEqual(f1, 0.0) || epsEqual(f2, 0.0)) {
                continue;
            }

            if (f1 * f2 < 0) {
                F sum = fabs(f1) + fabs(f2);
                F w = 1 - (fabs(f1) / sum);

                hasZeroValue[j] = true;
                zeroPoint[j] = p0 * w + p1 * (1 - w);
            }
        }

        for (Index j = 0; j < face.vertexNumber(); ++j) {
            for (Index k = j + 1; k <= face.vertexNumber(); ++k) {
                Index n = k % face.vertexNumber();
                if (n != j && hasZeroValue[j] && hasZeroValue[n]) {
                    Segment<Point> seg(zeroPoint[j], zeroPoint[n]);

                    //Avoid degenerate segments
                    if (!epsEqual(seg.p1(), seg.p2())) {
                        functionSegments.push_back(seg);

                        pointSet.insert(zeroPoint[j]);
                        pointSet.insert(zeroPoint[n]);
                    }

                    continue;
                }
            }
        }
    }

    //Merge closest points
    for (typename std::set<Point>::iterator it1 = pointSet.begin(); it1 != pointSet.end(); ++it1) {
        typename std::set<Point>::iterator it2 = it1;

        Point value = *it1;

        ++it2;

        while (it2 != pointSet.end() && epsEqual(value, *it2)) {
            for (Segment<Point>& seg : functionSegments) {
                if (seg.p1() != value && epsEqual(seg.p1(), value)) {
                    seg.setP1(value);
                }
                if (seg.p2() != value && epsEqual(seg.p2(), value)) {
                    seg.setP2(value);
                }
            }

            ++it2;
        }
    }

    //Sort and erase duplicates
    std::sort(functionSegments.begin(), functionSegments.end());
    functionSegments.erase(std::unique(functionSegments.begin(), functionSegments.end()), functionSegments.end());

    return functionSegments;
}

}

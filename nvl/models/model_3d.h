/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_MODELS_MODEL_3D_H
#define NVL_MODELS_MODEL_3D_H

#include <nvl/nuvolib.h>

#include <nvl/models/structures/model.h>
#include <nvl/models/mesh_3d.h>
#include <nvl/models/skeleton_3d.h>
#include <nvl/models/skinning_weights_3d.h>
#include <nvl/models/animation_3d.h>


namespace nvl {

typedef Model<PolygonMesh3d, Skeleton3d, SkinningWeightsd, Animation3d> PolygonModel3d;
typedef Model<TriangleMesh3d, Skeleton3d, SkinningWeightsd, Animation3d> TriangleModel3d;
typedef Model<QuadMesh3d, Skeleton3d, SkinningWeightsd, Animation3d> QuadModel3d;
typedef PolygonModel3d Model3d;

}

#endif // NVL_MODELS_MODEL_3D_H

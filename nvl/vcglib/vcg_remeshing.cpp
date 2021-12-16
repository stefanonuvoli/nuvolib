#include "vcg_remeshing.h"

#include <vcg/complex/algorithms/isotropic_remeshing.h>

#include <nvl/vcglib/vcg_convert.h>
#include <nvl/vcglib/vcg_triangle_mesh.h>

namespace nvl {

template<class Mesh>
Mesh isotropicRemeshing(
        const Mesh& mesh,
        const double& edgeSize,
        const unsigned int& iterations,
        const bool& fixBorders)
{
    Mesh resultMesh;

    VCGTriangleMesh vcgMesh;
    VCGTriangleMesh vcgRemeshed;

    convertMeshToVCGMesh(mesh, vcgMesh);
    vcgIsotropicRemeshing(vcgMesh, vcgRemeshed, edgeSize, iterations, fixBorders);
    convertVCGMeshToMesh(vcgRemeshed, resultMesh);

    return resultMesh;
}

template<class VCGMesh>
void vcgIsotropicRemeshing(
        VCGMesh& vcgMesh,
        VCGMesh& vcgResultMesh,
        const double& edgeSize,
        const unsigned int& iterations,
        const bool& fixBorders)
{
    vcg::tri::UpdateNormal<VCGMesh>::PerFaceNormalized(vcgMesh);
    vcg::tri::UpdateNormal<VCGMesh>::PerVertexNormalized(vcgMesh);
    vcg::tri::UpdateBounding<VCGMesh>::Box(vcgMesh);
    vcg::tri::UpdateTopology<VCGMesh>::FaceFace(vcgMesh);
    vcg::tri::UpdateTopology<VCGMesh>::VertexFace(vcgMesh);
    vcg::tri::UpdateFlags<VCGMesh>::FaceBorderFromFF(vcgMesh);
    vcg::tri::UpdateFlags<VCGMesh>::VertexBorderFromNone(vcgMesh);

    //Fix borders
    vcg::tri::UpdateSelection<VCGMesh>::Clear(vcgMesh);

    if (fixBorders) {
        for (size_t i=0;i<vcgMesh.face.size();i++) {
            for (int j=0;j<vcgMesh.face[i].VN();j++)
            {
                if (!vcg::face::IsBorder(vcgMesh.face[i],j))continue;
                vcgMesh.face[i].SetS();
            }
        }
        for (size_t i=0;i<vcgMesh.face.size();i++)
        {
            if (vcgMesh.face[i].IsS())
                vcgMesh.face[i].ClearS();
            else
                vcgMesh.face[i].SetS();
        }
    }

    typename vcg::tri::IsotropicRemeshing<VCGMesh>::Params Par;

    Par.swapFlag     = true;
    Par.collapseFlag = true;
    Par.smoothFlag=true;
    Par.projectFlag=true;
    Par.SetFeatureAngleDeg(100);
    Par.SetTargetLen(edgeSize);
    Par.selectedOnly=true;
    Par.adapt=false;
    Par.iter=iterations;

    vcg::tri::Append<VCGMesh,VCGMesh>::Mesh(vcgResultMesh,vcgMesh);

    vcg::tri::UpdateNormal<VCGMesh>::PerFaceNormalized(vcgResultMesh);
    vcg::tri::UpdateNormal<VCGMesh>::PerVertexNormalized(vcgResultMesh);
    vcg::tri::UpdateBounding<VCGMesh>::Box(vcgResultMesh);
    vcg::tri::UpdateTopology<VCGMesh>::FaceFace(vcgResultMesh);
    vcg::tri::UpdateTopology<VCGMesh>::VertexFace(vcgResultMesh);
    vcg::tri::UpdateFlags<VCGMesh>::FaceBorderFromFF(vcgResultMesh);
    vcg::tri::UpdateFlags<VCGMesh>::VertexBorderFromNone(vcgResultMesh);

    vcg::tri::IsotropicRemeshing<VCGMesh>::Do(vcgResultMesh,vcgMesh,Par);
}

}

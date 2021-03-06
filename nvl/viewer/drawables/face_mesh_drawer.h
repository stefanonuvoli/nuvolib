/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_VIEWER_FACE_MESH_DRAWER_H
#define NVL_VIEWER_FACE_MESH_DRAWER_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/drawables/polyline_mesh_drawer.h>
#include <nvl/viewer/drawables/face_mesh_drawer_base.h>

#include <nvl/utilities/color.h>

#include <vector>

namespace nvl {

template<class M>
class FaceMeshDrawer :
        public FaceMeshDrawerBase,
        public PolylineMeshDrawer<M>
{

public:

    FaceMeshDrawer();
    FaceMeshDrawer(M* mesh, const bool visible = true, const bool pickable = true);
    virtual ~FaceMeshDrawer();

    virtual void draw() const override;
    virtual void drawWithNames(Canvas* canvas, const Index drawableId) const override;

    virtual void update() override;

    bool hasFaceNormals() const override;
    bool hasFaceColors() const override;
    bool hasTextures() const override;
    bool hasFaceTransparency() const override;
    bool hasFaceLighting() const override;
    bool hasSmoothShading() const override;
    bool hasFlatShading() const override;
    bool hasVertexColors() const override;

    const std::vector<unsigned int>& renderingFace(const Index& id) const;
    Vector3d renderingFaceNormal(const Index& id) const;
    Color renderingFaceColor(const Index& id) const;
    Color renderingFaceWireframeColor(const Index& id) const;

    virtual void resetRenderingData();
    void resetRenderingFaceData();

    void resetRenderingFaces();
    void resetRenderingFaceNormals();
    void resetRenderingFaceColors();
    void resetRenderingFaceUVs();
    void resetRenderingFaceWireframeColors();
    void resetRenderingFaceMaterials();
    void resetRenderingFace(const Index& id);
    void resetRenderingFaceNormal(const Index& id);
    void resetRenderingFaceColor(const Index& id);
    void resetRenderingFaceUV(const Index& id);
    void resetRenderingFaceWireframeColor(const Index& id);
    void resetRenderingFaceMaterial(const Index& id);

    void setRenderingFaces(const std::vector<std::vector<unsigned int>>& renderingFaces);
    void setRenderingFaceNormals(const std::vector<double>& renderingFaceNormals);
    void setRenderingFaceColors(const std::vector<float>& renderingFaceColors);
    void setRenderingFaceUVs(const std::vector<std::vector<float>>& renderingFaceUVs);
    void setRenderingFaceWireframeColors(const std::vector<float>& renderingFaceColors);
    void setRenderingFaceMaterials(const std::vector<Index>& renderingFaceMaterials);
    void setRenderingFace(const Index& id, const std::vector<unsigned int>& f);
    void setRenderingFaceNormal(const Index& id, const Vector3d& n);
    void setRenderingFaceColor(const Index& id, const Color& c);
    void setRenderingFaceUV(const Index& id, const std::vector<Point2f>& uvs);
    void setRenderingFaceWireframeColor(const Index& id, const Color& c);
    void setRenderingFaceMaterial(const Index& id, const Index& m);

    void loadTextures();
    void clearTextures();

protected:

    std::vector<Index> vFaceMap;

    std::vector<unsigned int> vTextures;

    std::vector<std::vector<unsigned int>> vRenderingFaces;
    std::vector<double> vRenderingFaceNormals;
    std::vector<float> vRenderingFaceColors;
    std::vector<std::vector<float>> vRenderingFaceUVs;
    std::vector<float> vRenderingFaceWireframeColors;
    std::vector<Index> vRenderingFaceMaterials;

    void drawFaceSmoothShading() const;
    void drawFaceFlatShading() const;
    void drawWireframe() const;
    void drawFaceNormals() const;

    void drawFaceWithNames(Canvas* canvas, const Index drawableId) const;
    void drawWireframeWithNames(Canvas* canvas, const Index drawableId) const;

    void drawShader() const;

    double getFaceNormalRadius() const;
    double getFaceNormalLength() const;

    Color vDefaultFaceColor;
};

}

#include "face_mesh_drawer.cpp"

#endif // NVL_VIEWER_FACE_MESH_DRAWER_H

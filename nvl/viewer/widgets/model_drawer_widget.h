#ifndef NVL_VIEWER_MODEL_DRAWER_WIDGET_H
#define NVL_VIEWER_MODEL_DRAWER_WIDGET_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/widgets/qcanvas.h>
#include <nvl/viewer/widgets/drawable_list_widget.h>
#include <nvl/viewer/widgets/face_mesh_drawer_widget.h>
#include <nvl/viewer/widgets/skeleton_drawer_widget.h>
#include <nvl/viewer/widgets/polyline_mesh_drawer_widget.h>
#include <nvl/viewer/widgets/vertex_mesh_drawer_widget.h>

#include <QTabWidget>

namespace Ui {
class ModelDrawerWidget;
}

namespace nvl {

class ModelDrawerWidget : public QTabWidget
{

    Q_OBJECT

public:

    explicit ModelDrawerWidget(
            QCanvas* canvas,
            DrawableListWidget* drawableListWidget,
            QWidget *parent = nullptr);
    ~ModelDrawerWidget();

private:

    Ui::ModelDrawerWidget *ui;

    QCanvas* vCanvas;
    DrawableListWidget* vDrawableListWidget;
    FaceMeshDrawerWidget* vFaceMeshDrawerWidget;
    PolylineMeshDrawerWidget* vPolylineMeshDrawerWidget;
    VertexMeshDrawerWidget* vVertexMeshDrawerWidget;
    SkeletonDrawerWidget* vSkeletonDrawerWidget;
};

}

#include "model_drawer_widget.cpp"

#endif // NVL_VIEWER_MODEL_DRAWER_WIDGET_H

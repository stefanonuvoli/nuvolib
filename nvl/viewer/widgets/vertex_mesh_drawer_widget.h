#ifndef NVL_VIEWER_VERTEX_MESH_DRAWER_WIDGET_H
#define NVL_VIEWER_VERTEX_MESH_DRAWER_WIDGET_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/widgets/canvas.h>
#include <nvl/viewer/widgets/drawable_list_widget.h>
#include <nvl/viewer/drawables/vertex_mesh_drawer.h>

#include <nvl/models/meshes.h>
#include <nvl/models/model.h>

#include <QFrame>

namespace Ui {
class VertexMeshDrawerWidget;
}

namespace nvl {

class VertexMeshDrawerWidget : public QFrame
{

    Q_OBJECT

public:

    explicit VertexMeshDrawerWidget(
            Canvas* canvas,
            DrawableListWidget* drawableListWidget,
            QWidget *parent = nullptr);
    ~VertexMeshDrawerWidget();

public Q_SLOTS:

    void slot_drawableSelectionChanged();
    void slot_selectedDrawableUpdated();


private Q_SLOTS:

    void on_vertexVisibleCheckBox_stateChanged(int arg1);
    void on_vertexTransparencyCheckBox_stateChanged(int arg1);
    void on_vertexShapePointButton_clicked();
    void on_vertexShapeDotButton_clicked();
    void on_vertexSizeSlider_valueChanged(int value);
    void on_vertexColorVertexButton_clicked();
    void on_vertexColorUniformButton_clicked();
    void on_vertexColorUniformColorButton_colorChanged(QColor color);
    void on_vertexNormalsCheckBox_stateChanged(int arg1);
    void on_vertexNormalsSlider_valueChanged(int value);



private:

    void updateView();

    void updateVertexView();

    void getSelectedDrawers();

    void connectSignals();

    Ui::VertexMeshDrawerWidget *ui;

    Canvas* vCanvas;
    DrawableListWidget* vDrawableListWidget;

    std::vector<VertexMeshDrawerBase*> vVertexMeshDrawers;

    bool vHandleUpdate;
};

}

#include "vertex_mesh_drawer_widget.cpp"

#endif // NVL_VIEWER_VERTEX_MESH_DRAWER_WIDGET_H

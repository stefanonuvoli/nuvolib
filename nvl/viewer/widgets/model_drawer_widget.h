#ifndef NVL_VIEWER_MODEL_DRAWER_WIDGET_H
#define NVL_VIEWER_MODEL_DRAWER_WIDGET_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/widgets/canvas.h>
#include <nvl/viewer/widgets/drawable_list_widget.h>
#include <nvl/viewer/drawables/model_drawer.h>
#include <nvl/viewer/drawables/face_mesh_drawer_base.h>
#include <nvl/viewer/drawables/polyline_mesh_drawer_base.h>
#include <nvl/viewer/drawables/vertex_mesh_drawer_base.h>
#include <nvl/viewer/gl/ramp_qglshader.h>
#include <nvl/viewer/gl/contour_qglshader.h>

#include <nvl/models/meshes.h>
#include <nvl/models/model.h>

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
            Canvas* canvas,
            DrawableListWidget* drawableListWidget,
            QWidget *parent = nullptr);
    ~ModelDrawerWidget();

public Q_SLOTS:

    void slot_drawableSelectionChanged();
    void slot_selectedDrawableUpdated();


private Q_SLOTS:

    void on_faceVisibleCheckBox_stateChanged(int arg1);
    void on_faceTextureVisibleCheckBox_stateChanged(int arg1);
    void on_faceShadingSmoothButton_clicked();
    void on_faceShadingFlatButton_clicked();
    void on_faceTransparencyCheckBox_stateChanged(int arg1);
    void on_faceColorFaceButton_clicked();
    void on_faceColorVertexButton_clicked();
    void on_faceColorUniformButton_clicked();
    void on_faceColorUniformColorButton_colorChanged(QColor color);
    void on_faceWireframeCheckBox_stateChanged(int arg1);
    void on_faceWireframeSlider_valueChanged(int value);
    void on_faceWireframeColorButton_colorChanged(QColor color);
    void on_faceNormalsCheckBox_stateChanged(int arg1);
    void on_faceNormalsSlider_valueChanged(int value);
    void on_faceShaderComboBox_currentIndexChanged(int index);

    void on_skeletonVisibleCheckBox_stateChanged(int arg1);
    void on_skeletonTransparencyCheckBox_stateChanged(int arg1);
    void on_skeletonJointVisibleCheckBox_stateChanged(int arg1);
    void on_skeletonJointSlider_valueChanged(int value);
    void on_skeletonJointColorButton_colorChanged(QColor color);
    void on_skeletonRootColorButton_colorChanged(QColor color);
    void on_skeletonBoneSlider_valueChanged(int value);
    void on_skeletonBoneVisibleCheckBox_stateChanged(int arg1);
    void on_skeletonBoneColorButton_colorChanged(QColor color);

private:

    enum ShaderOptions { SHADER_NONE, SHADER_RAMP, SHADER_CONTOUR };

    void updateView();

    void updateFaceView();
    void updatePolylineView();
    void updateVertexView();
    void updateSkeletonView();
    void updateOthersView();

    void getSelectedDrawers();

    void connectSignals();

    Ui::ModelDrawerWidget *ui;

    Canvas* vCanvas;
    DrawableListWidget* vDrawableListWidget;

    std::vector<FaceMeshDrawerBase*> vFaceMeshDrawers;
    std::vector<PolylineMeshDrawerBase*> vPolylineMeshDrawers;
    std::vector<VertexMeshDrawerBase*> vVertexMeshDrawers;
    std::vector<SkeletonDrawerBase*> vSkeletonDrawers;

    ContourQGLShader contourQGLShader;
    RampQGLShader rampQGLShader;

    bool vHandleUpdate;
};

}

#include "model_drawer_widget.cpp"

#endif // NVL_VIEWER_MODEL_DRAWER_WIDGET_H

#ifndef NVL_VIEWER_FACE_MESH_DRAWER_WIDGET_H
#define NVL_VIEWER_FACE_MESH_DRAWER_WIDGET_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/widgets/canvas.h>
#include <nvl/viewer/widgets/drawable_list_widget.h>
#include <nvl/viewer/drawables/face_mesh_drawer.h>
#include <nvl/viewer/gl/ramp_qglshader.h>
#include <nvl/viewer/gl/contour_qglshader.h>

#include <QFrame>

namespace Ui {
class FaceMeshDrawerWidget;
}

namespace nvl {

class FaceMeshDrawerWidget : public QFrame
{

    Q_OBJECT

public:

    explicit FaceMeshDrawerWidget(
            Canvas* canvas,
            DrawableListWidget* drawableListWidget,
            QWidget *parent = nullptr);
    ~FaceMeshDrawerWidget();

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

private:

    enum ShaderOptions { SHADER_NONE, SHADER_RAMP, SHADER_CONTOUR };

    void updateView();

    void updateFaceView();

    void getSelectedDrawers();

    void connectSignals();

    Ui::FaceMeshDrawerWidget *ui;

    Canvas* vCanvas;
    DrawableListWidget* vDrawableListWidget;

    std::vector<FaceMeshDrawerBase*> vFaceMeshDrawers;

    ContourQGLShader contourQGLShader;
    RampQGLShader rampQGLShader;

    bool vHandleUpdate;
};

}

#include "face_mesh_drawer_widget.cpp"

#endif // NVL_VIEWER_FACE_MESH_DRAWER_WIDGET_H

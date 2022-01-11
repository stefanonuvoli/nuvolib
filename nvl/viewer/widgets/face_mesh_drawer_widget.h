#ifndef NVL_VIEWER_FACE_MESH_DRAWER_WIDGET_H
#define NVL_VIEWER_FACE_MESH_DRAWER_WIDGET_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/widgets/qcanvas.h>
#include <nvl/viewer/widgets/drawable_list_widget.h>
#include <nvl/viewer/drawables/face_mesh_drawer.h>

#ifdef NVL_QGLVIEWER_LOADED
#include <nvl/viewer/gl/qgl_ramp_shader.h>
#include <nvl/viewer/gl/qgl_contour_shader.h>
#endif

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
            QCanvas* canvas,
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
    void on_faceLightingCheckBox_stateChanged(int arg1);
    void on_faceTextureModeComboBox_currentIndexChanged(int index);

private:

    enum ShaderOptions { SHADER_NONE, SHADER_RAMP, SHADER_CONTOUR };

    void updateView();

    void updateFaceView();

    void getSelectedDrawers();

    void connectSignals();

    Ui::FaceMeshDrawerWidget *ui;

    QCanvas* vCanvas;
    DrawableListWidget* vDrawableListWidget;

    std::vector<FaceMeshDrawerBase*> vFaceMeshDrawers;

#ifdef NVL_QGLVIEWER_LOADED
    QGLContourShader contourQGLShader;
    QGLRampShader rampQGLShader;
#endif

    bool vHandleUpdate;
};

}


#include "face_mesh_drawer_widget.cpp"

#endif // NVL_VIEWER_FACE_MESH_DRAWER_WIDGET_H

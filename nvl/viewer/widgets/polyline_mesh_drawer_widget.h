/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_VIEWER_POLYLINE_MESH_DRAWER_WIDGET_H
#define NVL_VIEWER_POLYLINE_MESH_DRAWER_WIDGET_H

#include <nvl/nuvolib.h>

#ifdef NVL_QT
#ifdef NVL_OPENGL_LOADED

#include <nvl/viewer/widgets/qcanvas.h>
#include <nvl/viewer/widgets/drawable_list_widget.h>
#include <nvl/viewer/drawables/polyline_mesh_drawer.h>

#include <QFrame>

namespace Ui {
class PolylineMeshDrawerWidget;
}

namespace nvl {

class PolylineMeshDrawerWidget : public QFrame
{

    Q_OBJECT

public:

    explicit PolylineMeshDrawerWidget(
            QCanvas* canvas,
            DrawableListWidget* drawableListWidget,
            QWidget *parent = nullptr);
    ~PolylineMeshDrawerWidget();

public Q_SLOTS:

    void slot_drawableSelectionChanged();
    void slot_selectedDrawableUpdated();


private Q_SLOTS:

    void on_polylineVisibleCheckBox_stateChanged(int arg1);
    void on_polylineTransparencyCheckBox_stateChanged(int arg1);
    void on_polylineShapeLineButton_clicked();
    void on_polylineShapeCylinderButton_clicked();
    void on_polylineShapeArrowButton_clicked();
    void on_polylineSizeSlider_valueChanged(int value);
    void on_polylineColorPolylineButton_clicked();
    void on_polylineColorVertexButton_clicked();
    void on_polylineColorUniformButton_clicked();
    void on_polylineColorUniformColorButton_colorChanged(QColor color);



private:

    void updateView();

    void updatePolylineView();

    void getSelectedDrawers();

    void connectSignals();

    Ui::PolylineMeshDrawerWidget *ui;

    QCanvas* vCanvas;
    DrawableListWidget* vDrawableListWidget;

    std::vector<PolylineMeshDrawerBase*> vPolylineMeshDrawers;

    bool vHandleUpdate;
};

}

#endif
#endif

#include "polyline_mesh_drawer_widget.cpp"

#endif // NVL_VIEWER_POLYLINE_MESH_DRAWER_WIDGET_H

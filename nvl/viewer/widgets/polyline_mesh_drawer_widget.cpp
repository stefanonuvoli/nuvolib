#include "polyline_mesh_drawer_widget.h"
#include "ui_polyline_mesh_drawer_widget.h"

#include <nvl/models/model_3d.h>

namespace nvl {

NVL_INLINE PolylineMeshDrawerWidget::PolylineMeshDrawerWidget(
        QCanvas* canvas,
        DrawableListWidget* drawableListWidget,
        QWidget *parent) :
    QFrame(parent),
    ui(new Ui::PolylineMeshDrawerWidget),
    vCanvas(canvas),
    vDrawableListWidget(drawableListWidget),
    vHandleUpdate(true)
{
    ui->setupUi(this);

    connectSignals();

    updateView();
}

NVL_INLINE PolylineMeshDrawerWidget::~PolylineMeshDrawerWidget()
{
    delete ui;
}

NVL_INLINE void PolylineMeshDrawerWidget::slot_drawableSelectionChanged()
{
    getSelectedDrawers();

    updateView();
}

NVL_INLINE void PolylineMeshDrawerWidget::slot_selectedDrawableUpdated()
{
    updateView();
}

NVL_INLINE void PolylineMeshDrawerWidget::updateView()
{
    vHandleUpdate = false;

    updatePolylineView();

    vHandleUpdate = true;
}

NVL_INLINE void PolylineMeshDrawerWidget::updatePolylineView()
{
    assert(!vHandleUpdate);

    bool polylineEnabled = !vPolylineMeshDrawers.empty() && vCanvas != nullptr;

    bool polylineColorEnabled = polylineEnabled;
    bool vertexColorEnabled = polylineEnabled;
    bool polylineTransparencyEnabled = polylineEnabled;

    bool polylineVisible = polylineEnabled;
    int polylineSize = 1;
    bool lineSelected = polylineEnabled;
    bool cylinderSelected = polylineEnabled;
    bool arrowSelected = polylineEnabled;
    bool polylineColorSelected = polylineEnabled;
    bool vertexColorSelected = polylineEnabled;
    bool uniformColorSelected = polylineEnabled;
    Color polylineUniformColor(255,255,255);
    bool polylineTransparency = polylineEnabled;

    if (polylineEnabled) {
        bool first = true;

        for (PolylineMeshDrawerBase* meshDrawer : vPolylineMeshDrawers) {
            polylineColorEnabled &= meshDrawer->hasPolylineColors();
            vertexColorEnabled &= meshDrawer->hasVertexColors();
            polylineTransparencyEnabled &= meshDrawer->hasPolylineTransparency();

            polylineVisible &= meshDrawer->polylineVisible();
            lineSelected &= meshDrawer->polylineShapeMode() == PolylineMeshDrawerBase::PolylineShapeMode::POLYLINE_SHAPE_LINE;
            cylinderSelected &= meshDrawer->polylineShapeMode() == PolylineMeshDrawerBase::PolylineShapeMode::POLYLINE_SHAPE_CYLINDER;
            arrowSelected &= meshDrawer->polylineShapeMode() == PolylineMeshDrawerBase::PolylineShapeMode::POLYLINE_SHAPE_ARROW;
            polylineColorSelected &= polylineColorEnabled && meshDrawer->polylineColorMode() == PolylineMeshDrawerBase::PolylineColorMode::POLYLINE_COLOR_PER_POLYLINE;
            vertexColorSelected &= vertexColorEnabled && meshDrawer->polylineColorMode() == PolylineMeshDrawerBase::PolylineColorMode::POLYLINE_COLOR_PER_VERTEX;
            uniformColorSelected &= meshDrawer->polylineColorMode() == PolylineMeshDrawerBase::PolylineColorMode::POLYLINE_COLOR_UNIFORM;
            polylineUniformColor = (first || polylineUniformColor == meshDrawer->polylineUniformColor() ? meshDrawer->polylineUniformColor() : Color(255,255,255));
            polylineSize = (first || polylineSize == meshDrawer->polylineSize() ? meshDrawer->polylineSize() : 1);
            polylineTransparency &= polylineTransparencyEnabled && meshDrawer->polylineTransparency();

            first = false;
        }
    }

    ui->polylineColorPolylineButton->setEnabled(polylineColorEnabled);
    ui->polylineColorVertexButton->setEnabled(vertexColorEnabled);
    ui->polylineTransparencyCheckBox->setEnabled(polylineTransparencyEnabled);

    this->setEnabled(polylineEnabled);
    ui->polylineVisibleCheckBox->setChecked(polylineVisible);
    ui->polylineShapeLineButton->setChecked(lineSelected);
    ui->polylineShapeCylinderButton->setChecked(cylinderSelected);
    ui->polylineShapeArrowButton->setChecked(arrowSelected);
    ui->polylineSizeSlider->setValue(polylineSize);
    ui->polylineColorPolylineButton->setChecked(polylineColorSelected);
    ui->polylineColorVertexButton->setChecked(vertexColorSelected);
    ui->polylineColorUniformButton->setChecked(uniformColorSelected);
    ui->polylineColorUniformColorButton->setColor(polylineUniformColor.toQColor());
    ui->polylineTransparencyCheckBox->setChecked(polylineTransparency);
}

NVL_INLINE void PolylineMeshDrawerWidget::getSelectedDrawers()
{
    vPolylineMeshDrawers.clear();

    if (vCanvas != nullptr) {
        for (const Index drawableId : vDrawableListWidget->selectedDrawables()) {
            Drawable* drawable = vCanvas->drawable(drawableId);

            Drawable* meshDrawerWrapper;
            ModelDrawer<Model3d>* modelDrawer = dynamic_cast<ModelDrawer<Model3d>*>(drawable);
            if (modelDrawer != nullptr) {
                meshDrawerWrapper = &modelDrawer->meshDrawer();
            }
            else {
                meshDrawerWrapper = drawable;
            }

            PolylineMeshDrawerBase* polylineMeshDrawer = dynamic_cast<PolylineMeshDrawerBase*>(meshDrawerWrapper);
            if (polylineMeshDrawer != nullptr) {
                vPolylineMeshDrawers.push_back(polylineMeshDrawer);
            }
        }
    }
}

NVL_INLINE void PolylineMeshDrawerWidget::connectSignals()
{
    if (vCanvas != nullptr) {
        //Connect signals to the viewer
        connect(vDrawableListWidget, &DrawableListWidget::signal_drawableSelectionChanged, this, &PolylineMeshDrawerWidget::slot_drawableSelectionChanged);
    }
}

NVL_INLINE void PolylineMeshDrawerWidget::on_polylineVisibleCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (PolylineMeshDrawerBase* meshDrawer : vPolylineMeshDrawers) {
            meshDrawer->setPolylineVisible(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void PolylineMeshDrawerWidget::on_polylineTransparencyCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (PolylineMeshDrawerBase* meshDrawer : vPolylineMeshDrawers) {
            meshDrawer->setPolylineTransparency(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}


NVL_INLINE void PolylineMeshDrawerWidget::on_polylineShapeLineButton_clicked()
{
    if (vHandleUpdate) {
        for (PolylineMeshDrawerBase* meshDrawer : vPolylineMeshDrawers) {
            meshDrawer->setPolylineShapeMode(PolylineMeshDrawerBase::PolylineShapeMode::POLYLINE_SHAPE_LINE);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void PolylineMeshDrawerWidget::on_polylineShapeCylinderButton_clicked()
{
    if (vHandleUpdate) {
        for (PolylineMeshDrawerBase* meshDrawer : vPolylineMeshDrawers) {
            meshDrawer->setPolylineShapeMode(PolylineMeshDrawerBase::PolylineShapeMode::POLYLINE_SHAPE_CYLINDER);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void PolylineMeshDrawerWidget::on_polylineShapeArrowButton_clicked()
{
    if (vHandleUpdate) {
        for (PolylineMeshDrawerBase* meshDrawer : vPolylineMeshDrawers) {
            meshDrawer->setPolylineShapeMode(PolylineMeshDrawerBase::PolylineShapeMode::POLYLINE_SHAPE_ARROW);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void PolylineMeshDrawerWidget::on_polylineSizeSlider_valueChanged(int value)
{
    if (vHandleUpdate) {
        for (PolylineMeshDrawerBase* meshDrawer : vPolylineMeshDrawers) {
            meshDrawer->setPolylineSize(value);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void PolylineMeshDrawerWidget::on_polylineColorPolylineButton_clicked()
{
    if (vHandleUpdate) {
        for (PolylineMeshDrawerBase* meshDrawer : vPolylineMeshDrawers) {
            meshDrawer->setPolylineColorMode(PolylineMeshDrawerBase::PolylineColorMode::POLYLINE_COLOR_PER_POLYLINE);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void PolylineMeshDrawerWidget::on_polylineColorVertexButton_clicked()
{
    if (vHandleUpdate) {
        for (PolylineMeshDrawerBase* meshDrawer : vPolylineMeshDrawers) {
            meshDrawer->setPolylineColorMode(PolylineMeshDrawerBase::PolylineColorMode::POLYLINE_COLOR_PER_VERTEX);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void PolylineMeshDrawerWidget::on_polylineColorUniformButton_clicked()
{
    if (vHandleUpdate) {
        for (PolylineMeshDrawerBase* meshDrawer : vPolylineMeshDrawers) {
            meshDrawer->setPolylineColorMode(PolylineMeshDrawerBase::PolylineColorMode::POLYLINE_COLOR_UNIFORM);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void PolylineMeshDrawerWidget::on_polylineColorUniformColorButton_colorChanged(QColor color)
{
    if (vHandleUpdate) {
        for (PolylineMeshDrawerBase* meshDrawer : vPolylineMeshDrawers) {
            meshDrawer->setPolylineUniformColor(Color(color));
        }

        updateView();
        vCanvas->updateGL();
    }
}

}

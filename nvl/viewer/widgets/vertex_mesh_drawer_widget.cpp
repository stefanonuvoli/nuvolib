#include "vertex_mesh_drawer_widget.h"
#include "ui_vertex_mesh_drawer_widget.h"

namespace nvl {

NVL_INLINE VertexMeshDrawerWidget::VertexMeshDrawerWidget(
        Canvas* canvas,
        DrawableListWidget* drawableListWidget,
        QWidget *parent) :
    QFrame(parent),
    ui(new Ui::VertexMeshDrawerWidget),
    vCanvas(canvas),
    vDrawableListWidget(drawableListWidget),
    vHandleUpdate(true)
{
    ui->setupUi(this);

    connectSignals();

    updateView();
}

NVL_INLINE VertexMeshDrawerWidget::~VertexMeshDrawerWidget()
{
    delete ui;
}

NVL_INLINE void VertexMeshDrawerWidget::slot_drawableSelectionChanged()
{
    getSelectedDrawers();

    updateView();
}

NVL_INLINE void VertexMeshDrawerWidget::slot_selectedDrawableUpdated()
{
    updateView();
}

NVL_INLINE void VertexMeshDrawerWidget::updateView()
{
    vHandleUpdate = false;

    updateVertexView();

    vHandleUpdate = true;
}

NVL_INLINE void VertexMeshDrawerWidget::updateVertexView()
{
    assert(!vHandleUpdate);

    bool vertexEnabled = !vVertexMeshDrawers.empty() && vCanvas != nullptr;

    bool vertexColorEnabled = vertexEnabled;
    bool vertexTransparencyEnabled = vertexEnabled;
    bool vertexNormalEnabled = vertexEnabled;

    bool vertexVisible = vertexEnabled;
    bool vertexTransparency = vertexEnabled;
    int vertexSize = 1;
    bool pointSelected = vertexEnabled;
    bool dotSelected = vertexEnabled;
    bool vertexColorSelected = vertexEnabled;
    bool uniformColorSelected = vertexEnabled;
    Color vertexUniformColor(255,255,255);
    bool normalsVisible = vertexEnabled;
    int normalSize = 1;

    if (vertexEnabled) {
        bool first = true;

        for (VertexMeshDrawerBase* meshDrawer : vVertexMeshDrawers) {
            vertexColorEnabled &= meshDrawer->hasVertexColors();
            vertexColorEnabled &= meshDrawer->hasVertexColors();
            vertexNormalEnabled &= meshDrawer->hasVertexNormals();
            vertexTransparencyEnabled &= meshDrawer->hasVertexTransparency();

            vertexVisible &= meshDrawer->vertexVisible();
            vertexTransparency &= vertexTransparencyEnabled && meshDrawer->vertexTransparency();
            pointSelected &= meshDrawer->vertexShapeMode() == VertexMeshDrawerBase::VertexShapeMode::VERTEX_SHAPE_POINT;
            dotSelected &= meshDrawer->vertexShapeMode() == VertexMeshDrawerBase::VertexShapeMode::VERTEX_SHAPE_DOT;
            vertexColorSelected &= vertexColorEnabled && meshDrawer->vertexColorMode() == VertexMeshDrawerBase::VertexColorMode::VERTEX_COLOR_PER_VERTEX;
            vertexColorSelected &= vertexColorEnabled && meshDrawer->vertexColorMode() == VertexMeshDrawerBase::VertexColorMode::VERTEX_COLOR_PER_VERTEX;
            uniformColorSelected &= meshDrawer->vertexColorMode() == VertexMeshDrawerBase::VertexColorMode::VERTEX_COLOR_UNIFORM;
            vertexUniformColor = (first || vertexUniformColor == meshDrawer->vertexUniformColor() ? meshDrawer->vertexUniformColor() : Color(255,255,255));
            vertexSize = (first || vertexSize == meshDrawer->vertexSize() ? meshDrawer->vertexSize() : 1);
            normalsVisible &= vertexNormalEnabled && meshDrawer->vertexNormalsVisible();
            normalSize = (first || normalSize == meshDrawer->vertexNormalSize() ? meshDrawer->vertexNormalSize() : 1);

            first = false;
        }
    }

    ui->vertexTransparencyCheckBox->setEnabled(vertexTransparencyEnabled);
    ui->vertexColorVertexButton->setEnabled(vertexColorEnabled);
    ui->vertexNormalsSlider->setEnabled(vertexNormalEnabled);
    ui->vertexNormalsCheckBox->setEnabled(vertexNormalEnabled);

    this->setEnabled(vertexEnabled);
    ui->vertexVisibleCheckBox->setChecked(vertexVisible);
    ui->vertexTransparencyCheckBox->setChecked(vertexTransparency);
    ui->vertexShapePointButton->setChecked(pointSelected);
    ui->vertexShapeDotButton->setChecked(dotSelected);
    ui->vertexSizeSlider->setValue(vertexSize);
    ui->vertexColorVertexButton->setChecked(vertexColorSelected);
    ui->vertexColorVertexButton->setChecked(vertexColorSelected);
    ui->vertexColorUniformButton->setChecked(uniformColorSelected);
    ui->vertexColorUniformColorButton->setColor(vertexUniformColor.toQColor());
    ui->vertexNormalsCheckBox->setChecked(normalsVisible);
    ui->vertexNormalsSlider->setValue(normalSize);
}

NVL_INLINE void VertexMeshDrawerWidget::getSelectedDrawers()
{
    vVertexMeshDrawers.clear();

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

            VertexMeshDrawerBase* vertexMeshDrawer = dynamic_cast<VertexMeshDrawerBase*>(meshDrawerWrapper);
            if (vertexMeshDrawer != nullptr) {
                vVertexMeshDrawers.push_back(vertexMeshDrawer);
            }
        }
    }
}

NVL_INLINE void VertexMeshDrawerWidget::connectSignals()
{
    if (vCanvas != nullptr) {
        //Connect signals to the viewer
        connect(vDrawableListWidget, &DrawableListWidget::signal_drawableSelectionChanged, this, &VertexMeshDrawerWidget::slot_drawableSelectionChanged);
    }
}

NVL_INLINE void VertexMeshDrawerWidget::on_vertexVisibleCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (VertexMeshDrawerBase* meshDrawer : vVertexMeshDrawers) {
            meshDrawer->setVertexVisible(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void VertexMeshDrawerWidget::on_vertexTransparencyCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (VertexMeshDrawerBase* meshDrawer : vVertexMeshDrawers) {
            meshDrawer->setVertexTransparency(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}


NVL_INLINE void VertexMeshDrawerWidget::on_vertexShapePointButton_clicked()
{
    if (vHandleUpdate) {
        for (VertexMeshDrawerBase* meshDrawer : vVertexMeshDrawers) {
            meshDrawer->setVertexShapeMode(VertexMeshDrawerBase::VertexShapeMode::VERTEX_SHAPE_POINT);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void VertexMeshDrawerWidget::on_vertexShapeDotButton_clicked()
{
    if (vHandleUpdate) {
        for (VertexMeshDrawerBase* meshDrawer : vVertexMeshDrawers) {
            meshDrawer->setVertexShapeMode(VertexMeshDrawerBase::VertexShapeMode::VERTEX_SHAPE_DOT);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void VertexMeshDrawerWidget::on_vertexSizeSlider_valueChanged(int value)
{
    if (vHandleUpdate) {
        for (VertexMeshDrawerBase* meshDrawer : vVertexMeshDrawers) {
            meshDrawer->setVertexSize(value);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void VertexMeshDrawerWidget::on_vertexColorVertexButton_clicked()
{
    if (vHandleUpdate) {
        for (VertexMeshDrawerBase* meshDrawer : vVertexMeshDrawers) {
            meshDrawer->setVertexColorMode(VertexMeshDrawerBase::VertexColorMode::VERTEX_COLOR_PER_VERTEX);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void VertexMeshDrawerWidget::on_vertexColorUniformButton_clicked()
{
    if (vHandleUpdate) {
        for (VertexMeshDrawerBase* meshDrawer : vVertexMeshDrawers) {
            meshDrawer->setVertexColorMode(VertexMeshDrawerBase::VertexColorMode::VERTEX_COLOR_UNIFORM);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void VertexMeshDrawerWidget::on_vertexColorUniformColorButton_colorChanged(QColor color)
{
    if (vHandleUpdate) {
        for (VertexMeshDrawerBase* meshDrawer : vVertexMeshDrawers) {
            meshDrawer->setVertexUniformColor(Color(color));
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void VertexMeshDrawerWidget::on_vertexNormalsCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (VertexMeshDrawerBase* meshDrawer : vVertexMeshDrawers) {
            meshDrawer->setVertexNormalVisible(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void VertexMeshDrawerWidget::on_vertexNormalsSlider_valueChanged(int value)
{
    if (vHandleUpdate) {
        for (VertexMeshDrawerBase* meshDrawer : vVertexMeshDrawers) {
            meshDrawer->setVertexNormalSize(value);
        }

        updateView();
        vCanvas->updateGL();
    }
}


}

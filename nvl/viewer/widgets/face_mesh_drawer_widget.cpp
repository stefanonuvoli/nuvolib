/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#include "face_mesh_drawer_widget.h"

#ifdef NVL_QT
#ifdef NVL_OPENGL_LOADED

#include "ui_face_mesh_drawer_widget.h"

#include <nvl/models/structures/model.h>

namespace nvl {

NVL_INLINE FaceMeshDrawerWidget::FaceMeshDrawerWidget(
        QCanvas* canvas,
        DrawableListWidget* drawableListWidget,
        QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FaceMeshDrawerWidget),
    vCanvas(canvas),
    vDrawableListWidget(drawableListWidget),
    vHandleUpdate(true)
{
    ui->setupUi(this);

    connectSignals();

    updateView();
}

NVL_INLINE FaceMeshDrawerWidget::~FaceMeshDrawerWidget()
{
    delete ui;
}

NVL_INLINE void FaceMeshDrawerWidget::slot_drawableSelectionChanged()
{
    getSelectedDrawers();

    updateView();
}

NVL_INLINE void FaceMeshDrawerWidget::slot_selectedDrawableUpdated()
{
    updateView();
}

NVL_INLINE void FaceMeshDrawerWidget::updateView()
{
    vHandleUpdate = false;

    updateFaceView();

    vHandleUpdate = true;
}

NVL_INLINE void FaceMeshDrawerWidget::updateFaceView()
{
    assert(!vHandleUpdate);

    bool faceEnabled = !vFaceMeshDrawers.empty() && vCanvas != nullptr;

    bool textureEnabled = faceEnabled;
    bool faceLightingEnabled = faceEnabled;
    bool faceTransparencyEnabled = faceEnabled;
    bool smoothShadingEnabled = faceEnabled;
    bool flatShadingEnabled = faceEnabled;
    bool faceColorEnabled = faceEnabled;
    bool vertexColorEnabled = faceEnabled;
    bool faceNormalEnabled = faceEnabled;

    bool faceVisible = faceEnabled;
    bool textureVisible = faceEnabled;
    bool faceLighting = faceEnabled;
    bool faceTransparency = faceEnabled;
    bool wireframeVisible = faceEnabled;
    int wireframeSize = 1;
    Color wireframeColor(255,255,255);
    bool smoothSelected = faceEnabled;
    bool flatSelected = faceEnabled;
    bool faceColorSelected = faceEnabled;
    bool vertexColorSelected = faceEnabled;
    bool uniformColorSelected = faceEnabled;
    Color faceUniformColor(255,255,255);
    bool normalsVisible = faceEnabled;
    int normalSize = 1;
    int textureModeSelected = FaceMeshDrawerBase::TEXTURE_MODE_MODULATE;
    bool faceShaderVisible = faceEnabled;

    if (faceEnabled) {
        bool first = true;

        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            smoothShadingEnabled &= meshDrawer->hasSmoothShading();
            flatShadingEnabled &= meshDrawer->hasFlatShading();
            faceColorEnabled &= meshDrawer->hasFaceColors();
            vertexColorEnabled &= meshDrawer->hasVertexColors();
            faceNormalEnabled &= meshDrawer->hasFaceNormals();
            textureEnabled &= meshDrawer->hasTextures();
            faceLightingEnabled &= meshDrawer->hasFaceLighting();
            faceTransparencyEnabled &= meshDrawer->hasFaceTransparency();

            faceVisible &= meshDrawer->faceVisible();
            wireframeVisible &= meshDrawer->wireframeVisible();
            wireframeSize = (first || wireframeSize == meshDrawer->wireframeSize() ? meshDrawer->wireframeSize() : 1);
            wireframeColor = (first || wireframeColor == meshDrawer->wireframeColor() ? meshDrawer->wireframeColor() : Color(255,255,255));
            smoothSelected &= meshDrawer->faceShadingMode() == FaceMeshDrawerBase::FaceShadingMode::FACE_SHADING_SMOOTH;
            flatSelected &= meshDrawer->faceShadingMode() == FaceMeshDrawerBase::FaceShadingMode::FACE_SHADING_FLAT;
            faceColorSelected &= faceColorEnabled && meshDrawer->faceColorMode() == FaceMeshDrawerBase::FaceColorMode::FACE_COLOR_PER_FACE;
            vertexColorSelected &= vertexColorEnabled && meshDrawer->faceColorMode() == FaceMeshDrawerBase::FaceColorMode::FACE_COLOR_PER_VERTEX;
            uniformColorSelected &= meshDrawer->faceColorMode() == FaceMeshDrawerBase::FaceColorMode::FACE_COLOR_UNIFORM;
            faceUniformColor = (first || faceUniformColor == meshDrawer->faceUniformColor() ? meshDrawer->faceUniformColor() : Color(255,255,255));
            normalsVisible &= faceNormalEnabled && meshDrawer->faceNormalVisible();
            normalSize = (first || normalSize == meshDrawer->faceNormalSize() ? meshDrawer->faceNormalSize() : 1);
            textureVisible &= textureEnabled && meshDrawer->textureVisible();
            faceTransparency &= faceTransparencyEnabled && meshDrawer->faceTransparency();
            faceLighting &= faceTransparencyEnabled && meshDrawer->faceLighting();
            faceShaderVisible &= meshDrawer->faceShaderVisible();

            if (meshDrawer->textureMode() == FaceMeshDrawerBase::TEXTURE_MODE_MODULATE) {
                textureModeSelected = 0;
            }
            else {
                assert(meshDrawer->textureMode() == FaceMeshDrawerBase::TEXTURE_MODE_REPLACE);
                textureModeSelected = 1;
            }

            first = false;
        }
    }

    this->setEnabled(faceEnabled);
    ui->faceColorFaceButton->setEnabled(faceColorEnabled);
    ui->faceColorVertexButton->setEnabled(vertexColorEnabled);
    ui->faceNormalsSlider->setEnabled(faceNormalEnabled);
    ui->faceNormalsCheckBox->setEnabled(faceNormalEnabled);
    ui->faceLightingCheckBox->setEnabled(faceLightingEnabled);
    ui->faceTransparencyCheckBox->setEnabled(faceTransparencyEnabled);
    ui->faceTextureVisibleCheckBox->setEnabled(textureEnabled);
    ui->faceTextureModeComboBox->setEnabled(textureEnabled);
    ui->faceShadingSmoothButton->setEnabled(smoothShadingEnabled);
    ui->faceShadingFlatButton->setEnabled(flatShadingEnabled);

    ui->faceVisibleCheckBox->setChecked(faceVisible);
    ui->faceWireframeCheckBox->setChecked(wireframeVisible);
    ui->faceWireframeSlider->setValue(wireframeSize);
    ui->faceWireframeColorButton->setColor(wireframeColor.toQColor());
    ui->faceShadingSmoothButton->setChecked(smoothSelected);
    ui->faceShadingFlatButton->setChecked(flatSelected);
    ui->faceColorFaceButton->setChecked(faceColorSelected);
    ui->faceColorVertexButton->setChecked(vertexColorSelected);
    ui->faceColorUniformButton->setChecked(uniformColorSelected);
    ui->faceColorUniformColorButton->setColor(faceUniformColor.toQColor());
    ui->faceNormalsCheckBox->setChecked(normalsVisible);
    ui->faceNormalsSlider->setValue(normalSize);
    ui->faceLightingCheckBox->setChecked(faceLighting);
    ui->faceTransparencyCheckBox->setChecked(faceTransparency);
    ui->faceTextureVisibleCheckBox->setChecked(textureVisible);
    ui->faceTextureModeComboBox->setCurrentIndex(textureModeSelected);
    ui->faceShaderCheckBox->setChecked(faceShaderVisible);
}

NVL_INLINE void FaceMeshDrawerWidget::getSelectedDrawers()
{
    vFaceMeshDrawers.clear();

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

            FaceMeshDrawerBase* faceMeshDrawer = dynamic_cast<FaceMeshDrawerBase*>(meshDrawerWrapper);
            if (faceMeshDrawer != nullptr) {
                vFaceMeshDrawers.push_back(faceMeshDrawer);
            }
        }
    }
}

NVL_INLINE void FaceMeshDrawerWidget::connectSignals()
{
    if (vCanvas != nullptr) {
        //Connect signals to the viewer
        connect(vDrawableListWidget, &DrawableListWidget::signal_drawableSelectionChanged, this, &FaceMeshDrawerWidget::slot_drawableSelectionChanged);
    }
}

NVL_INLINE void FaceMeshDrawerWidget::on_faceVisibleCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceVisible(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}


NVL_INLINE void FaceMeshDrawerWidget::on_faceTextureVisibleCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setTextureVisible(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void FaceMeshDrawerWidget::on_faceShadingSmoothButton_clicked()
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceShadingMode(FaceMeshDrawerBase::FaceShadingMode::FACE_SHADING_SMOOTH);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void FaceMeshDrawerWidget::on_faceShadingFlatButton_clicked()
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceShadingMode(FaceMeshDrawerBase::FaceShadingMode::FACE_SHADING_FLAT);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void FaceMeshDrawerWidget::on_faceTransparencyCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceTransparency(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void FaceMeshDrawerWidget::on_faceColorFaceButton_clicked()
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceColorMode(FaceMeshDrawerBase::FaceColorMode::FACE_COLOR_PER_FACE);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void FaceMeshDrawerWidget::on_faceColorVertexButton_clicked()
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceColorMode(FaceMeshDrawerBase::FaceColorMode::FACE_COLOR_PER_VERTEX);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void FaceMeshDrawerWidget::on_faceColorUniformButton_clicked()
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceColorMode(FaceMeshDrawerBase::FaceColorMode::FACE_COLOR_UNIFORM);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void FaceMeshDrawerWidget::on_faceColorUniformColorButton_colorChanged(QColor color)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceUniformColor(Color(color));
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void FaceMeshDrawerWidget::on_faceWireframeCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setWireframeVisible(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void FaceMeshDrawerWidget::on_faceWireframeSlider_valueChanged(int value)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setWireframeSize(value);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void FaceMeshDrawerWidget::on_faceWireframeColorButton_colorChanged(QColor color)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setWireframeColor(Color(color));
            meshDrawer->update();
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void FaceMeshDrawerWidget::on_faceNormalsCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceNormalVisible(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void FaceMeshDrawerWidget::on_faceNormalsSlider_valueChanged(int value)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceNormalSize(value);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void FaceMeshDrawerWidget::on_faceLightingCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceLighting(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void FaceMeshDrawerWidget::on_faceTextureModeComboBox_currentIndexChanged(int index)
{
    if (vHandleUpdate) {
        FaceMeshDrawerBase::TextureMode textureMode;
        if (index == 0) {
            textureMode = FaceMeshDrawerBase::TEXTURE_MODE_MODULATE;
        }
        else {
            assert(index == 1);
            textureMode = FaceMeshDrawerBase::TEXTURE_MODE_REPLACE;
        }

        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setTextureMode(textureMode);
            meshDrawer->update();
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void FaceMeshDrawerWidget::on_faceShaderCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceShaderVisible(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

}

#endif
#endif

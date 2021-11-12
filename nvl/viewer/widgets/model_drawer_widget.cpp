#include "model_drawer_widget.h"
#include "ui_model_drawer_widget.h"

namespace nvl {

NVL_INLINE ModelDrawerWidget::ModelDrawerWidget(
        Canvas* canvas,
        DrawableListWidget* drawableListWidget,
        QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::ModelDrawerWidget),
    vCanvas(canvas),
    vDrawableListWidget(drawableListWidget),
    vHandleUpdate(true)
{
    ui->setupUi(this);

    connectSignals();

    updateView();
}

NVL_INLINE ModelDrawerWidget::~ModelDrawerWidget()
{
    rampQGLShader.unload();
    contourQGLShader.unload();

    delete ui;
}

NVL_INLINE void ModelDrawerWidget::slot_drawableSelectionChanged()
{
    getSelectedDrawers();

    updateView();
}

NVL_INLINE void ModelDrawerWidget::slot_selectedDrawableUpdated()
{
    updateView();
}

NVL_INLINE void ModelDrawerWidget::updateView()
{
    vHandleUpdate = false;

    updateFaceView();
    updatePolylineView();
    updateVertexView();
    updateSkeletonView();

    vHandleUpdate = true;
}

NVL_INLINE void ModelDrawerWidget::updateFaceView()
{
    assert(!vHandleUpdate);

    bool tabEnabled = !vFaceMeshDrawers.empty() && vCanvas != nullptr;
    bool faceVisibleChecked = tabEnabled;
    bool textureVisibleChecked = tabEnabled;
    bool normalsVisibleChecked = tabEnabled;
    bool wireframeVisibleChecked = tabEnabled;
    int wireframeSize = 1;
    Color wireframeColor(255,255,255);
    bool smoothSelected = tabEnabled;
    bool flatSelected = tabEnabled;
    bool faceColorSelected = tabEnabled;
    bool vertexColorSelected = tabEnabled;
    bool uniformColorSelected = tabEnabled;
    int faceNormalSize = 1;
    Color faceUniformColor(255,255,255);
    int shaderSelected = ShaderOptions::SHADER_NONE;
    bool transparencyChecked = tabEnabled;

    if (tabEnabled) {
        bool first = true;

        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            faceVisibleChecked &= meshDrawer->faceVisible();
            textureVisibleChecked &= meshDrawer->textureVisible();
            normalsVisibleChecked &= meshDrawer->faceNormalVisible();
            wireframeVisibleChecked &= meshDrawer->wireframeVisible();
            wireframeSize = (first || wireframeSize == meshDrawer->wireframeSize() ? meshDrawer->wireframeSize() : 1);
            wireframeColor = (first || wireframeColor == meshDrawer->wireframeColor() ? meshDrawer->wireframeColor() : Color(255,255,255));
            smoothSelected &= meshDrawer->faceShadingMode() == FaceMeshDrawerBase::FaceShadingMode::FACE_SHADING_SMOOTH;
            flatSelected &= meshDrawer->faceShadingMode() == FaceMeshDrawerBase::FaceShadingMode::FACE_SHADING_FLAT;
            faceColorSelected &= meshDrawer->faceColorMode() == FaceMeshDrawerBase::FaceColorMode::FACE_COLOR_PER_FACE;
            vertexColorSelected &= meshDrawer->faceColorMode() == FaceMeshDrawerBase::FaceColorMode::FACE_COLOR_PER_VERTEX;
            uniformColorSelected &= meshDrawer->faceColorMode() == FaceMeshDrawerBase::FaceColorMode::FACE_COLOR_UNIFORM;
            faceUniformColor = (first || faceUniformColor == meshDrawer->faceUniformColor() ? meshDrawer->faceUniformColor() : Color(255,255,255));
            faceNormalSize = (first || wireframeSize == meshDrawer->faceNormalSize() ? meshDrawer->faceNormalSize() : 1);
            transparencyChecked &= meshDrawer->faceTransparency();

            if (meshDrawer->faceShaderMode() == FaceMeshDrawerBase::FaceShaderMode::FACE_SHADER_NONE) {
                shaderSelected = ShaderOptions::SHADER_NONE;
            }
            else {
                assert(meshDrawer->faceShaderMode() == FaceMeshDrawerBase::FaceShaderMode::FACE_SHADER_VERTEX_VALUE);

                RampQGLShader* rampShader = dynamic_cast<RampQGLShader*>(meshDrawer->faceShader());
                if (rampShader != nullptr) {
                    shaderSelected = (first || shaderSelected == ShaderOptions::SHADER_RAMP ? ShaderOptions::SHADER_RAMP : ShaderOptions::SHADER_NONE);
                }
                else {
                    ContourQGLShader* rampShader = dynamic_cast<ContourQGLShader*>(meshDrawer->faceShader());
                    if (rampShader != nullptr) {
                        shaderSelected = (first || shaderSelected == ShaderOptions::SHADER_CONTOUR ? ShaderOptions::SHADER_CONTOUR : ShaderOptions::SHADER_NONE);
                    }
                }
            }

            first = false;
        }
    }

    ui->faceTab->setEnabled(tabEnabled);
    ui->faceNormalsCheckBox->setChecked(normalsVisibleChecked);
    ui->faceVisibleCheckBox->setChecked(faceVisibleChecked);
    ui->faceTextureVisibleCheckBox->setChecked(textureVisibleChecked);
    ui->faceWireframeCheckBox->setChecked(wireframeVisibleChecked);
    ui->faceWireframeSlider->setValue(wireframeSize);
    ui->faceWireframeColorButton->setColor(wireframeColor.toQColor());
    ui->faceShadingSmoothButton->setChecked(smoothSelected);
    ui->faceShadingFlatButton->setChecked(flatSelected);
    ui->faceColorFaceButton->setChecked(faceColorSelected);
    ui->faceColorVertexButton->setChecked(vertexColorSelected);
    ui->faceColorUniformButton->setChecked(uniformColorSelected);
    ui->faceColorUniformColorButton->setColor(faceUniformColor.toQColor());
    ui->faceShaderComboBox->setCurrentIndex(shaderSelected);
    ui->faceNormalsSlider->setValue(faceNormalSize);
    ui->faceTransparencyCheckBox->setChecked(transparencyChecked);
}

NVL_INLINE void ModelDrawerWidget::updatePolylineView()
{
    assert(!vHandleUpdate);
    //TODO
}

NVL_INLINE void ModelDrawerWidget::updateVertexView()
{
    assert(!vHandleUpdate);
    //TODO
}

NVL_INLINE void ModelDrawerWidget::updateSkeletonView()
{
    assert(!vHandleUpdate);
    bool tabEnabled = !vSkeletonDrawers.empty() && vCanvas != nullptr;
    bool skeletonVisibleChecked = tabEnabled;
    bool jointVisibleChecked = tabEnabled;
    bool boneVisibleChecked = tabEnabled;
    int jointSize = 1;
    int boneSize = 1;
    Color jointColor(255,255,255);
    Color rootColor(255,255,255);
    Color boneColor(255,255,255);
    bool transparencyChecked = tabEnabled;

    if (tabEnabled) {
        bool first = true;

        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonVisibleChecked &= skeletonDrawer->skeletonVisible();
            jointVisibleChecked &= skeletonDrawer->jointVisible();
            boneVisibleChecked &= skeletonDrawer->boneVisible();
            jointSize = (first || jointSize == skeletonDrawer->jointSize() ? skeletonDrawer->jointSize() : 1);
            boneSize = (first || boneSize == skeletonDrawer->boneSize() ? skeletonDrawer->boneSize() : 1);
            jointColor = (first || jointColor == skeletonDrawer->jointColor() ? skeletonDrawer->jointColor() : Color(255,255,255));
            rootColor = (first || rootColor == skeletonDrawer->rootColor() ? skeletonDrawer->rootColor() : Color(255,255,255));
            boneColor = (first || boneColor == skeletonDrawer->boneColor() ? skeletonDrawer->boneColor() : Color(255,255,255));
            transparencyChecked &= skeletonDrawer->transparency();

            first = false;
        }
    }

    ui->skeletonTab->setEnabled(tabEnabled);
    ui->skeletonVisibleCheckBox->setChecked(skeletonVisibleChecked);
    ui->skeletonJointVisibleCheckBox->setChecked(jointVisibleChecked);
    ui->skeletonBoneVisibleCheckBox->setChecked(boneVisibleChecked);
    ui->skeletonJointSlider->setValue(jointSize);
    ui->skeletonBoneSlider->setValue(boneSize);
    ui->skeletonJointColorButton->setColor(jointColor.toQColor());
    ui->skeletonRootColorButton->setColor(rootColor.toQColor());
    ui->skeletonBoneColorButton->setColor(boneColor.toQColor());
    ui->skeletonTransparencyCheckBox->setChecked(transparencyChecked);
}

NVL_INLINE void ModelDrawerWidget::getSelectedDrawers()
{
    vFaceMeshDrawers.clear();
    vPolylineMeshDrawers.clear();
    vVertexMeshDrawers.clear();
    vSkeletonDrawers.clear();

    if (vCanvas != nullptr) {
        for (const Index drawableId : vDrawableListWidget->selectedDrawables()) {
            Drawable* drawable = vCanvas->drawable(drawableId);

            Drawable* meshDrawerWrapper;
            Drawable* skeletonDrawerWrapper;
            ModelDrawer<Model3d>* modelDrawer = dynamic_cast<ModelDrawer<Model3d>*>(drawable);
            if (modelDrawer != nullptr) {
                meshDrawerWrapper = &modelDrawer->meshDrawer();
                skeletonDrawerWrapper = &modelDrawer->skeletonDrawer();
            }
            else {
                meshDrawerWrapper = drawable;
                skeletonDrawerWrapper = drawable;
            }

            FaceMeshDrawerBase* faceMeshDrawer = dynamic_cast<FaceMeshDrawerBase*>(meshDrawerWrapper);
            if (faceMeshDrawer != nullptr) {
                vFaceMeshDrawers.push_back(faceMeshDrawer);
            }
            PolylineMeshDrawerBase* polylineMeshDrawer = dynamic_cast<PolylineMeshDrawerBase*>(meshDrawerWrapper);
            if (polylineMeshDrawer != nullptr) {
                vPolylineMeshDrawers.push_back(polylineMeshDrawer);
            }
            VertexMeshDrawerBase* vertexMeshDrawer = dynamic_cast<VertexMeshDrawerBase*>(meshDrawerWrapper);
            if (vertexMeshDrawer != nullptr) {
                vVertexMeshDrawers.push_back(vertexMeshDrawer);
            }
            SkeletonDrawerBase* skeletonDrawer = dynamic_cast<SkeletonDrawerBase*>(skeletonDrawerWrapper);
            if (skeletonDrawer != nullptr) {
                vSkeletonDrawers.push_back(skeletonDrawer);
            }
        }
    }
}

NVL_INLINE void ModelDrawerWidget::connectSignals()
{
    if (vCanvas != nullptr) {
        //Connect signals to the viewer
        connect(vDrawableListWidget, &DrawableListWidget::signal_drawableSelectionChanged, this, &ModelDrawerWidget::slot_drawableSelectionChanged);
    }
}

NVL_INLINE void ModelDrawerWidget::on_faceVisibleCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceVisible(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}


NVL_INLINE void ModelDrawerWidget::on_faceTextureVisibleCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setTextureVisible(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelDrawerWidget::on_faceShadingSmoothButton_clicked()
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceShadingMode(FaceMeshDrawerBase::FaceShadingMode::FACE_SHADING_SMOOTH);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelDrawerWidget::on_faceShadingFlatButton_clicked()
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceShadingMode(FaceMeshDrawerBase::FaceShadingMode::FACE_SHADING_FLAT);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelDrawerWidget::on_faceTransparencyCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceTransparency(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelDrawerWidget::on_faceColorFaceButton_clicked()
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceColorMode(FaceMeshDrawerBase::FaceColorMode::FACE_COLOR_PER_FACE);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelDrawerWidget::on_faceColorVertexButton_clicked()
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceColorMode(FaceMeshDrawerBase::FaceColorMode::FACE_COLOR_PER_VERTEX);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelDrawerWidget::on_faceColorUniformButton_clicked()
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceColorMode(FaceMeshDrawerBase::FaceColorMode::FACE_COLOR_UNIFORM);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelDrawerWidget::on_faceColorUniformColorButton_colorChanged(QColor color)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceUniformColor(Color(color));
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelDrawerWidget::on_faceWireframeCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setWireframeVisible(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelDrawerWidget::on_faceWireframeSlider_valueChanged(int value)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setWireframeSize(value);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelDrawerWidget::on_faceWireframeColorButton_colorChanged(QColor color)
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

NVL_INLINE void ModelDrawerWidget::on_faceNormalsCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceNormalVisible(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelDrawerWidget::on_faceNormalsSlider_valueChanged(int value)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            meshDrawer->setFaceNormalSize(value);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelDrawerWidget::on_faceShaderComboBox_currentIndexChanged(int index)
{
    if (vHandleUpdate) {
        for (FaceMeshDrawerBase* meshDrawer : vFaceMeshDrawers) {
            switch (index) {
                case ShaderOptions::SHADER_RAMP:
                    rampQGLShader.load(vCanvas->qglContext());
                    meshDrawer->setFaceShader(&rampQGLShader);
                    meshDrawer->setFaceShaderMode(FaceMeshDrawerBase::FaceShaderMode::FACE_SHADER_VERTEX_VALUE);
                    break;

                case ShaderOptions::SHADER_CONTOUR:
                    contourQGLShader.load(vCanvas->qglContext());
                    meshDrawer->setFaceShader(&contourQGLShader);
                    meshDrawer->setFaceShaderMode(FaceMeshDrawerBase::FaceShaderMode::FACE_SHADER_VERTEX_VALUE);
                    break;

                default:
                    assert(index == ShaderOptions::SHADER_NONE);
                    meshDrawer->setFaceShader(nullptr);
                    meshDrawer->setFaceShaderMode(FaceMeshDrawerBase::FaceShaderMode::FACE_SHADER_NONE);
                    break;
            }
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void nvl::ModelDrawerWidget::on_skeletonVisibleCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonDrawer->setSkeletonVisible(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelDrawerWidget::on_skeletonTransparencyCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonDrawer->setTransparency(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelDrawerWidget::on_skeletonJointVisibleCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonDrawer->setJointVisible(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelDrawerWidget::on_skeletonJointSlider_valueChanged(int value)
{
    if (vHandleUpdate) {
        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonDrawer->setJointSize(value);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelDrawerWidget::on_skeletonJointColorButton_colorChanged(QColor color)
{
    if (vHandleUpdate) {
        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonDrawer->setJointColor(Color(color));
            skeletonDrawer->update();
        }

        updateView();
        vCanvas->updateGL();
    }
}
NVL_INLINE void ModelDrawerWidget::on_skeletonRootColorButton_colorChanged(QColor color)
{
    if (vHandleUpdate) {
        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonDrawer->setRootColor(Color(color));
            skeletonDrawer->update();
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelDrawerWidget::on_skeletonBoneVisibleCheckBox_stateChanged(int arg1)
{
    if (vHandleUpdate) {
        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonDrawer->setBoneVisible(arg1 == Qt::Checked);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelDrawerWidget::on_skeletonBoneSlider_valueChanged(int value)
{
    if (vHandleUpdate) {
        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonDrawer->setBoneSize(value);
        }

        updateView();
        vCanvas->updateGL();
    }
}

NVL_INLINE void ModelDrawerWidget::on_skeletonBoneColorButton_colorChanged(QColor color)
{
    if (vHandleUpdate) {
        for (SkeletonDrawerBase* skeletonDrawer : vSkeletonDrawers) {
            skeletonDrawer->setBoneColor(Color(color));
            skeletonDrawer->update();
        }

        updateView();
        vCanvas->updateGL();
    }
}


}

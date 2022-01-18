#include "model_loader_widget.h"

#ifdef NVL_QT
#ifdef NVL_OPENGL_LOADED

#include "ui_model_loader_widget.h"

#include <nvl/models/io/model_io.h>

#include <nvl/models/algorithms/mesh_normals.h>

#include <QMessageBox>
#include <QFileDialog>

namespace nvl {

NVL_INLINE ModelLoaderWidget::ModelLoaderWidget(
        QCanvas* canvas,
        DrawableListWidget* drawableListWidget,
        QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ModelLoaderWidget),
    vCanvas(canvas),
    vDrawableListWidget(drawableListWidget)
{
    ui->setupUi(this);

    initialize();
    connectSignals();
}

NVL_INLINE ModelLoaderWidget::~ModelLoaderWidget()
{
    this->clear();

    delete ui;
}

NVL_INLINE Index ModelLoaderWidget::loadModel(Model3d* model, const std::string& name)
{
    ModelDrawer<Model3d>* modelDrawer = new ModelDrawer<Model3d>(model);
    modelDrawer->meshDrawer().setFaceColorMode(ModelDrawer<Model3d>::FaceColorMode::FACE_COLOR_PER_FACE);

    vLoadedDrawables.insert(modelDrawer);

    vModelMap.insert(std::make_pair(modelDrawer, model));

    return vCanvas->addDrawable(modelDrawer, name);
}

NVL_INLINE Index ModelLoaderWidget::loadPolygonMesh(PolygonMesh3d* mesh, const std::string& name)
{
    FaceMeshDrawer<PolygonMesh3d>* meshDrawer = new FaceMeshDrawer<PolygonMesh3d>(mesh);
    meshDrawer->setFaceColorMode(ModelDrawer<Model3d>::FaceColorMode::FACE_COLOR_PER_FACE);

    vLoadedDrawables.insert(meshDrawer);

    vPolygonMeshMap.insert(std::make_pair(meshDrawer, mesh));

    return vCanvas->addDrawable(meshDrawer, name);
}

NVL_INLINE Index ModelLoaderWidget::loadModel(const Model3d& model, const std::string& name)
{
    return loadModel(new Model3d(model), name);
}

NVL_INLINE Index ModelLoaderWidget::loadPolygonMesh(const PolygonMesh3d& mesh, const std::string& name)
{
    return loadPolygonMesh(new PolygonMesh3d(mesh), name);
}

NVL_INLINE Index ModelLoaderWidget::loadDrawableFromFile(const std::string& filename)
{
    std::string ext = stringToLower(filenameExtension(filename));

    if (ext == "rig" || ext == "fbx") {
        return loadModelFromFile(filename);
    }
    else if (ext == "obj") {
        return loadPolygonMeshFromFile(filename);
    }
    else {
        QMessageBox::warning(this, tr("Error"), tr("Error: cannot load the file, no supported extension!"));
        return NULL_ID;
    }
}

NVL_INLINE Index ModelLoaderWidget::loadPolygonMeshFromFile(const std::string& filename)
{
    PolygonMesh3d tmpMesh;

    bool success = meshLoadFromFile(filename, tmpMesh);

    if (success) {
        PolygonMesh3d* mesh = new PolygonMesh3d(tmpMesh);
        updateNormals(*mesh);

        std::string name = filenameName(filename);
        return loadPolygonMesh(mesh, name);
    }
    else {
        QMessageBox::warning(this, tr("Error"), tr("Error: impossible to load mesh!"));
        return NULL_ID;
    }
}

NVL_INLINE Index ModelLoaderWidget::loadModelFromFile(const std::string& filename)
{
    Model3d tmpModel;

    bool success = modelLoadFromFile(filename, tmpModel);

    if (success) {
        Model3d* model = new Model3d(tmpModel);
        updateNormals(model->mesh);

        std::string name = filenameName(filename);
        return loadModel(model, name);
    }
    else {
        QMessageBox::warning(this, tr("Error"), tr("Error: impossible to load model!"));
        return NULL_ID;
    }
}

NVL_INLINE void ModelLoaderWidget::slot_drawableSelectionChanged(const std::unordered_set<Skeleton3d::JointId>& selectedDrawables)
{
    NVL_SUPPRESS_UNUSEDVARIABLE(selectedDrawables);

    updateView();
}

NVL_INLINE void ModelLoaderWidget::on_drawableLoadButton_clicked()
{
    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFiles);
    QStringList filters;
    filters
            << "Model (*.rig *.RIG *.obj *.OBJ *.fbx *.FBX)"
            << "RIG (*.rig *.RIG)"
            << "FBX (*.fbx *.FBX)"
            << "OBJ (*.obj *.OBJ)"
            << "Any file (*)";
    dialog.setNameFilters(filters);

    QStringList files;

    if (dialog.exec()) {
        files = dialog.selectedFiles();
    }

    for (const QString& str : files) {
        loadDrawableFromFile(str.toStdString());
    }

    vCanvas->fitScene();
}

NVL_INLINE void ModelLoaderWidget::on_drawableRemoveButton_clicked()
{
    Index offset = 0;
    const std::unordered_set<Index>& selectedDrawables = vDrawableListWidget->selectedDrawables();
    while (selectedDrawables.size() > offset) {
        std::unordered_set<Index>::const_iterator it = selectedDrawables.begin();

        for (Index i = 0; i < offset; i++) {
            it++;
        }

        Index id = *it;

        Drawable* drawable = vCanvas->drawable(id);

        bool success = removeDrawable(drawable);
        if (!success) {
            offset++;
        }
    }

    vCanvas->updateGL();
}

NVL_INLINE void ModelLoaderWidget::on_drawableSaveButton_clicked()
{
    const std::unordered_set<Index>& selectedDrawables = vDrawableListWidget->selectedDrawables();

    for (const Index& id : selectedDrawables) {
        Drawable* drawable = vCanvas->drawable(id);

        std::unordered_map<Drawable*, Model3d*>::iterator modelIt = vModelMap.find(drawable);
        if (modelIt != vModelMap.end()) {

            QString filename = QFileDialog::getSaveFileName(this,
                tr("Save model"), QDir::homePath(),
                tr("Model (*.rig *.RIG *.obj *.OBJ *.fbx *.FBX);;RIG (*.rig *.RIG);;FBX (*.fbx *.FBX);;OBJ (*.obj *.OBJ);;Any file (*)")
            );

            if (!filename.isEmpty()) {
                const Model3d& model = *(modelIt->second);
                bool success = nvl::modelSaveToFile(filename.toStdString(), model);
                if (!success) {
                    QMessageBox::warning(this, tr("Error"), tr("Error: impossible to save model!"));
                }
            }
        }

        std::unordered_map<Drawable*, PolygonMesh3d*>::iterator polygonMeshIt = vPolygonMeshMap.find(drawable);
        if (polygonMeshIt != vPolygonMeshMap.end()) {

            QString filename = QFileDialog::getSaveFileName(this,
                tr("Save model"), QDir::homePath(),
                tr("Mesh (*.obj *.OBJ);;OBJ (*.obj *.OBJ);;Any file (*)")
            );

            if (!filename.isEmpty()) {
                const PolygonMesh3d& mesh = *(polygonMeshIt->second);
                bool success = nvl::meshSaveToFile(filename.toStdString(), mesh);
                if (!success) {
                    QMessageBox::warning(this, tr("Error"), tr("Error: impossible to save mesh!"));
                }
            }
        }
    }
}

template<class M>
NVL_INLINE void ModelLoaderWidget::updateNormals(M& mesh)
{
    if (ui->faceNormalsCheckBox->isChecked()) {
        mesh.computeFaceNormals();
    }
    if (ui->vertexNormalsCheckBox->isChecked()) {
        mesh.computeVertexNormals();
    }
}

NVL_INLINE void ModelLoaderWidget::clear() {
    while (!vLoadedDrawables.empty()) {
        Drawable* drawable = *(vLoadedDrawables.begin());
        removeDrawable(drawable);
    }
}

NVL_INLINE bool ModelLoaderWidget::removeDrawable(Drawable* drawable)
{
    std::unordered_set<Drawable*>::iterator it = vLoadedDrawables.find(drawable);

    if (it != vLoadedDrawables.end()) {
        vCanvas->removeDrawable(drawable);

        std::unordered_map<Drawable*, Model3d*>::iterator modelIt = vModelMap.find(drawable);
        if (modelIt != vModelMap.end()) {
            delete modelIt->second;
            vModelMap.erase(modelIt);
        }

        std::unordered_map<Drawable*, PolygonMesh3d*>::iterator polygonMeshIt = vPolygonMeshMap.find(drawable);
        if (polygonMeshIt != vPolygonMeshMap.end()) {
            delete polygonMeshIt->second;
            vPolygonMeshMap.erase(polygonMeshIt);
        }

        delete drawable;

        vLoadedDrawables.erase(*it);

        return true;
    }
    else {
        QMessageBox::warning(this, tr("Error"), tr("Impossible to remove a drawable object: the object was not added to the canvas by the loader."));

        return false;
    }
}

NVL_INLINE void ModelLoaderWidget::initialize()
{
    updateView();
}

NVL_INLINE void ModelLoaderWidget::connectSignals()
{
    if (vCanvas != nullptr) {
        //Connect signals to the viewer
        connect(vDrawableListWidget, &DrawableListWidget::signal_drawableSelectionChanged, this, &ModelLoaderWidget::slot_drawableSelectionChanged);
    }
}

NVL_INLINE void ModelLoaderWidget::updateView()
{
    bool drawableSelected = vDrawableListWidget->selectedDrawableNumber() > 0;

    ui->drawableLoadButton->setEnabled(true);
    ui->drawableRemoveButton->setEnabled(drawableSelected);
    ui->drawableSaveButton->setEnabled(drawableSelected);
}

}

#endif
#endif

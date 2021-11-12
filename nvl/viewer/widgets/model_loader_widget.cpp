#include "model_loader_widget.h"
#include "ui_model_loader_widget.h"

#include <nvl/models/model_io.h>

#include <nvl/models/mesh_normals.h>

#include <QMessageBox>
#include <QFileDialog>

namespace nvl {

NVL_INLINE ModelLoaderWidget::ModelLoaderWidget(
        nvl::Canvas* canvas,
        nvl::DrawableListWidget* drawableListWidget,
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

NVL_INLINE Index ModelLoaderWidget::loadModel(Model* model, const std::string& name)
{
    ModelDrawer* modelDrawer = new ModelDrawer(model);
    modelDrawer->meshDrawer().setFaceColorMode(ModelDrawer::FaceColorMode::FACE_COLOR_PER_FACE);

    vLoadedDrawables.insert(modelDrawer);

    vModelMap.insert(std::make_pair(modelDrawer, model));

    return vCanvas->addDrawable(modelDrawer, name);
}

NVL_INLINE Index ModelLoaderWidget::loadPolygonMesh(PolygonMesh* mesh, const std::string& name)
{
    PolygonMeshDrawer* meshDrawer = new PolygonMeshDrawer(mesh);
    meshDrawer->setFaceColorMode(ModelDrawer::FaceColorMode::FACE_COLOR_PER_FACE);

    vLoadedDrawables.insert(meshDrawer);

    vPolygonMeshMap.insert(std::make_pair(meshDrawer, mesh));

    return vCanvas->addDrawable(meshDrawer, name);
}

NVL_INLINE Index ModelLoaderWidget::loadModel(const Model& model, const std::string& name)
{
    return loadModel(new Model(model), name);
}

NVL_INLINE Index ModelLoaderWidget::loadPolygonMesh(const PolygonMesh& mesh, const std::string& name)
{
    return loadPolygonMesh(new PolygonMesh(mesh), name);
}

NVL_INLINE Index ModelLoaderWidget::loadDrawableFromFile(const std::string& filename)
{
    std::string ext = filenameExtension(filename);

    if (ext == "rig") {
        return loadModelFromFile(filename);
    }
    else if (ext == "obj") {
        return loadPolygonMeshFromFile(filename);
    }
    else {
        QMessageBox::warning(this, tr("Error"), tr("Error: cannot load the file, no supported extension!"));
        return nvl::MAX_INDEX;
    }
}

NVL_INLINE Index ModelLoaderWidget::loadPolygonMeshFromFile(const std::string& filename)
{
    PolygonMesh tmpMesh;

    bool success = nvl::meshLoadFromFile(filename, tmpMesh);

    if (success) {
        PolygonMesh* mesh = new PolygonMesh(tmpMesh);
        updateNormals(*mesh);

        std::string name = filenameName(filename);
        return loadPolygonMesh(mesh, name);
    }
    else {
        QMessageBox::warning(this, tr("Error"), tr("Error: impossible to load mesh!"));
        return nvl::MAX_INDEX;
    }
}

NVL_INLINE Index ModelLoaderWidget::loadModelFromFile(const std::string& filename)
{
    Model tmpModel;

    bool success = nvl::modelLoadFromFile(filename, tmpModel);

    if (success) {
        Model* model = new Model(tmpModel);
        updateNormals(model->mesh);

        std::string name = filenameName(filename);
        return loadModel(model, name);
    }
    else {
        QMessageBox::warning(this, tr("Error"), tr("Error: impossible to load model!"));
        return nvl::MAX_INDEX;
    }
}

NVL_INLINE void ModelLoaderWidget::slot_drawableSelectionChanged(const std::unordered_set<nvl::Skeleton3d::JointId>& selectedDrawables)
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
            << "Model (*.rig *.obj *.ply *.stl)"
            << "Any files (*)";
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
    //TODO
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

        std::unordered_map<Drawable*, Model*>::iterator modelIt = vModelMap.find(drawable);
        if (modelIt != vModelMap.end()) {
            delete modelIt->second;
            vModelMap.erase(modelIt);
        }

        std::unordered_map<Drawable*, PolygonMesh*>::iterator polygonMeshIt = vPolygonMeshMap.find(drawable);
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
        connect(vDrawableListWidget, &nvl::DrawableListWidget::signal_drawableSelectionChanged, this, &ModelLoaderWidget::slot_drawableSelectionChanged);
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

#ifndef NVL_MODEL_LOADER_WIDGET_H
#define NVL_MODEL_LOADER_WIDGET_H

#include <QFrame>

#include <nvl/viewer/widgets/canvas.h>
#include <nvl/viewer/widgets/drawable_list_widget.h>
#include <nvl/models/model.h>
#include <nvl/models/meshes.h>

#include <unordered_map>
#include <unordered_set>

namespace Ui {
class ModelLoaderWidget;
}

namespace nvl {

class ModelLoaderWidget : public QFrame
{
    Q_OBJECT

    typedef nvl::Model3d Model;
    typedef nvl::ModelDrawer<Model> ModelDrawer;
    typedef nvl::PolygonMesh3d PolygonMesh;
    typedef nvl::FaceMeshDrawer<PolygonMesh> PolygonMeshDrawer;


public:

    explicit ModelLoaderWidget(
            nvl::Canvas* canvas,
            nvl::DrawableListWidget* drawableListWidget,
            QWidget *parent = nullptr);
    ~ModelLoaderWidget();

    Index loadModel(Model* drawable, const std::string& name);
    Index loadPolygonMesh(PolygonMesh* drawable, const std::string& name);
    Index loadModel(const Model& drawable, const std::string& name);
    Index loadPolygonMesh(const PolygonMesh& drawable, const std::string& name);

    Index loadDrawableFromFile(const std::string& filename);
    Index loadModelFromFile(const std::string& filename);
    Index loadPolygonMeshFromFile(const std::string& filename);


public Q_SLOTS:

    void slot_drawableSelectionChanged(const std::unordered_set<nvl::Skeleton3d::JointId>& selectedDrawables);


private Q_SLOTS:

    void on_drawableRemoveButton_clicked();
    void on_drawableLoadButton_clicked();
    void on_drawableSaveButton_clicked();


private:

    template<class M>
    void updateNormals(M& mesh);

    void clear();
    bool removeDrawable(Drawable* drawable);

    void initialize();
    void connectSignals();

    void updateView();

    Ui::ModelLoaderWidget *ui;

    nvl::Canvas* vCanvas;
    nvl::DrawableListWidget* vDrawableListWidget;

    std::unordered_set<Drawable*> vLoadedDrawables;

    std::unordered_map<Drawable*, Model*> vModelMap;
    std::unordered_map<Drawable*, PolygonMesh*> vPolygonMeshMap;

};

}

#include "model_loader_widget.cpp"

#endif // NVL_MODEL_LOADER_WIDGET_H

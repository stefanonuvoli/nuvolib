#ifndef NVL_MODEL_LOADER_WIDGET_H
#define NVL_MODEL_LOADER_WIDGET_H

#include <QFrame>

#include <nvl/viewer/widgets/qcanvas.h>
#include <nvl/viewer/widgets/drawable_list_widget.h>

#include <nvl/models/model_3d.h>

#include <unordered_map>
#include <unordered_set>

namespace Ui {
class ModelLoaderWidget;
}

namespace nvl {

class ModelLoaderWidget : public QFrame
{
    Q_OBJECT


public:

    explicit ModelLoaderWidget(
            QCanvas* canvas,
            DrawableListWidget* drawableListWidget,
            QWidget *parent = nullptr);
    ~ModelLoaderWidget();

    Index loadModel(Model3d* drawable, const std::string& name);
    Index loadPolygonMesh(PolygonMesh3d* drawable, const std::string& name);
    Index loadModel(const Model3d& drawable, const std::string& name);
    Index loadPolygonMesh(const PolygonMesh3d& drawable, const std::string& name);

    Index loadDrawableFromFile(const std::string& filename);
    Index loadModelFromFile(const std::string& filename);
    Index loadPolygonMeshFromFile(const std::string& filename);


public Q_SLOTS:

    void slot_drawableSelectionChanged(const std::unordered_set<Skeleton3d::JointId>& selectedDrawables);


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

    QCanvas* vCanvas;
    DrawableListWidget* vDrawableListWidget;

    std::unordered_set<Drawable*> vLoadedDrawables;

    std::unordered_map<Drawable*, Model3d*> vModelMap;
    std::unordered_map<Drawable*, PolygonMesh3d*> vPolygonMeshMap;

};

}

#include "model_loader_widget.cpp"

#endif // NVL_MODEL_LOADER_WIDGET_H

#include "drawable_list_widget.h"
#include "ui_drawable_list_widget.h"

#include <nvl/viewer/drawables/model_drawer.h>
#include <nvl/models/model.h>

namespace nvl {

NVL_INLINE DrawableListWidget::DrawableListWidget(Canvas* canvas, QWidget* parent) :
    QFrame(parent),
    ui(new Ui::DrawableListWidget),
    vCanvas(canvas),
    vHandleUpdate(true)
{
    ui->setupUi(this);
    ui->drawableListTreeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

    connectSignals();

    updateDrawableList();
}

NVL_INLINE DrawableListWidget::~DrawableListWidget()
{
    delete ui;
}

NVL_INLINE Size DrawableListWidget::selectedDrawableNumber() const
{
    return vSelectedDrawables.size();
}

NVL_INLINE const std::unordered_set<Index>& DrawableListWidget::selectedDrawables() const
{
    return vSelectedDrawables;
}

NVL_INLINE void DrawableListWidget::setSelectedDrawables(const std::unordered_set<Index>& selectedDrawables)
{
    vSelectedDrawables = selectedDrawables;
    updateSelection();

    emit signal_drawableSelectionChanged(vSelectedDrawables);
}

NVL_INLINE bool DrawableListWidget::isDrawableSelected(const Index& id) const
{
    return vSelectedDrawables.find(id) != vSelectedDrawables.end();
}

NVL_INLINE void DrawableListWidget::selectDrawable(const Index& id)
{
    vSelectedDrawables.insert(id);
    updateSelection();

    emit signal_drawableSelectionChanged(vSelectedDrawables);
}

NVL_INLINE void DrawableListWidget::unselectDrawable(const Index& id)
{
    vSelectedDrawables.erase(id);
    updateSelection();

    emit signal_drawableSelectionChanged(vSelectedDrawables);
}

NVL_INLINE void DrawableListWidget::unselectAllDrawables()
{
    vSelectedDrawables.clear();
    updateSelection();

    emit signal_drawableSelectionChanged(vSelectedDrawables);
}

NVL_INLINE void DrawableListWidget::slot_drawableListChanged()
{
    updateDrawableList();
}

NVL_INLINE void DrawableListWidget::slot_drawableRemoved(Index id)
{
    std::unordered_set<Index> newSelectedDrawables;
    for (const Index& s : vSelectedDrawables) {
        if (s > id) {
            newSelectedDrawables.insert(s - 1);
        }
        else if (s < id) {
            newSelectedDrawables.insert(s);
        }
    }
    vSelectedDrawables = newSelectedDrawables;

    emit signal_drawableSelectionChanged(vSelectedDrawables);
}

NVL_INLINE void DrawableListWidget::updateDrawableList()
{
    vHandleUpdate = false;

    itemMap.clear();

    ui->drawableListTreeWidget->clear();

    if (vCanvas != nullptr) {
        ui->drawableListTreeWidget->setEnabled(true);

        Index itemId = 0;
        for (Index i = 0; i < vCanvas->drawableNumber(); ++i) {
            if (vCanvas->isListable(i)) {
                addTreeItem(vCanvas->drawable(i), vCanvas->drawableName(i));
                itemMap.push_back(i);

                if (vSelectedDrawables.find(i) != vSelectedDrawables.end()) {
                    ui->drawableListTreeWidget->topLevelItem(itemId)->setSelected(true);
                }

                itemId++;
            }
        }
    }
    else {
        ui->drawableListTreeWidget->setEnabled(false);
    }

    ui->drawableListTreeWidget->update();
    ui->drawableListTreeWidget->repaint();

    vHandleUpdate = true;
}

NVL_INLINE void DrawableListWidget::updateSelection()
{
    vHandleUpdate = false;

    if (vCanvas != nullptr) {
        Index itemId = 0;
        for (Index i = 0; i < vCanvas->drawableNumber(); ++i) {
            if (vCanvas->isListable(i)) {
                bool check = vSelectedDrawables.find(i) != vSelectedDrawables.end();
                ui->drawableListTreeWidget->topLevelItem(itemId)->setSelected(check);

                itemId++;
            }
        }

        ui->drawableListTreeWidget->update();
        ui->drawableListTreeWidget->repaint();
    }
    else {
        updateDrawableList();
    }

    vHandleUpdate = true;
}

NVL_INLINE QTreeWidgetItem* DrawableListWidget::addTreeItem(Drawable* drawable, const std::string& name)
{
    std::string type;
    if (dynamic_cast<ModelDrawer<Model3d>*>(drawable) != nullptr) {
        type = "Model";
    }
    else if (dynamic_cast<MeshDrawer<PolygonMesh3d>*>(drawable) != nullptr) {
        type = "Polygon mesh";
    }
    else if (dynamic_cast<MeshDrawer<TriangleMesh3d>*>(drawable) != nullptr) {
        type = "Triangle mesh";
    }
    else if (dynamic_cast<MeshDrawer<QuadMesh3d>*>(drawable) != nullptr) {
        type = "Quad mesh";
    }
    else if (dynamic_cast<MeshDrawer<PolylineMesh3d>*>(drawable) != nullptr) {
        type = "Polyline mesh";
    }
    else if (dynamic_cast<MeshDrawer<VertexMesh3d>*>(drawable) != nullptr) {
        type = "Vertex mesh";
    }
    else {
        type = "Object";
    }

    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, QString(type.c_str()));
    item->setText(1, QString(name.c_str()));
    ui->drawableListTreeWidget->addTopLevelItem(item);

    return item;
}

NVL_INLINE void DrawableListWidget::connectSignals()
{
    if (vCanvas != nullptr) {
        //Connect signals from the viewer
        connect(vCanvas, &Canvas::signal_drawableListChanged, this, &DrawableListWidget::slot_drawableListChanged);
        connect(vCanvas, &Canvas::signal_drawableRemoved, this, &DrawableListWidget::slot_drawableRemoved);
    }
}

NVL_INLINE void DrawableListWidget::on_drawableListTreeWidget_itemSelectionChanged()
{
    if (vHandleUpdate) {
        QList<QTreeWidgetItem*> items = ui->drawableListTreeWidget->selectedItems();

        std::unordered_set<Index> selectedItems;

        for (int i = 0; i < items.size(); ++i) {
            QTreeWidgetItem* item = items[i];
            int id = itemMap[ui->drawableListTreeWidget->indexOfTopLevelItem(item)];

            if (id >= 0) {
                selectedItems.insert(static_cast<Index>(id));
            }
        }

        vSelectedDrawables = selectedItems;
        emit signal_drawableSelectionChanged(vSelectedDrawables);
    }
}

}

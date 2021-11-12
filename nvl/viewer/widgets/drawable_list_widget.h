#ifndef NVL_VIEWER_DRAWABLE_LIST_WIDGET_H
#define NVL_VIEWER_DRAWABLE_LIST_WIDGET_H

#include <nvl/nuvolib.h>

#include <QFrame>
#include <QTreeWidgetItem>

#include <nvl/viewer/interfaces/drawable.h>

#include <nvl/viewer/widgets/canvas.h>

namespace Ui {
class DrawableListWidget;
}

namespace nvl {

class DrawableListWidget : public QFrame
{
    Q_OBJECT

public:

    explicit DrawableListWidget(Canvas* canvas, QWidget* parent = nullptr);
    ~DrawableListWidget();

    Size selectedDrawableNumber() const;
    const std::unordered_set<Index>& selectedDrawables() const;
    void setSelectedDrawables(const std::unordered_set<Index>& value);
    bool isDrawableSelected(const Index& id) const;
    void selectDrawable(const Index& id);
    void unselectDrawable(const Index& id);
    void unselectAllDrawables();

    void updateDrawableList();


Q_SIGNALS:

    void signal_drawableSelectionChanged(const std::unordered_set<Index>& selectedDrawables);


public Q_SLOTS:

    void slot_drawableListChanged();
    void slot_drawableRemoved(Index id);


private Q_SLOTS:

    void on_drawableListTreeWidget_itemSelectionChanged();


private:

    void updateSelection();

    QTreeWidgetItem* addTreeItem(Drawable* drawable, const std::string& name);

    void connectSignals();


    Ui::DrawableListWidget *ui;

    Canvas* vCanvas;

    std::vector<Index> itemMap;
    std::unordered_set<Index> vSelectedDrawables;

    bool vHandleUpdate;

};

}

#include "drawable_list_widget.cpp"

#endif // NVL_VIEWER_DRAWABLE_LIST_WIDGET_H

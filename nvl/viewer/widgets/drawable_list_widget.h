/*
 * This file is part of nuvolib: https://github.com/stefanonuvoli/nuvolib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 *
 * @author Stefano Nuvoli (stefano.nuvoli@gmail.com)
 */
#ifndef NVL_VIEWER_DRAWABLE_LIST_WIDGET_H
#define NVL_VIEWER_DRAWABLE_LIST_WIDGET_H

#include <nvl/nuvolib.h>

#ifdef NVL_QT
#ifdef NVL_OPENGL_LOADED

#include <QFrame>
#include <QTreeWidgetItem>

#include <nvl/viewer/interfaces/drawable.h>

#include <nvl/viewer/widgets/qcanvas.h>

namespace Ui {
class DrawableListWidget;
}

namespace nvl {

class DrawableListWidget : public QFrame
{
    Q_OBJECT

public:

    explicit DrawableListWidget(QCanvas* canvas, QWidget* parent = nullptr);
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

    QCanvas* vCanvas;

    std::vector<Index> itemMap;
    std::unordered_set<Index> vSelectedDrawables;

    bool vHandleUpdate;

};

}

#endif
#endif

#include "drawable_list_widget.cpp"

#endif // NVL_VIEWER_DRAWABLE_LIST_WIDGET_H

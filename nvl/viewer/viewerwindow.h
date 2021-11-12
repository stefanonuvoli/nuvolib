#ifndef NVL_VIEWER_VIEWERWINDOW_H
#define NVL_VIEWER_VIEWERWINDOW_H

#include <nvl/nuvolib.h>

#include <nvl/viewer/widgets/canvas.h>

#include <QMainWindow>

namespace Ui {
class ViewerWindow;
}

namespace nvl {

class ViewerWindow : public QMainWindow
{
    Q_OBJECT


public:

    explicit ViewerWindow(QWidget* parent = nullptr);
    ~ViewerWindow();

    void addCanvas(Canvas* canvas);
    void removeCanvas();
    Canvas* canvas();

    void showLeftDock();
    void showRightDock();
    void hideLeftDock();
    void hideRightDock();

    void addToLeftDock(QWidget* widget);
    void addToRightDock(QWidget* widget);


private:

    void initCanvas();


public:

    void resizeEvent(QResizeEvent* event);


public Q_SLOTS:

    void slot_fpsDisplayedChanged();
    void slot_axisDisplayedChanged();
    void slot_backgroundColorChanged();


private Q_SLOTS:

    void on_actionQuit_triggered();

    void on_actionShowFPS_triggered(bool checked);
    void on_actionShowAxis_triggered(bool checked);

    void on_actionFitScene_triggered();

    void on_actionFullScreen_triggered(bool checked);
    void on_actionLeftDock_triggered(bool checked);
    void on_actionRightDock_triggered(bool checked);



private:

    Ui::ViewerWindow *ui;

    Canvas* vCanvas;
};

}

#include "viewerwindow.cpp"

#endif // NVL_VIEWER_VIEWERWINDOW_H

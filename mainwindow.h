#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QButtonGroup>
#include <QMainWindow>
#include <QMouseEvent>

#include "appHome/about.h"
#include "appHome/feedback.h"
#include "appHome/histrioy.h"
#include "appHome/home.h"
#include "appHome/setting.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

  public slots:
    void receivelogin();

  private slots:

    void on_pushButtonquit_clicked();

    void on_pushButtonreduce_clicked();

    //    void on_pushButtonbig_clicked();

    void on_pushButtonbig_clicked();

  protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent* event);

  private:
    Ui::MainWindow* ui;
    QMainWindow* win;
    QButtonGroup btnGroup;
    Home home;
    Setting set;
    About ab;
    Feedback fd;
    Histrioy histrioy;

    bool m_bDrag;
    QPoint mouseStartPoint;
    QPoint WindowTopLeftPoint;

    QList<QMainWindow*> m_widget;
    QMap<QMainWindow*, QRect> m_WidgetRect;
    bool flag = true;
};
#endif  // MAINWINDOW_H

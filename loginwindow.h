#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDateTime>
#include <QDebug>
#include <QDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QMouseEvent>
#include <QString>
#include <QTimer>

#include "database/data.h"
#include "mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class LoginWindow;
}
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

  public:
    LoginWindow(QWidget* parent = nullptr);
    ~LoginWindow();

  protected:
    void mousePressEvent(QMouseEvent* event);

    void mouseMoveEvent(QMouseEvent* event);

    void mouseReleaseEvent(QMouseEvent* event);

  private slots:
    void timerUpdate();

    void on_pushButton_close_clicked();

    void on_pushButton_full_clicked();

    void on_pushButton_min_clicked();

    void on_pushButton_login_clicked();

    void on_pushButton_regist_clicked();

  private:
    Ui::LoginWindow* ui;
    Data* data;
    bool m_bDrag;
    QPoint mouseStartPoint;
    QPoint WindowTopLeftPoint;
    MainWindow* mainwindow;
};
#endif  // LOGINWINDOW_H

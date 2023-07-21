#ifndef REGIST_H
#define REGIST_H

#include <QMainWindow>
#include <QMouseEvent>

#include "loginwindow.h"

namespace Ui
{
    class Regist;
}

class Regist : public QMainWindow
{
    Q_OBJECT

  public:
    explicit Regist(QWidget* parent = nullptr);
    ~Regist();
    QList<QString> getEidUsrAndPwd();

  protected:
    void mousePressEvent(QMouseEvent* event);

    void mouseMoveEvent(QMouseEvent* event);

    void mouseReleaseEvent(QMouseEvent* event);

  private slots:
    void on_pushButton_regist_clicked();

    void on_pushButton_2_clicked();

  private:
    Ui::Regist* ui;
    bool m_bDrag;
    QPoint mouseStartPoint;
    QPoint WindowTopLeftPoint;
    Data* data;
};

#endif  // REGIST_H

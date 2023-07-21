#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <QMainWindow>
#include <QStandardItem>
namespace Ui {
    class Feedback;
}

class Feedback : public QMainWindow {
    Q_OBJECT

  public:
    explicit Feedback(QWidget* parent = nullptr);
    ~Feedback();
    void limitTextEditTextCount();

  private:
    Ui::Feedback* ui;
    QStandardItem* s1;
    QStandardItemModel* sml;
};

#endif  // FEEDBACK_H

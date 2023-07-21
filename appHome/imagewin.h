#ifndef IMAGEWIN_H
#define IMAGEWIN_H

#include <QListWidget>
#include <QRect>
#include <QWidget>

class imageWin : public QWidget {
    Q_OBJECT
  public:
    imageWin(QListWidgetItem* item, QRect rect, QWidget* parent = 0);
    ~imageWin();

  private:
    QListWidget* imageWidget;
    QListWidgetItem* item;

  signals:

  public slots:

  protected:
    bool eventFilter(QObject* obj, QEvent* e);
};

#endif  // IMAGEWIN_H

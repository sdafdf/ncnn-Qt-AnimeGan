#ifndef HISTRIOY_H
#define HISTRIOY_H

#include <QDir>
#include <QListWidget>
#include <QVector>
#include <QWidget>

#include "customfilewatcher.h"
#include "utils/utils.h"
namespace Ui
{
    class Histrioy;
}

class Histrioy : public QWidget
{
    Q_OBJECT

  public:
    explicit Histrioy(QWidget* parent = nullptr);
    ~Histrioy();

    void showImages(QListWidget* imglist, QString floder);
    void getAllFileFolder(QString dirPath, QVector<QString>& folder);
  private slots:
    void handleDirectoryChanged();

    void on_pushButton_clicked();
    void enlargeImage(QListWidgetItem* item);
    void changeSavePath(QString path);

    void on_comboBox_currentTextChanged(const QString& arg1);

  private:
    Ui::Histrioy* ui;
    QStringList namefiles, files;
    QDir* dir;
    Utils utils;
    CustomFileWatcher m_fileWatcher;
    QVector<QString> floders;
};

#endif  // HISTRIOY_H

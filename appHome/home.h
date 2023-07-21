#ifndef HOME_H
#define HOME_H

#include <QDir>
#include <QListWidgetItem>
#include <QMainWindow>

#include "animeGan/animegan.h"
#include "customfilewatcher.h"
#include "utils/utils.h"
namespace Ui
{
    class Home;
}

class Home : public QMainWindow
{
    Q_OBJECT

  public:
    explicit Home(QWidget* parent = nullptr);
    void initPageImg();
    void initPage2Img();

    void showImages(QListWidget* imglist, cv::Mat& bgr, QString path);
    /*
     * 进度条更新
     */
    void progressBarSize(const int size);
    void progressBarUpdate(const int i);

    /*
     * 单张图片推理函数
     */
    void predict(const QString& fileName);

    ~Home();

  signals:
    void filePath(QString file);
    void dirPathSignal(const QString& dirPath);

  private:
    Ui::Home* ui;
    QStringList namefiles, files;
    QDir* dir;
    Utils utils;
    CustomFileWatcher m_fileWatcher;
    AnimeGan* anime;
    Detection* detection;
    QThread* pthread;
    int maxSize;

  public slots:
    void enlargeImage(QListWidgetItem* item);  //放大图片
  private slots:
    void on_pushButtonSelect_clicked();
    void on_pushButtonSelect_4_clicked();

    void on_pushButtonStart_clicked();
    void on_pushButtonStart_4_clicked();
    void reciveImg(cv::Mat bgr, QString path);
};

#endif  // HOME_H

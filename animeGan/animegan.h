#ifndef ANIMEGAN_H
#define ANIMEGAN_H

#include <ncnn/gpu.h>
#include <ncnn/net.h>

#include <QDir>
#include <QMutex>
#include <QObject>
#include <QThread>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <sstream>

#include "utils/utils.h"

class AnimeGan : public QObject
{
    Q_OBJECT
  public:
    AnimeGan();
    AnimeGan(cv::Mat& bgr);
    void loadModel();
    ~AnimeGan();

    void detecet(const cv::Mat& bgr, cv::Mat& outbgr);
  public slots:
    void getModelName(QString name);

  private:
    int input_h;
    int input_w;

    int output_w;
    int output_h;

    static QString modelname;

    ncnn::Net styletransfernet;
};

class Detection : public QThread
{
    Q_OBJECT
  public:
    Detection();
    ~Detection();
  public slots:
    void scvPath(const QString& Path);
  signals:
    void fileSize(const int size);
    void progressBarValue(const int i);
    void imgdata(cv::Mat bgr, QString path);

  private:
    Utils utils;
    QDir* dir;
    AnimeGan* anime;
    QStringList namefiles, files;
    QString dirPath;
    QMutex mutex;

    // QThread interface
  protected:
    void run();
};

#endif  // ANIMEGAN_H

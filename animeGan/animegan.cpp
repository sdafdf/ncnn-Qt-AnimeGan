#include "animegan.h"

#include <QDebug>
#include <QDir>
#include <QMetaType>
#include <QMutex>
#include <iostream>

QString AnimeGan::modelname = "celeba";

AnimeGan::AnimeGan()
{
}

AnimeGan::AnimeGan(cv::Mat& bgr)
{
    if (bgr.empty())
    {
        qDebug() << "图片读取失败";
    }
    ncnn::create_gpu_instance();
    {
        ncnn::Option opt;
        opt.use_vulkan_compute = true;

        const char* model_paths = "models/face_paint_512_v1.bin";
        const char* param_paths = "models/face_paint_512_v1.param";
        qDebug() << "model_paths:" << model_paths;
        qDebug() << "param_paths:" << param_paths;
        styletransfernet.opt = opt;

        int ret0 = styletransfernet.load_param(param_paths);
        int ret1 = styletransfernet.load_model(model_paths);
        if (ret0 < 0 || ret1 < 0)
        {
            qDebug() << "load:" << ret0 << ret1;
        }
    }
}

void AnimeGan::loadModel()
{
    ncnn::create_gpu_instance();
    {
        ncnn::Option opt;
        opt.use_vulkan_compute = true;
        QByteArray model_path = ("models/" + modelname + ".bin").toLatin1();
        QByteArray param_path = ("models/" + modelname + ".param").toLatin1();
        const char* model_paths = model_path.data();
        const char* param_paths = param_path.data();
        qDebug() << "model_paths:" << model_path;
        qDebug() << "param_paths:" << param_path;
        styletransfernet.opt = opt;

        int ret0 = styletransfernet.load_param(param_paths);
        int ret1 = styletransfernet.load_model(model_paths);
        if (ret0 < 0 || ret1 < 0)
        {
            qDebug() << "load:" << ret0 << ret1;
        }
    }
}
AnimeGan::~AnimeGan()
{
}

void AnimeGan::detecet(const cv::Mat& bgr, cv::Mat& outbgr)
{
    const int w = bgr.cols;
    const int h = bgr.rows;

    int target_w = 512;
    int target_h = 512;

    const float mean_vals[3] = {127.5f, 127.5f, 127.5f};
    const float norm_vals[3] = {1 / 127.5f, 1 / 127.5f, 1 / 127.5f};
    ncnn::Mat in = ncnn::Mat::from_pixels_resize(bgr.data, ncnn::Mat::PIXEL_BGR2RGB, w, h, target_w, target_h);
    in.substract_mean_normalize(mean_vals, norm_vals);
    ncnn::Mat out;
    {
        ncnn::Extractor ex = styletransfernet.create_extractor();

        ex.input("input", in);
        ex.extract("out", out);
    }

    cv::Mat result(out.h, out.w, CV_32FC3);
    for (int i = 0; i < out.c; i++)
    {
        float* out_data = out.channel(i);
        for (int h = 0; h < out.h; h++)
        {
            for (int w = 0; w < out.w; w++)
            {
                result.at<cv::Vec3f>(h, w)[2 - i] = out_data[h * out.h + w];
            }
        }
    }
    cv::Mat result8U(out.h, out.w, CV_8UC3);
    result.convertTo(result8U, CV_8UC3, 127.5, 127.5);
    result8U.copyTo(outbgr);
}

void AnimeGan::getModelName(QString name)
{
    modelname = name;
}

Detection::Detection()
{
    qRegisterMetaType<cv::Mat>("cv::Mat");
}

Detection::~Detection()
{
    delete dir;
    delete anime;
}

void Detection::scvPath(const QString& Path)
{
    dirPath = Path;
}

void Detection::run()
{
    anime = new AnimeGan();
    anime->loadModel();
    dir = new QDir(dirPath);
    namefiles << "*.png"
              << "*.jpg";
    files = (*dir).entryList(namefiles, QDir::Files | QDir::Readable, QDir::Name);
    emit fileSize(files.size());

    QString path;          //本地图片路径
    utils.readJson(path);  //获取config文件中的保存路径

    QDateTime time = QDateTime::currentDateTime();
    QString floderName = time.toString("yyyy-MM-dd");

    QDir dirs(path);
    if (!dirs.exists(floderName))
    {
        dirs.mkdir(floderName);
    }

    for (int i = 0; i < files.size(); i++)
    {
        cv::Mat bgr = cv::imread((dirPath + "/" + files.at(i)).toStdString());

#pragma omp parallel for num_threads(2)
        cv::Mat outbgr;

        anime->detecet(bgr, outbgr);

        QDateTime sfm = QDateTime::currentDateTime();
        QString fileName = sfm.toString("hh-mm-ss-zzz");

        QString save = QString(path + "/" + floderName + "/" + fileName + ".jpg");
        cv::imwrite(save.toStdString(), outbgr);
        emit progressBarValue(i);
        emit imgdata(outbgr, save);
    }
    ncnn::destroy_gpu_instance();
}

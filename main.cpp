#include <QApplication>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextCodec>

#include "loginwindow.h"
void init_config()
{
    //创建config文件夹
    QDir dir(QDir::currentPath());
    QString filesName("config");
    if (!dir.exists(filesName))
    {
        dir.mkdir(filesName);
    }
    if (!dir.exists("output"))
    {
        dir.mkdir("output");
    }

    //创建config.json并初始化
    QFile file(QDir::currentPath() + "/config/config.json");
    bool ret = file.open(QIODevice::ReadWrite | QIODevice::Text);
    if (ret)
    {
        QJsonObject obj;
        QJsonDocument doc;
        obj.insert("SavePath", QDir::currentPath() + "/output");

        doc.setObject(obj);
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream << doc.toJson();
        file.close();
    }
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    QTextCodec* codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);

    //初始化json
    init_config();

    Data* data = new Data();
    data->init();

    LoginWindow w;
    w.show();
    return a.exec();
}

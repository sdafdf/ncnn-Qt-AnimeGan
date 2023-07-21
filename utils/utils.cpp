#include "utils.h"

Utils::Utils()
{
}

bool Utils::readJson(QString& filesPath)
{
    QFile file(QDir::currentPath() + "/config/config.json");
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        QString str = stream.readAll();

        //解析json
        QJsonParseError jsonError;
        doc = QJsonDocument::fromJson(str.toUtf8(), &jsonError);

        auto root = doc.object();
        filesPath = root.value("SavePath").toString();

        // emit savePath(SavePath);
        return true;
    }
    file.close();
    return false;
}

bool Utils::saveJson(QString& savePath)
{
    QFile file(QDir::currentPath() + "/config/config.json");
    bool ret = file.open(QIODevice::ReadWrite | QIODevice::Text);
    if (ret)
    {
        QByteArray jsonData = file.readAll();
        QJsonDocument jsondoc(QJsonDocument::fromJson(jsonData));

        QJsonObject jsonobj = jsondoc.object();
        jsonobj["SavePath"] = savePath;
        jsondoc.setObject(jsonobj);
        file.resize(0);
        file.write(jsondoc.toJson());
        file.close();
        return true;
    }
    return false;
}

bool Utils::readClassName(const QString& filePath, std::vector<std::string>& classNames)
{
    std::ifstream fp(filePath.toStdString());
    if (!fp.is_open())
    {
        qDebug() << fp.is_open();
        qDebug() << "could not open file...";
        return false;
    }
    std::string name;
    while (!fp.eof())
    {
        std::getline(fp, name);
        if (name.length())
            classNames.push_back(name);
    }
    fp.close();
    return true;
}

QString Utils::RandomFile()
{
    QString result;
    qsrand(QDateTime::currentMSecsSinceEpoch());
    const char arr[] = "qwertyuiopasdfghjklzxcvbnm";
    int arr_size = sizeof(arr);
    int index = 0;
    for (int i = 0; i < 6; i++)
    {
        index = qrand() % (arr_size - 1);
        QChar ch = arr[index];
        result.append(ch);
    }
    return result;
}

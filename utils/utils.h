#ifndef UTILS_H
#define UTILS_H
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <fstream>
#include <string>
#include <vector>
class Utils
{
  public:
    Utils();
    bool readJson(QString& filesPath);

    bool saveJson(QString& savePath);

    bool readClassName(const QString& filePath, std::vector<std::string>& classNames);

    bool saveImages();

    QString RandomFile();

  signals:
    void savePath(QString& path);

  private:
    QJsonObject* obj;
    QJsonDocument doc;
};

#endif  // UTILS_H

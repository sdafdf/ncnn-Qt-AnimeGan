#ifndef CUSTOMFILEWATCHER_H
#define CUSTOMFILEWATCHER_H

#include <QFileSystemWatcher>
#include <QThread>

#include "utils/utils.h"

class CustomFileWatcher : public QFileSystemWatcher
{
    Q_OBJECT
  public:
    explicit CustomFileWatcher(QObject* parent = Q_NULLPTR);
    explicit CustomFileWatcher(const QString& path, QObject* parent = Q_NULLPTR);

    void addPath(const QString& path);

  signals:
    /*!
     * \brief fileCreated 文件创建信号
     * \param fileName 创建的文件名
     */
    void fileCreated(const QString& fileName);

    /*!
     * \brief fileChanged 文件名改变信号
     * \param previousFileName 改变之前的文件名
     * \param currentFileName 改变之后的文件名
     */
    void fileNameChanged(const QString& previousFileName, const QString& currentFileName);

    /*!
     * \brief fileRemoved 文件删除信号
     * \param fileName 删除的文件名
     */
    void fileRemoved(const QString& fileName);

  private slots:

    void onDirectoryChanged(const QString& path);

  private:
    QStringList m_fileEntryList;
};

#endif  // CUSTOMFILEWATCHER_H

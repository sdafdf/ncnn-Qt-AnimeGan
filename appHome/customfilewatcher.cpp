#include "customfilewatcher.h"

#include <QDebug>
#include <QDir>

#include "appHome/home.h"
CustomFileWatcher::CustomFileWatcher(QObject* parent) : QFileSystemWatcher(parent)
{
    connect(this, &CustomFileWatcher::directoryChanged, this, &CustomFileWatcher::onDirectoryChanged);
}

CustomFileWatcher::CustomFileWatcher(const QString& path, QObject* parent) :
    QFileSystemWatcher(QStringList(path), parent)
{
    QFileSystemWatcher::addPath(path);

    QDir dir(path);
    dir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
    m_fileEntryList = dir.entryList();

    connect(this, &CustomFileWatcher::directoryChanged, this, &CustomFileWatcher::onDirectoryChanged);
}

/*!
 * \brief addPath 增加监视目录路径
 * \param path 路径
 */
void CustomFileWatcher::addPath(const QString& path)
{
    if (!directories().isEmpty())
    {
        removePath(directories().first());
    }
    QFileSystemWatcher::addPath(path);

    QDir dir(path);
    dir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
    m_fileEntryList = dir.entryList();
}

/*!
 * \brief onDirectoryChanged 目录状态（被修改）改变处理函数
 * \param path 目录路径
 */
void CustomFileWatcher::onDirectoryChanged(const QString& path)
{
    QDir dir(path);
    dir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);

    QStringList fileEntryList = dir.entryList();

    int previousFileCount = m_fileEntryList.count();
    int currentFileCount = fileEntryList.count();
    if (previousFileCount == currentFileCount)
    {
        for (int i = 0; i < previousFileCount; i++)
        {
            QString previousFileName = m_fileEntryList.at(i);
            if (!fileEntryList.contains(previousFileName))
            {
                for (int j = 0; j < currentFileCount; j++)
                {
                    QString currentFileName = fileEntryList.at(j);
                    if (!m_fileEntryList.contains(currentFileName))
                    {
                        emit fileNameChanged(previousFileName, currentFileName);
                        m_fileEntryList = fileEntryList;
                        return;
                    }
                }
            }
        }
    }
    else if (previousFileCount > currentFileCount)
    {
        for (int i = 0; i < currentFileCount; i++)
        {
            m_fileEntryList.removeOne(fileEntryList.at(i));
        }
        emit fileRemoved(m_fileEntryList.first());
        m_fileEntryList = fileEntryList;
    }
    else
    {
        QStringList tempList = fileEntryList;
        for (int i = 0; i < previousFileCount; i++)
        {
            tempList.removeOne(m_fileEntryList.at(i));
        }
        emit fileCreated(tempList.first());
        m_fileEntryList = fileEntryList;
    }
}

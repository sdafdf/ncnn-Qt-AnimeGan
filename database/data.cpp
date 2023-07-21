#pragma execution_character_set("utf-8")
#include "data.h"

#include <QMessageBox>
#include <QTimer>
Data::Data()
{
}

Data::~Data()
{
    // delete query;
}

void Data::init()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //    db.setHostName("120.48.88.186");
    //    db.setPort(3306);
    //    db.setDatabaseName("test");
    //    db.setUserName("wang");
    //    db.setPassword("123456");
    db.setDatabaseName("usr.db");
    QSqlQuery query(db);
    bool ret = db.open();
    if (!ret)
    {
        qDebug() << "数据库打开失败";
        QMessageBox* m_box = new QMessageBox(QMessageBox::Information, QString("错误"), QString("请检查网络连接"));
        m_box->setStyleSheet("QMessageBox {background-color: #F2F2F2; }");
        m_box->show();
    }
    else
    {
        qDebug() << "数据库打开成功";
    }
    query.exec("create table usr(id int, name varchar(30) primary key,pwd varchar(60))");
    query.exec("insert into usr values(1, '袁天罡', '123456')");
}

bool Data::insert(QString sql)
{
    QSqlQuery query;
    if (!query.exec(sql))
    {
        return false;
    }
    return true;
}

int Data::getTableSize()
{
    QSqlQuery query;
    query.exec("select * from usr");
    QSqlQueryModel* querymodel = new QSqlQueryModel();
    querymodel->setQuery(query);
    int count = querymodel->rowCount();
    return count;
}

bool Data::select(QString sql, QList<QString>& res)
{
    QSqlQuery query;
    bool ret = query.exec(sql);

    if (!ret)
    {
        qDebug() << query.lastError();
    }
    else
    {
        //这里虽然写了个循环，但是根据数据库主键的特殊性，查询到的结果应该只有一条
        while (query.next())
        {
            QString name = query.value(0).toString();
            QString pwd = query.value(1).toString();
            if (name == nullptr || pwd == nullptr)
            {
                return false;
            }
            res.append(name);
            res.append(pwd);
        }
    }
    return ret;
}

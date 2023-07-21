#ifndef DATA_H
#define DATA_H
//#pragma execution_character_set("UTF-8")
#include <QDebug>
#include <QList>
#include <QMap>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QString>
class Data {
  public:
    Data();
    ~Data();
    void init();
    bool insert(QString sql);
    int getTableSize();
    bool select(QString sql, QList<QString>& res);
};

#endif  // DATA_H

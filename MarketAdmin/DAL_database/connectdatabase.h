#ifndef CONNECTDATABASE_H
#define CONNECTDATABASE_H

#include <QString>
#include <QSqlDatabase>
#include <QSqlError>

class ConnectDatabase
{
public:
    static  QSqlDatabase ConnectSqlite(const QString &databaseName);
    static void CloseSqlite(const QString &databaseName);
};

#endif // CONNECTDATABASE_H

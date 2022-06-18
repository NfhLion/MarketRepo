#include "connectdatabase.h"
#include <QDebug>

QSqlDatabase ConnectDatabase::ConnectSqlite(const QString &databaseName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "connectionUser");
    db.setDatabaseName(databaseName);

    return db;
}

void ConnectDatabase::CloseSqlite(const QString &databaseName)
{
    QSqlDatabase::database(databaseName).close();
    return ;
}

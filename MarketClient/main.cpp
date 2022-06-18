#include "marketlogin.h"
#include "config.h"
#include "execsql.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Config::InitApp(a.applicationDirPath());        //返回程序可执行文件的目录
    MarketLogin w;

    ExecSql::GetExecSql()->OpenUserDb(Config::m_strDatabasePath + "user.db");
    ExecSql::GetExecSql()->OpenCommodityDb(Config::m_strDatabasePath + "commodity.db");

    w.show();
    int res = a.exec();

    ExecSql::GetExecSql()->CloseUserDb(Config::m_strDatabasePath + "user.db");
    ExecSql::GetExecSql()->CloseCommdityDb(Config::m_strDatabasePath + "commodity.db");

    return res;
}

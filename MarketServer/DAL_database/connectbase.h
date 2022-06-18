#ifndef CONNECTBASE_H
#define CONNECTBASE_H
#include <stdlib.h>
#include <stdio.h>
#include <mysql/mysql.h>

MYSQL *InitDatabase();                                                              //创建并初始化数据库
void   ConnectDatabase(MYSQL *mysql, const char *host, const char *user,
                    const char *passwd, const char *db);         //链接数据库
void   CloseDatabase(MYSQL *mysql);                                                             //关闭数据库链接
MYSQL *GetDatebase();                                                               //返回数据库指针共其他文件使用


#endif // CONNECTBASE_H

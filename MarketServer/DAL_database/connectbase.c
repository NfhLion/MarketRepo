#include "connectbase.h"

static MYSQL *sql = NULL;

///创建并初始化数据库
MYSQL *InitDatabase()
{
    MYSQL *sql = (MYSQL *)malloc(sizeof(MYSQL));
    if(sql == NULL)
    {
        printf("malloc MYSQL failed!\n");
        return NULL;
    }
    //数据库初始化
    if(mysql_init(sql) == NULL)
    {
        printf("mysql_init file: %s\n", strerror(mysql_errno(sql)));
        return NULL;
    }

    return sql;
}

///链接数据库
void ConnectDatabase(MYSQL *mysql, const char *host, const char *user,
                  const char *passwd, const char *db)
{
    //链接数据库
    if(mysql_real_connect(mysql, host, user, passwd, db, 0, NULL, 0) == NULL){
        printf("mysql_real_connect: %s\n", strerror(mysql_errno(sql)));
        exit(1);
    }

    //设置可采用utf8格式
    mysql_set_character_set(mysql, "utf8");
    printf("connect mysql successful...\n");

    sql = mysql;
    return ;
}

///关闭数据库
void CloseDatabase(MYSQL *mysql)
{
    if(mysql != NULL){
        mysql_close(mysql);
        free(mysql);
        mysql = NULL;
    }

    return ;
}

///返回数据库指针共其他文件使用
MYSQL *GetDatebase()
{
    return sql;
}



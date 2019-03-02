#ifndef MYSQL_H
#define MYSQL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>

#define  QMYSQL   0         //QMYSQL = 0 使用SQLITE数据库   QMYSQL = 1 使用MYSQL数据库

static bool CreateConnection()
{
    #if QMYSQL
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL"); //数据库类型

    db.setDatabaseName("music"); //数据库名
    db.setHostName("localhost");    //数据库所在服务器地址
    db.setUserName("root");         //用户名
    db.setPassword("111111");       //密码
    db.setPort(3306);               //端口
    #else

    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE"); //数据库类型
    db.setDatabaseName("music.db"); //数据库名

    #endif

    if(!db.open())
    {
        qDebug()<<"Database error: "<<db.lastError().text();
        return false;
    }

    // 如果 MySQL 数据库中已经存在同名的表， 那么下面的代码不会执行
    QSqlQuery query(db);



    //创建表 保存本地添加记录   要指定varchar的大小
    qDebug()<<query.exec(QString( "create table localMusic"
                                  "("
                                     "id                   int , "
                                     "MusicName    varchar(255), "
                                     "SongFileName varchar(255) NOT NULL unique, "
                                     "LrcFileName  varchar(255), "
                                     "PicFileName  varchar(255), "
                                     "primary key  (id)"
                                  ")"
                                 )
                         );

    return true;
}


#endif // MYSQL_H

#ifndef MYSQL_H
#define MYSQL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>

#define  QMYSQL   0         //QMYSQL = 0 ʹ��SQLITE���ݿ�   QMYSQL = 1 ʹ��MYSQL���ݿ�

static bool CreateConnection()
{
    #if QMYSQL
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL"); //���ݿ�����

    db.setDatabaseName("music"); //���ݿ���
    db.setHostName("localhost");    //���ݿ����ڷ�������ַ
    db.setUserName("root");         //�û���
    db.setPassword("111111");       //����
    db.setPort(3306);               //�˿�
    #else

    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE"); //���ݿ�����
    db.setDatabaseName("music.db"); //���ݿ���

    #endif

    if(!db.open())
    {
        qDebug()<<"Database error: "<<db.lastError().text();
        return false;
    }

    // ��� MySQL ���ݿ����Ѿ�����ͬ���ı� ��ô����Ĵ��벻��ִ��
    QSqlQuery query(db);



    //������ ���汾����Ӽ�¼   Ҫָ��varchar�Ĵ�С
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

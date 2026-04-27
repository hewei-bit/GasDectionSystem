/***************************************************************
 *Copyright(c) 2014-2015 Company By LH Unigress
 *All rights reserved.
 *文件名称：数据库的创建文件
 *简要描述：创建数据库，数据表
 *
 *当前版本：V2.0
 *作者：Kelvin Li
 *创作日期：2014/10
 *说明：
*****************************************************************/
#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QtSql>
#include <QObject>


/*创建数据里并链接*/
static bool createConnection()
{
    QSqlDatabase db;            //创建一个数据库
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }
    db.setHostName("localhost");
    db.setDatabaseName("gasleakMonitor.db");  //设置数据库的名字
    db.setUserName("root");
    db.setPassword("123456");
    if(!db.open())
    {
        qCritical("Can't open database: %s(%s)",
                  db.lastError().text().toLocal8Bit().data(),
                  qt_error_string().toLocal8Bit().data());

        return false;
    }

 #if 0
    QSqlQuery query;
    /***************************数据记录表 ****************************
     *"DeviceID", "日期时间", "LORA地址", "LORA信道", "湿度", "温度", "CH4浓度"
     *
    */
    CREATE TABLE [Record](
      [DeviceID] INT PRIMARY KEY ON CONFLICT ROLLBACK AUTOINCREMENT NOT NULL UNIQUE,
      [DATETIME] DATETIME(20) NOT NULL,
      [LORA_ADDR] INT(20),
      [LORA_CHN] INT(20),
      [HUMIDITY] DOUBLE(20),
      [temperature] DOUBLE(20),
      [CH4_concentration] INT(20));
    query.exec(QObject::tr("create table Record (ID vchar(4) primary key ON CONFLICT rollback autoincrement not null unique,"
                           "DATETIME datetime(20) not null,"
                           "LORA_ADDR int(20),"
                           "LORA_CHN int(20),"
                           "HUMIDITY double(20),"
                           "temperature double(20),"
                           "CH4_concentration int(20)"));
    query.exec(QObject::tr("insert into Record(DeviceID，DATETIME,LORA_ADDR,LORA_CHN,HUMIDITY,temperature,CH4_concentration) values(‘1’，'2020-12-19 14:57:30','10','23','89','20','500')"));
    query.exec(QObject::tr("SELECT * FROM Record where DATE = '2020-12-17'"));
    query.exec(QObject::tr("update Record set temperature = 45 where rowid = 6"));
    query.exec(QObject::tr("DELETE FROM Record where rowid = 6"));


#endif
    return true;
}

/***************关闭数据库****************/
static bool closeConnection()
{
    QSqlDatabase::database().close();
    return 1;
}


#endif // DATABASE_H

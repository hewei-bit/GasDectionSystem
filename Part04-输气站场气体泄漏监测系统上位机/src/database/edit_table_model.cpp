#include "edit_table_model.h"



edit_table_model::edit_table_model(QObject *parent)
{

}

bool edit_table_model::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //根据表的列数，判断是否为有效列
    if(index.column() < 1 || index.column() > 6)
        return false;

    //为了获取得到ID
    QModelIndex prinmaryKeyIndex = QSqlQueryModel::index(index.row(),0);
    qDebug() <<  "DeviceID:   "<<data(QSqlQueryModel::index(index.row(),0)).toInt();
    int DeviceID  = data(prinmaryKeyIndex).toInt();
    //清除操作
    clear();
    qDebug() << "=====" << DeviceID << "=====" << index.column();

    bool ok;

    if(index.column()==1)
    {
        //设置单元格上的内容
        ok = setDATETIME(DeviceID,value.toString());
    }
    if(index.column()==2)
    {
        //设置单元格上的内容
        ok = setLORA_ADDR(DeviceID,value.toString());
    }
    if(index.column()==3)
    {
        //设置单元格上的内容
        ok = setLORA_CHN(DeviceID,value.toString());
    }
    if(index.column()==4)
    {
        //设置单元格上的内容
        ok = setHumidity(DeviceID,value.toString());
    }
    if(index.column()==5)
    {
        //设置单元格上的内容
        ok = setTemperature(DeviceID,value.toString());
    }
    if(index.column()==6)
    {
        //设置单元格上的内容
        ok = setCH4(DeviceID,value.toString());
    }

    refresh(index.column());

    return  ok;
}

Qt::ItemFlags edit_table_model::flags(const QModelIndex &index) const
{
    //获取当前条目的标志

   Qt::ItemFlags flags = QSqlQueryModel::flags(index);

   //根据自己表中的列数情况，设置对应列的标志
   if(index.column() == 1)
       flags |= Qt::ItemIsEditable; //给条目增加一个可编辑的标志
   if(index.column() == 2)
       flags |= Qt::ItemIsEditable; //给条目增加一个可编辑的标志
   if(index.column() == 3)
       flags |= Qt::ItemIsEditable; //给条目增加一个可编辑的标志
   if(index.column() == 4)
       flags |= Qt::ItemIsEditable; //给条目增加一个可编辑的标志
   if(index.column() == 5)
       flags |= Qt::ItemIsEditable; //给条目增加一个可编辑的标志
   if(index.column() == 6)
       flags |= Qt::ItemIsEditable; //给条目增加一个可编辑的标志

   return flags;
}

void edit_table_model::refresh(int i)
{
    setQuery("select * from Record");
    setHeaderData(0,Qt::Horizontal,QObject::tr("DeviceID"));
    setHeaderData(1,Qt::Horizontal,QObject::tr("日期时间"));
    setHeaderData(2,Qt::Horizontal,QObject::tr("LORA地址"));
    setHeaderData(3,Qt::Horizontal,QObject::tr("LORA信道"));
    setHeaderData(4,Qt::Horizontal,QObject::tr("湿度"));
    setHeaderData(5,Qt::Horizontal,QObject::tr("温度"));
    setHeaderData(6,Qt::Horizontal,QObject::tr("CH4浓度"));

}

bool edit_table_model::setDATETIME(int DeviceID, const QString &DATETIME)
{
    QSqlQuery query;
    query.prepare("update Record set DATETIME = ? where DeviceID = ?");
    query.addBindValue(DATETIME);
    query.addBindValue(DeviceID);
    return  query.exec();
}

bool edit_table_model::setLORA_ADDR(int DeviceID, const QString &LORA_ADDR)
{
    QSqlQuery query;
    query.prepare("update Record set LORA_ADDR = ? where DeviceID = ?");
    query.addBindValue(LORA_ADDR);
    query.addBindValue(DeviceID);
    return  query.exec();
}

bool edit_table_model::setLORA_CHN(int DeviceID, const QString &LORA_CHN)
{
    QSqlQuery query;
    query.prepare("update Record set LORA_CHN = ? where DeviceID = ?");
    query.addBindValue(LORA_CHN);
    query.addBindValue(DeviceID);
    return  query.exec();
}

bool edit_table_model::setHumidity(int DeviceID, const QString &humidity)
{
    QSqlQuery query;
    query.prepare("update Record set humidity = ? where DeviceID = ?");
    query.addBindValue(humidity);
    query.addBindValue(DeviceID);
    return  query.exec();
}

bool edit_table_model::setTemperature(int DeviceID, const QString &temperature)
{
    QSqlQuery query;
    query.prepare("update Record set temperature = ? where DeviceID = ?");
    query.addBindValue(temperature);
    query.addBindValue(DeviceID);
    return  query.exec();
}

bool edit_table_model::setCH4(int DeviceID, const QString &CH4)
{
    QSqlQuery query;
    query.prepare("update Record set CH4 = ? where DeviceID = ?");
    query.addBindValue(CH4);
    query.addBindValue(DeviceID);
    return  query.exec();
}

bool edit_table_model::InsertData(const QString &DeviceID, const QString &DATETIME, const QString &LORA_ADDR, const QString &LORA_CHN, const QString &humidity, const QString &temperature, const QString &CH4)
{
    QSqlQuery query;
    query.prepare("insert into Record(DeviceID，DATETIME,LORA_ADDR,LORA_CHN,HUMIDITY,temperature,CH4_concentration) "
                  "values(?,?,?,?,?,?,?)");
    query.addBindValue(DeviceID);
    query.addBindValue(DATETIME);
    query.addBindValue(LORA_ADDR);
    query.addBindValue(LORA_CHN);
    query.addBindValue(humidity);
    query.addBindValue(temperature);
    query.addBindValue(CH4);
    return  query.exec();
}










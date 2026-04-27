#include "edit_table_model.h"



edit_table_model::edit_table_model(QObject *parent)
{

}

bool edit_table_model::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //根据表的列数，判断是否为有效列
    if(index.column() < 1 || index.column() > 7)
        return false;

    //为了获取得到id
    QModelIndex prinmaryKeyIndex = QSqlQueryModel::index(index.row(),0);
    qDebug() <<  "id:   "<<data(QSqlQueryModel::index(index.row(),0)).toInt();
    int id  = data(prinmaryKeyIndex).toInt();
    //清除操作
    clear();
    qDebug() << "=====" << id << "=====" << index.column();

    bool ok;

    if(index.column()==1)
    {
        //设置单元格上的内容
        ok = setName(id,value.toString());
    }
    if(index.column()==2)
    {
        //设置单元格上的内容
        ok = setTele(id,value.toString());
    }
    if(index.column()==3)
    {
        //设置单元格上的内容
        ok = setAccount(id,value.toString());
    }
    if(index.column()==4)
    {
        //设置单元格上的内容
        ok = setPassword(id,value.toString());
    }
    if(index.column()==5)
    {
        //设置单元格上的内容
        ok = setbirth(id,value.toString());
    }
    if(index.column()==6)
    {
        //设置单元格上的内容
        ok = setAddress(id,value.toString());
    }
    if(index.column()==7)
    {
        //设置单元格上的内容
        ok = setRFID(id,value.toString());
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
   if(index.column() == 7)
       flags |= Qt::ItemIsEditable; //给条目增加一个可编辑的标志

   return flags;
}

void edit_table_model::refresh(int i)
{
    setQuery("select * from owner");
    setHeaderData(0,Qt::Horizontal,QObject::tr("id"));
    setHeaderData(1,Qt::Horizontal,QObject::tr("name"));
    setHeaderData(2,Qt::Horizontal,QObject::tr("tele"));
    setHeaderData(3,Qt::Horizontal,QObject::tr("account"));
    setHeaderData(4,Qt::Horizontal,QObject::tr("password"));
    setHeaderData(5,Qt::Horizontal,QObject::tr("birth"));
    setHeaderData(6,Qt::Horizontal,QObject::tr("address"));
    setHeaderData(7,Qt::Horizontal,QObject::tr("RFID"));
}

bool edit_table_model::setName(int userId, const QString &userName)
{
    QSqlQuery query;
    query.prepare("update owner set name = ? where id = ?");
    query.addBindValue(userName);
    query.addBindValue(userId);
    return  query.exec();
}


bool edit_table_model::setTele(int userId, const QString &usertele)
{
    QSqlQuery query;
    query.prepare("update owner set tele = ? where id = ?");
    query.addBindValue(usertele);
    query.addBindValue(userId);
    return  query.exec();
}

bool edit_table_model::setAccount(int userId, const QString &useraccount)
{
    QSqlQuery query;
    query.prepare("update owner set account = ? where id = ?");
    query.addBindValue(useraccount);
    query.addBindValue(userId);
    return  query.exec();
}

bool edit_table_model::setPassword(int userId, const QString &userpassword)
{
    QSqlQuery query;
    query.prepare("update owner set passowrd = ? where id = ?");
    query.addBindValue(userpassword);
    query.addBindValue(userId);
    return  query.exec();
}

bool edit_table_model::setbirth(int userId, const QString &userbirth)
{
    QSqlQuery query;
    query.prepare("update owner set birth = ? where id = ?");
    query.addBindValue(userbirth);
    query.addBindValue(userId);
    return  query.exec();
}

bool edit_table_model::setAddress(int userId, const QString &useraddr)
{
    QSqlQuery query;
    query.prepare("update owner set address = ? where id = ?");
    query.addBindValue(useraddr);
    query.addBindValue(userId);
    return  query.exec();
}

bool edit_table_model::setRFID(int userId, const QString &userRFID)
{
    QSqlQuery query;
    query.prepare("update owner set RFID = ? where id = ?");
    query.addBindValue(userRFID);
    query.addBindValue(userId);
    return  query.exec();
}





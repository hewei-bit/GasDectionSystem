#ifndef EDIT_TABLE_MODEL_H
#define EDIT_TABLE_MODEL_H

#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QtDebug>

class edit_table_model:public QSqlQueryModel
{
public:
    explicit edit_table_model(QObject *parent = nullptr);

    //重写父类的虚函数
    //The model is read-only by default. To make it read-write,
    //you must subclass it and reimplement setData() and flags()
    virtual bool setData(const QModelIndex &index,
                         const QVariant &value, int role = Qt::EditRole);

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    //更新数据
    void refresh(int i);

    //根据需求定义内部的字段相关的接口
    bool setName(int userId,const QString &userName);

    bool setTele(int userId,const QString &usertele);

    bool setAccount(int userId,const QString &useraccount);

    bool setPassword(int userId,const QString &userpassword);

    bool setbirth(int userId,const QString &userbirth);

    bool setAddress(int userId,const QString &useraddr);

    bool setRFID(int userId,const QString &userRFID);

    bool setData(int userId,const QString &userRFID);

    bool InsertData(int userId,const QString &userRFID);
};

#endif // EDIT_TABLE_MODEL_H

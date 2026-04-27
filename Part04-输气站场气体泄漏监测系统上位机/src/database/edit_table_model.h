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
    bool setDATETIME(int DeviceID,const QString &DATETIME);

    bool setLORA_ADDR(int DeviceID,const QString &LORA_ADDR);

    bool setLORA_CHN(int DeviceID,const QString &LORA_CHN);

    bool setHumidity(int DeviceID,const QString &humidity);

    bool setTemperature(int DeviceID,const QString &temperature);

    bool setCH4(int DeviceID,const QString &CH4);

    bool InsertData(const QString &DeviceID,
                    const QString &DATETIME,
                    const QString &LORA_ADDR,
                    const QString &LORA_CHN,
                    const QString &humidity,
                    const QString &temperature,
                    const QString &CH4);
};

#endif // EDIT_TABLE_MODEL_H

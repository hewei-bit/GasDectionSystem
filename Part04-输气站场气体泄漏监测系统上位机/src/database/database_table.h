#ifndef DATABASE_TABLE_H
#define DATABASE_TABLE_H
#include <QString>

class Database_table
{
public:
    Database_table(QString nameTable,QString DeviceID,
                   QString DATETIME,QString LORA_ADDR,
                   QString LORA_CHN,QString humidity,QString temperature,QString CH4)
        :nameTable(nameTable),DeviceID(DeviceID),DATETIME(DATETIME),LORA_ADDR(LORA_ADDR),
          LORA_CHN(LORA_CHN),humidity(humidity),temperature(temperature),CH4(CH4)
    {

    }
    QString getTableName() const
    {
        return nameTable;
    }
    QString getDeviceID() const
    {
        return DeviceID;
    }
    QString getDATETIME() const
    {
        return DATETIME;
    }
    QString getLORA_ADDR() const
    {
        return LORA_ADDR;
    }
    QString getLORA_CHN() const
    {
        return LORA_CHN;
    }

    QString getHumidity() const
    {
        return humidity;
    }
    QString getTemperature() const
    {
        return temperature;
    }
    QString getCH4() const
    {
        return CH4;
    }

private:
    QString nameTable;
    QString DeviceID;
    QString DATETIME;
    QString LORA_ADDR;
    QString LORA_CHN;
    QString humidity;
    QString temperature;
    QString CH4;
};






























#endif // DATABASE_TABLE_H

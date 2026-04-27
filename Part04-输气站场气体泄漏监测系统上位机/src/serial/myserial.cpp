#include "myserial.h"


myserial::myserial()
{


}
myserial::~myserial()
{

}
QSerialPort::Parity myserial::selectParityBits(const QString &value)
{
    if(value == "无")
    {
        qDebug() << "NoParity";
        return QSerialPort::NoParity;
    }
    if(value == "偶校验")
    {
        return QSerialPort::EvenParity;
    }
    if(value == "奇校验")
    {
        return QSerialPort::OddParity;
    }
    if(value == "SpaceParity")
    {
        return QSerialPort::SpaceParity;
    }
    if(value == "MarkParity")
    {
        return QSerialPort::MarkParity;
    }
    else
    {
        return QSerialPort::UnknownParity;
    }
}

QSerialPort::StopBits myserial::selectStopBits(const QString &value)
{
    if(value == "1")
    {
        qDebug() << "onestop";
        return QSerialPort::OneStop;
    }
    if(value == "1.5")
    {
        return QSerialPort::OneAndHalfStop;
    }
    if(value == "2")
    {
        return QSerialPort::TwoStop;
    }
    else
    {
        return QSerialPort::UnknownStopBits;
    }
}

QSerialPort::DataBits myserial::selectDataBits(const QString &value)
{
    switch (value.toUShort()) {
        case 5:return QSerialPort::Data5;
        case 6:return QSerialPort::Data6;
        case 7:return QSerialPort::Data7;
        case 8:
            qDebug() << "8";
            return QSerialPort::Data8;
        default:return QSerialPort::UnknownDataBits;
    }
}

QSerialPort::BaudRate myserial::selectBaudRate(const QString &value)
{
    switch (value.toInt())
    {
        case 1200:return QSerialPort::Baud1200;
        case 2400:return QSerialPort::Baud2400;
        case 4800:return QSerialPort::Baud4800;
        case 9600:return QSerialPort::Baud9600;
        case 19200:return QSerialPort::Baud19200;
        case 38400:return QSerialPort::Baud38400;
        case 57600:return QSerialPort::Baud57600;
        case 115200:
            qDebug() << "115200";
            return QSerialPort::Baud115200;
        default:return QSerialPort::UnknownBaud;
    }
}

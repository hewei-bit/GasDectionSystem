#ifndef MYSERIAL_H
#define MYSERIAL_H

#include <QMainWindow>
#include "ui_gasleakmonitor.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtDebug>
#include <QTime>
#include <QTimer>
#include <QMessageBox>



class myserial :public QSerialPort
{
public:
    myserial();
    ~myserial();
    QSerialPort::Parity selectParityBits(const QString &value);
    QSerialPort::StopBits selectStopBits(const QString &value);
    QSerialPort::DataBits selectDataBits(const QString &value);
    QSerialPort::BaudRate selectBaudRate(const QString &value);
private:
    QSerialPort mserial;//串口类对象
    QList<QSerialPortInfo> serialinfos;//串口类信息
    QTimer *timer;

    bool portIsOpen;
    bool Hex_recv;
    bool Hex_send;
    bool text_recv;
    bool text_send;
    bool autoSend;
    bool show_recvtime;
    bool send_newline;

    unsigned char fontNum;

};

#endif // MYSERIAL_H

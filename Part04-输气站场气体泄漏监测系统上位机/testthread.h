#ifndef SQLTESTTHREAD_H
#define SQLTESTTHREAD_H

#include <QMainWindow>
#include <QTime>
#include <QThread>
#include <QtDebug>


class MysqltestThread:public QThread
{
    Q_OBJECT
public:
    explicit MysqltestThread(QWidget *parent = nullptr){}
    ~MysqltestThread(){}

    void run() override
    {
        while(1){
            sleep(1);
            emit send();
        }
    }
signals:
    void send();
};

class MySysTestThread:public QThread
{
    Q_OBJECT
public:
    explicit MySysTestThread(QWidget *parent = nullptr){}
    ~MySysTestThread(){}

    void run() override
    {
        while(1){
            sleep(3);
            emit send();
        }
    }
signals:
    void send();
};

#endif // SQLTESTTHREAD_H

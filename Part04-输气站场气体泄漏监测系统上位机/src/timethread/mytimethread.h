#ifndef MYTIMETHREAD_H
#define MYTIMETHREAD_H

#include <QMainWindow>
#include <QTime>
#include <QThread>
#include <QtDebug>



class MyTimeThread:public QThread
{
    Q_OBJECT
public:
    explicit MyTimeThread(QWidget *parent = nullptr){}
    ~MyTimeThread(){}

    void run() override
    {
        while(1){
            //QTime time1 = QTime::currentTime();
            //qDebug() << time1.toString("hh:mm:ss");
            sleep(1);
            emit send();
        }
    }
signals:
    void send();
};








#endif // MYTIMETHREAD_H

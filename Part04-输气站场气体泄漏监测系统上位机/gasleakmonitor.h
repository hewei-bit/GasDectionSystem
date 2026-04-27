#ifndef GASLEAKMONITOR_H
#define GASLEAKMONITOR_H

#include <QMainWindow>
#include <QDebug>
#include <QtDebug>

#include <QTimer>
#include <QTime>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <QPushButton>
#include <QKeyEvent>
#include <QApplication>
#include <QFile>

//数据库
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
//
#include <QTableView>
#include <QTableWidgetItem>
//JSON数据解析
#include<QJsonArray>  //[]
#include <QJsonDocument> //JSON文档 --- 将服务器数据转换成一个QJsonDocument
#include <QJsonObject> //{}



//完成对网络数据的请求
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "qcustomplot.h"
#include "myserial.h"
#include "mytimethread.h"
#include "testthread.h"
#include "database.h"
#include "cJSON.h"
#include "mynode.h"

namespace Ui {
class GasLeakMonitor;
}

class GasLeakMonitor : public QMainWindow
{
    Q_OBJECT

public:


    explicit GasLeakMonitor(QWidget *parent = nullptr);
    ~GasLeakMonitor();
    //串口初始化
    void Port_init();
    //发送接收初始化
    void sendRecvInit();
    //显示时间相关
    void showtime();
    void TimeRun();
    //回车事件
    bool eventFilter(QObject *target, QEvent *event);//事件过滤器

    //画图
    void humi_plot();
    void temp_plot();
    void CH4_plot();

    //打开数据库
    void openDatabase();
    void openTable();
    void sql_test_run();

    //系统运行
    void sys_test_run();

    int count_for_record(QString &sqlquery);

    void Get_data_from_record(QString &sqlquery,QString &parameter,QVector<double> &dataX,QVector<double> &dataY);

    void resize_buffer();

    void humi_realtime_plot_set();

    void temp_realtime_plot_set();

    void CH4_realtime_plot_set();

    void plot_data_init();

    void readWeatherData(QNetworkReply* reply);
    void MyJsonParse(NODE &node,QByteArray &ba);

public slots:
    //读数据
    void read_data();
    void sql_test();
    void send_check();

    //画图demo
    void humi_plot_Slot(double new_humiY,int ID);
    void temp_plot_Slot(double new_tempY,int ID);
    void CH4_plot_Slot(double new_CH4Y,int ID);
    void http_weather();

private slots:
    //检查串口1
    void on_check_serial_Btn_1_clicked();
    //打开串口1
    void on_open_serial_Btn_1_clicked();
    //刷新串口1
    void on_refresh_Btn_1_clicked();
    //发送
    void on_send_Btn_clicked();
    //清除发送区
    void on_send_clear_Btn_clicked();
    //检查串口2
    void on_check_serial_Btn_2_clicked();
    //打开串口2
    void on_open_serial_Btn_2_clicked();
    //刷新串口2
    void on_refresh_Btn_2_clicked();
    //统计清零
    void on_sNumClearBtn_clicked();
    //清除接收区
    void on_clear_recv_box_Btn_clicked();

    void on_sql_tableWidget_cellActivated(int row, int column);
    //数据库查询
    void on_query_Btn_clicked();

    void on_check_Btn_clicked();

signals:
    void humi_plot_signal(double new_humiY,int ID);
    void temp_plot_signal(double new_tempY,int ID);
    void CH4_plot_signal(double new_CH4Y,int ID);

private:
    Ui::GasLeakMonitor *ui;

    QTimer *timer_1;
    myserial myserial_1;                //串口1
    myserial myserial_2;
    QList<QSerialPortInfo> serialinfos; //串口类信息

    QNetworkAccessManager *manager;

    QTimer *mtimer;                     //定时器
    QTime time1;                        //时间
    QDate data1;                        //日期
    MyTimeThread *mtimethread;          //时间显示
    NODE node1;
    NODE node2;
    //画图相关
    QTimer dataTimer;
    int num_all;
    int num_node_0;
    int num_node_1;
    int num_node_2;

    QVector<double> humiY_0;
    QVector<double> humiX_0;
    QVector<double> humiY_1;
    QVector<double> humiX_1;
    QVector<double> humiY_2;
    QVector<double> humiX_2;

    QVector<double> TempY_0;
    QVector<double> TempX_0;
    QVector<double> TempY_1;
    QVector<double> TempX_1;
    QVector<double> TempY_2;
    QVector<double> TempX_2;

    QVector<double> CH4Y_0;
    QVector<double> CH4X_0;
    QVector<double> CH4Y_1;
    QVector<double> CH4X_1;
    QVector<double> CH4Y_2;
    QVector<double> CH4X_2;

    //数据库相关变量
    QTimer *sql_timer;
    MysqltestThread *msqltestthread;
    MySysTestThread *msystestthread;

    //QString dbname = "C:/Users/10038/Desktop/GasLeakMonitorSystem/build-GasLeakMonitor-Desktop_Qt_5_13_0_MinGW_32_bit-Debug/gasleakMonitor.db";
    QString dbname = "../GasLeakMonitor/gasleakMonitor.db";
    //QString dbname = "./gasleakMonitor.db";
    QSqlDatabase database;

    QSqlQuery sqlQuery;

    QString sqlSelect = "SELECT * FROM Record";
    QString sqlSelect_0 = "SELECT * FROM Record where DeviceID = '0'";
    QString sqlSelect_1 = "SELECT * FROM Record where DeviceID = '1'";
    QString sqlSelect_2 = "SELECT * FROM Record where DeviceID = '2'";

    QString sqlInsert;
    QString sqlupdate;
    QString sqldelete;

    QString humi = "humi";
    QString temp = "temp";
    QString CH4 = "CH4";

    int count = 0;

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







#endif // GASLEAKMONITOR_H

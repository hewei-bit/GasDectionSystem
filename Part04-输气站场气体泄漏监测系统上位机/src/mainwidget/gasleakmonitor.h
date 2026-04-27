#ifndef GASLEAKMONITOR_H
#define GASLEAKMONITOR_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QTime>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <QPushButton>

#include "qcustomplot.h"

#include "myserial.h"
#include "mytimethread.h"

namespace Ui {
class GasLeakMonitor;
}

class GasLeakMonitor : public QMainWindow
{
    Q_OBJECT

public:
    explicit GasLeakMonitor(QWidget *parent = nullptr);
    ~GasLeakMonitor();


    void plot_test();

    void showtime();

    void TimeRun();

private slots:

    void on_check_serial_Btn_1_clicked();

    void on_open_serial_Btn_1_clicked();

    void on_refresh_Btn_1_clicked();

    void on_clear_recv_box_pB_clicked();

    void on_send_Btn_clicked();

    void on_send_clear_Btn_clicked();

    void on_check_serial_Btn_2_clicked();

    void on_open_serial_Btn_2_clicked();

    void on_refresh_Btn_2_clicked();



private:
    Ui::GasLeakMonitor *ui;
    myserial myserial_1;
    myserial myserial_2;
    QTimer *mtimer;
    QTime time1;
    QDate data1;
    MyTimeThread *mtimethread;
};

#endif // GASLEAKMONITOR_H

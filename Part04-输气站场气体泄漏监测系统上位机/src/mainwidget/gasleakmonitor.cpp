#include "gasleakmonitor.h"
#include "ui_gasleakmonitor.h"




void GasLeakMonitor::TimeRun()
{

//    mtimer = new QTimer(this);
//    connect(mtimer,&QTimer::timeout,this,&GasLeakMonitor::showtime);
//    mtimer->setInterval(1000);
//    mtimer->start(1000);

    mtimethread = new MyTimeThread(this);
    mtimethread->setObjectName("mtimethread");
    connect(mtimethread,&MyTimeThread::send,this,&GasLeakMonitor::showtime);
    mtimethread->start();
}

GasLeakMonitor::GasLeakMonitor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GasLeakMonitor)
{
    ui->setupUi(this); 
    plot_test();
    TimeRun();
    showtime();

}

void GasLeakMonitor::showtime()
{
    time1 = QTime::currentTime();
    ui->Time_label->setText(time1.toString("hh:mm:ss"));
    data1 = QDate::currentDate();
    ui->Date_label->setText(data1.toString("yyyy:MM:dd"));
}

void GasLeakMonitor::plot_test()
{
//    设定背景为黑色
    ui->Fixed_point_data_plot->setBackground(QBrush(Qt::black));
//    设定右上角图形标注可见
    ui->Fixed_point_data_plot->legend->setVisible(true);
    //设定右上角图形标注的字体
    ui->Fixed_point_data_plot->legend->setFont(QFont("Helvetica",9));
    QVector<double> x(101),y(101);
    //图形为y=x^3
    for(int i = 0;i < 101;i++){
        x[i] = i/5.0 - 10;
        y[i] = x[i]*x[i]*x[i];
    }
    //添加图形
    ui->Fixed_point_data_plot->addGraph();
    //设置画笔
    ui->Fixed_point_data_plot->graph(0)->setPen(QPen(Qt::blue));
    //设置画刷,曲线和X轴围成面积的颜色
    ui->Fixed_point_data_plot->graph(0)->setBrush(QBrush(QColor(255,255,0)));
    //设置右上角图形标注名称
    ui->Fixed_point_data_plot->graph(0)->setName("曲线");
    //传入数据，setData的两个参数类型为double
    ui->Fixed_point_data_plot->graph(0)->setData(x,y);

    /*-------------------------------------------*/
    //画动态曲线时，传入数据采用addData，通过定时器多次调用，并在之后调用ui->widget->replot();
    //动态曲线可以通过另一种设置坐标的方法解决坐标问题：
    //setRange ( double  position, double  size, Qt::AlignmentFlag  alignment  )
    //参数分别为：原点，偏移量，对其方式，有兴趣的读者可自行尝试，欢迎垂询
    /*-------------------------------------------*/

    QVector<double> temp(20);
    QVector<double> temp1(20);
    //图形为y = 100*x;
    for(int i = 0;i < 20;i++){
        temp[i] = 1;
        temp1[i] = i*10+10;
    }
    //添加图形
    ui->Fixed_point_data_plot->addGraph();
    //设置画笔
    ui->Fixed_point_data_plot->graph(1)->setPen(QPen(Qt::red));
    //设置画刷
    //ui->widget->graph(1)->setBrush(QBrush(QColor(0,255,0)));
    //传入数据
    ui->Fixed_point_data_plot->graph(1)->setData(temp,temp1);

    //设置右上角图形标注名称
    ui->Fixed_point_data_plot->graph(1)->setName("直线");
    //设置X轴文字标注
    ui->Fixed_point_data_plot->xAxis->setLabel("time");
    //设置y轴文字标注
    ui->Fixed_point_data_plot->yAxis->setLabel("temp/shidu");
    //设置X轴坐标范围
    ui->Fixed_point_data_plot->xAxis->setRange(-20,20);
    //设置y轴坐标范围
    ui->Fixed_point_data_plot->yAxis->setRange(-1100,1100);
    //在坐标轴右侧和上方画线，和X/Y轴一起形成一个矩形
    ui->Fixed_point_data_plot->axisRect()->setupFullAxesBox();
}

GasLeakMonitor::~GasLeakMonitor()
{
    delete ui;
}


void GasLeakMonitor::on_check_serial_Btn_1_clicked()
{

}

void GasLeakMonitor::on_open_serial_Btn_1_clicked()
{

}

void GasLeakMonitor::on_refresh_Btn_1_clicked()
{

}


void GasLeakMonitor::on_check_serial_Btn_2_clicked()
{

}

void GasLeakMonitor::on_open_serial_Btn_2_clicked()
{

}

void GasLeakMonitor::on_refresh_Btn_2_clicked()
{

}


void GasLeakMonitor::on_clear_recv_box_pB_clicked()
{

}

void GasLeakMonitor::on_send_Btn_clicked()
{

}

void GasLeakMonitor::on_send_clear_Btn_clicked()
{

}

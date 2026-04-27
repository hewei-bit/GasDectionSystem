#include "plot_demo.h"

plot_demo::plot_demo()
{

}

plot_demo::plot_demo(QCustomPlot *customPlot)
{
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));//曲线1蓝色
    customPlot->addGraph();
    customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));//曲线2红色
    // 坐标轴使用时间刻度
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    customPlot->xAxis->setTicker(timeTicker);
    // 四边安上坐标轴
    customPlot->axisRect()->setupFullAxesBox();
    // 设置y轴范围
    customPlot->yAxis->setRange(-1.2, 1.2);
    // 使上下轴、左右轴范围同步
    connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
    // 定时器连接槽函数realtimeDataSlot
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    dataTimer.start(0);
    // 间隔时间 0ms表示尽可能快的触发
}

void plot_demo::Graph_Show(QCustomPlot *CustomPlot)
{
    QVector<double> temp(10);
    QVector<double> temp1(10);

    for(int i = 0;i < 9;i++){
        num[i] = num[i+1];
    }
    num[9] = n;
    for(int i=0;i<10;i++)
     {
         temp[i] = i;
         temp1[i] =num[i];
     }
     CustomPlot->addGraph();
     CustomPlot->graph(0)->setPen(QPen(Qt::red));
     CustomPlot->graph(0)->setData(temp,temp1);

     CustomPlot->xAxis->setLabel("t");
     CustomPlot->yAxis->setLabel("mV");

     CustomPlot->xAxis->setRange(0,10);
     CustomPlot->yAxis->setRange(-50,50);
     CustomPlot->replot();
}

void plot_demo::plot_test(QCustomPlot *CustomPlot)
{
    // 设定背景为黑色
    CustomPlot->setBackground(QBrush(Qt::black));
    // 设定右上角图形标注可见
    CustomPlot->legend->setVisible(true);
    // 设定右上角图形标注的字体
    CustomPlot->legend->setFont(QFont("Helvetica",9));
    QVector<double> x(101),y(101);
    // 图形为y=x^3
    for(int i = 0;i < 101;i++){
        x[i] = i/5.0 - 10;
        y[i] = x[i]*x[i]*x[i];
    }
    // 添加图形
    CustomPlot->addGraph();
    // 设置画笔
    CustomPlot->graph(0)->setPen(QPen(Qt::blue));
    // 设置画刷,曲线和X轴围成面积的颜色
    CustomPlot->graph(0)->setBrush(QBrush(QColor(255,255,0)));
    // 设置右上角图形标注名称
    CustomPlot->graph(0)->setName("曲线");
    // 传入数据，setData的两个参数类型为double
    CustomPlot->graph(0)->setData(x,y);

    /*-------------------------------------------*/
    // 画动态曲线时，传入数据采用addData，通过定时器多次调用，并在之后调用ui->widget->replot();
    // 动态曲线可以通过另一种设置坐标的方法解决坐标问题：
    // setRange ( double  position, double  size, Qt::AlignmentFlag  alignment  )
    // 参数分别为：原点，偏移量，对其方式，有兴趣的读者可自行尝试，欢迎垂询
    /*-------------------------------------------*/

    QVector<double> temp(20);
    QVector<double> temp1(20);
    // 图形为y = 100*x;
    for(int i = 0;i < 20;i++){
        temp[i] = 1;
        temp1[i] = i*10+10;
    }
    // 添加图形
    CustomPlot->addGraph();
    // 设置画笔
    CustomPlot->graph(1)->setPen(QPen(Qt::red));
    // 设置画刷
    //ui->widget->graph(1)->setBrush(QBrush(QColor(0,255,0)));
    // 传入数据
    CustomPlot->graph(1)->setData(temp,temp1);

    // 设置右上角图形标注名称
    CustomPlot->graph(1)->setName("直线");
    // 设置X轴文字标注
    CustomPlot->xAxis->setLabel("time");
    // 设置y轴文字标注
    CustomPlot->yAxis->setLabel("temp/shidu");
    // 设置X轴坐标范围
    CustomPlot->xAxis->setRange(-20,20);
    // 设置y轴坐标范围
    CustomPlot->yAxis->setRange(-1100,1100);
    // 在坐标轴右侧和上方画线，和X/Y轴一起形成一个矩形
    CustomPlot->axisRect()->setupFullAxesBox();
}

void plot_demo::Graph_Show()
{
    QTime t;
    t = QTime::currentTime();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    n = qrand()%50;
    Graph_Show(MyCustomPlot);
}

void plot_demo::realtimeDataSlot()
{
    QCustomPlot* customPlot = MyCustomPlot;
    static QTime time(QTime::currentTime());

    double key = time.elapsed()/1000.0; // 开始到现在的时间，单位秒
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // 大约2ms添加一次数据
    {
        // 添加数据到graph
        customPlot->graph(0)->addData(key, qSin(key)+qrand()/(double)RAND_MAX*1*qSin(key/0.3843));
        customPlot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));
        // 记录当前时刻
        lastPointKey = key;
    }
    // 曲线能动起来的关键在这里，设定x轴范围为最近8个时刻
    customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
    // 绘图
    customPlot->replot();
    // 计算帧数
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // 每2秒求一次平均值
    {
        // 状态栏显示帧数和数据总数
//        ui->statusBar->showMessage(
//           QString("%1 FPS, Total Data points: %2")
//           .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
//           .arg(customPlot->graph(0)->data()->size()+customPlot->graph(1)->data()->size())
//           , 0);
        lastFpsKey = key;
        frameCount = 0;
    }
}

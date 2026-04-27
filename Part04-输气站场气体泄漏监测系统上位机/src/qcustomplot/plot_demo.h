#ifndef PLOT_DEMO_H
#define PLOT_DEMO_H

#include <QMainWindow>
#include <QDebug>
#include <QtDebug>


#include "qcustomplot.h"

class plot_demo: public QMainWindow
{
    Q_OBJECT
public:
    plot_demo();
    plot_demo(QCustomPlot *customPlots);
    void Graph_Show(QCustomPlot *CustomPlot);
    void plot_test(QCustomPlot *CustomPlot);


public slots:
    void Graph_Show();
    void realtimeDataSlot();

private:
    int n = 0;
    double num[10];
    QTimer dataTimer;
    QCustomPlot *MyCustomPlot;
};

#endif // PLOT_DEMO_H

#include "gasleakmonitor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GasLeakMonitor w;
    w.show();

    return a.exec();
}

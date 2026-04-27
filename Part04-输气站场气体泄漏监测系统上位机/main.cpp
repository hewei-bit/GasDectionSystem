#include "gasleakmonitor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //定制皮肤在程序运行之前设置
    QFile file(":/ui/qss/mystyle.qss");
    file.open(QIODevice::ReadOnly);
    QByteArray array =  file.readAll();
    #define qApp (static_cast<QApplication *>(QCoreApplication::instance()))
    qApp->setStyleSheet(array);


    GasLeakMonitor w;
    w.show();

    return a.exec();
}

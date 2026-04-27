#ifndef MYNODE_H
#define MYNODE_H

#include <QMainWindow>

class NODE
{
public:
    NODE() {}
    ~NODE() {}

    int IDint;
    QString IDstr;

    int LORA_ADRint;
    QString LORA_ADRstr;

    int LORA_CHNint;
    QString LORA_CHNstr;

    double tempdouble;
    QString tempstr;

    double humidouble;
    QString humistr;

    double CH4double;
    QString CH4_str;

    double lightdouble;
    QString light_str;
};
#endif // MYNODE_H

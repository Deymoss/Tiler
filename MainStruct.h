#ifndef MAINSTRUCT_H
#define MAINSTRUCT_H
#include <QString>
struct MainStruct
{
    QString  map;
    QString  style;
    double       latTop;
    double latBottom;
    double lonLeft;
    double lonRight;
    unsigned int startLevel;
    unsigned int endLevel;
    QString pluginName;
};

#endif // MAINSTRUCT_H

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
struct TileStruct
{
    uint32_t x;
    uint32_t y;
    uint32_t zoom;
    QString  map;
    QString  style;
};

#endif // MAINSTRUCT_H

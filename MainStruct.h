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
struct structForBinFile
{
    uint32_t x;
    uint32_t y;
    uint8_t zoom;
    uint16_t sizeOfTile;
    uint32_t startByte;
};
struct ConstantStruct
{
    uint32_t countOfTiles;
    uint32_t xTileStart;
    uint32_t yTileStart;
    uint32_t xTileCount;
    uint32_t yTileCount;
};

#endif // MAINSTRUCT_H

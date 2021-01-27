#ifndef TILEDATACLASS_H
#define TILEDATACLASS_H

#include <QString>
#include <QDataStream>
#include <QDebug>
class TileDataClass
{
    uint32_t x;
    uint32_t y;
    uint8_t zoom;
public:
    TileDataClass() : x( 0 ), y( 0 ), zoom(0) { }

        TileDataClass( uint32_t xx, uint32_t yy, uint8_t zzoom ) : x( xx ), y( yy ), zoom(zzoom) {
        }
    uint32_t getX();
    uint32_t getY();
    uint8_t getZoom();
    friend QDataStream& operator>>(QDataStream &stream, TileDataClass &data);
    friend QDataStream& operator<<(QDataStream &stream, TileDataClass data);
};

#endif // TILEDATACLASS_H

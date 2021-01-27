#include "tiledataclass.h"

uint32_t TileDataClass::getX()
{
    return x;
}

uint32_t TileDataClass::getY()
{
    return y;
}

uint8_t TileDataClass::getZoom()
{
    return zoom;
}
QDataStream& operator>>(QDataStream &stream, TileDataClass &data)
{
    stream>>data.x>>data.y>>data.zoom;
    return stream;
}
QDataStream& operator<<(QDataStream &stream, TileDataClass data)
{
    stream<<data.x<<data.y<<data.zoom;
    return stream;
}
QDebug operator<<(QDebug debug, TileDataClass &data)
{
    debug << QString("Data(%1, %2, %3 )").arg(data.getX()).arg(data.getY()).arg(data.getZoom());
    return debug;
}

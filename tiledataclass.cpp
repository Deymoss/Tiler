#include "tiledataclass.h"

QDataStream& operator>>(QDataStream &stream, TileDataClass &data)
{
    stream>>data.x>>data.y>>data.zoom<<data.startPoint<<data.size;
    return stream;
}
QDataStream& operator<<(QDataStream &stream, TileDataClass data)
{
    stream<<data.x<<data.y<<data.zoom<<data.startPoint<<data.size;
    return stream;
}
QDebug operator<<(QDebug debug, TileDataClass &data)
{
    debug << QString("Data(%1, %2, %3 )").arg(data.x).arg(data.y).arg(data.zoom);
    return debug;
}

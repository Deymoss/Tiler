#ifndef RENDERCLASS_H
#define RENDERCLASS_H

#include <QThread>
#include "queuebuilder.h"
#include "MainStruct.h"
#include <QPainter>
#include <osmscout/Database.h>
#include <osmscout/MapService.h>

#include <osmscout/MapPainterQt.h>

#include <osmscout/util/StopClock.h>
#include <osmscout/util/Tiling.h>

class RenderClass : public QThread
{
public:
    RenderClass(QueueBuilder * builder);
    void run() override;

    QString  map;
    QString  style;
    void MergeTilesToMapData(const std::list<osmscout::TileRef>& centerTiles,
                             const osmscout::MapService::TypeDefinition& ringTypeDefinition,
                             const std::list<osmscout::TileRef>& ringTiles,
                             osmscout::MapData& data);
    void getN(QueueBuilder * builder);
private:
    QueueBuilder * currentBuilder;
};

#endif // RENDERCLASS_H

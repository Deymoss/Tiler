#ifndef RENDERCLASS_H
#define RENDERCLASS_H

#include <QPainter>
#include <QDir>
#include <QObject>
#include <QElapsedTimer>

#include <iomanip>
#include <QtDebug>
#include <limits>

#include <QThread>
#include "queuebuilder.h"
#include "MainStruct.h"
#include "tiledataclass.h"
#include <osmscout/Database.h>
#include <osmscout/MapService.h>

#include <osmscout/MapPainterQt.h>

#include <osmscout/util/StopClock.h>
#include <osmscout/util/Tiling.h>

class RenderClass : public QThread
{
    Q_OBJECT
public:
    RenderClass(QueueBuilder * builder);
    void run() override;

    QString  map;
    QString  style;

    TileDataClass *tileClass;
    void MergeTilesToMapData(const std::list<osmscout::TileRef>& centerTiles,
                             const osmscout::MapService::TypeDefinition& ringTypeDefinition,
                             const std::list<osmscout::TileRef>& ringTiles,
                             osmscout::MapData& data);
    void getN(QueueBuilder * builder, MainStruct data);
private:
    QueueBuilder * currentBuilder;
signals:
    void endOfRender();
};

#endif // RENDERCLASS_H

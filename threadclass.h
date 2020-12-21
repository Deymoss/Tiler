#ifndef THREADCLASS_H
#define THREADCLASS_H

#include <QObject>
#include <QThread>
#include "MainStruct.h"
#include <osmscout/Database.h>
#include <osmscout/MapService.h>

#include <osmscout/MapPainterQt.h>

#include <osmscout/util/StopClock.h>
#include <osmscout/util/Tiling.h>
#include <QPainter>
#include <QDir>

#include <iostream>
#include <iomanip>
#include <QtDebug>
#include <limits>
class ThreadClass : public QThread
{
    Q_OBJECT
public:
    ThreadClass(MainStruct data,osmscout::MagnificationLevel level, osmscout::DatabaseParameter databaseParameter, osmscout::DatabaseRef database);
    void MergeTilesToMapData(const std::list<osmscout::TileRef>& centerTiles,
                                     const osmscout::MapService::TypeDefinition& ringTypeDefinition,
                                     const std::list<osmscout::TileRef>& ringTiles,
                                     osmscout::MapData& data);
//    void run() override;
private:
    MainStruct data;
    osmscout::TileProjection      projection;
    osmscout::MapParameter        drawParameter;
    osmscout::AreaSearchParameter searchParameter;
    osmscout::StyleConfigRef styleConfig;
    osmscout::DatabaseParameter  databaseParameter;
    osmscout::DatabaseRef       database;
    osmscout::MapServiceRef     mapService;
signals:
    void zoomFinished();
};

#endif // THREADCLASS_H

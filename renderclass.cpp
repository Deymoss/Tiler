#include "renderclass.h"
static const unsigned int tileWidth=256;
static const unsigned int tileHeight=256;
static const double       DPI=96.0;
static const int          tileRingSize=1;
RenderClass::RenderClass(QueueBuilder * builder)
{
    currentBuilder = builder;
    start();
}
void RenderClass::run()
{
    TileDataClass *tileClass = new TileDataClass();
    QDir dir = QDir::current();
    if(!dir.exists("offline_tiles"))
        dir.mkdir("offline_tiles");

    osmscout::DatabaseParameter databaseParameter;
    osmscout::DatabaseRef       database=std::make_shared<osmscout::Database>(databaseParameter);
    osmscout::MapServiceRef     mapService=std::make_shared<osmscout::MapService>(database);
    osmscout::GeoBox    boundingBox;
    osmscout::MapData   data;

    if (!database->Open(currentBuilder->getMapPath().toStdString())) {
        std::cerr << "Cannot open database" << std::endl;

        exit();
    }

    osmscout::StyleConfigRef styleConfig=std::make_shared<osmscout::StyleConfig>(database->GetTypeConfig());

    if (!styleConfig->Load(currentBuilder->getStypePath().toStdString())) {
        std::cerr << "Cannot open style" << std::endl;
    }
    osmscout::TileProjection      projection;
    osmscout::MapParameter        drawParameter;
    osmscout::AreaSearchParameter searchParameter;

    drawParameter.SetFontName("/usr/share/fonts/truetype/msttcorefonts/Verdana.ttf");
    drawParameter.SetFontName("/usr/share/fonts/TTF/DejaVuSans.ttf");
    drawParameter.SetFontSize(2.0);
    drawParameter.SetDrawFadings(false);
    searchParameter.SetUseLowZoomOptimization(true);
    searchParameter.SetMaximumAreaLevel(3);

    osmscout::MapService::TypeDefinition typeDefinition;
    osmscout::MapPainterQt painter(styleConfig);

    while(!isInterruptionRequested())
    {

        tileClass = currentBuilder->getNext();//takes the tile
        osmscout::MagnificationLevel level(tileClass->zoom);
        osmscout::Magnification magnification(level);

        for (const auto& type : database->GetTypeConfig()->GetTypes()) {
            bool hasLabel=false;

            if (type->CanBeNode()) {
                if (styleConfig->HasNodeTextStyles(type,
                                                   magnification)) {
                    typeDefinition.nodeTypes.Set(type);
                    hasLabel=true;
                }
            }

            if (type->CanBeArea()) {
                if (styleConfig->HasAreaTextStyles(type,
                                                   magnification)) {
                    if (type->GetOptimizeLowZoom() && searchParameter.GetUseLowZoomOptimization()) {
                        typeDefinition.optimizedAreaTypes.Set(type);
                    }
                    else {
                        typeDefinition.areaTypes.Set(type);
                    }

                    hasLabel=true;
                }
            }
        }


        projection.Set(osmscout::OSMTileId(tileClass->x,tileClass->y),
                       magnification,
                       DPI,
                       tileWidth,
                       tileHeight);
        projection.GetDimensions(boundingBox);
        std::cout << "Drawing tile " << QString::number(tileClass->zoom).toInt() << "." << tileClass->x << "." << tileClass->y << " " << boundingBox.GetDisplayText() << std::endl;
        std::list<osmscout::TileRef> centerTiles;

        mapService->LookupTiles(magnification,
                                boundingBox,
                                centerTiles);

        mapService->LoadMissingTileData(searchParameter,
                                        *styleConfig,
                                        centerTiles);
        std::map<osmscout::TileKey,osmscout::TileRef> ringTileMap;

        for (uint32_t ringY=tileClass->y-tileRingSize; ringY<=tileClass->y+tileRingSize; ringY++) {
            for (uint32_t ringX=tileClass->x-tileRingSize; ringX<=tileClass->x+tileRingSize; ringX++) {
                if (ringX==tileClass->x && ringY==tileClass->y) {
                    continue;
                }

                osmscout::GeoBox boundingBox(osmscout::OSMTileId(ringX,ringY).GetBoundingBox(magnification));


                std::list<osmscout::TileRef> tiles;

                mapService->LookupTiles(magnification,
                                        boundingBox,
                                        tiles);

                for (const auto& tile : tiles) {
                    ringTileMap[tile->GetKey()]=tile;
                }
            }
        }

        std::list<osmscout::TileRef> ringTiles;

        for (const auto& tileEntry : ringTileMap) {
            ringTiles.push_back(tileEntry.second);
        }

        mapService->LoadMissingTileData(searchParameter,
                                        magnification,
                                        typeDefinition,
                                        ringTiles);

        MergeTilesToMapData(centerTiles,
                            typeDefinition,
                            ringTiles,
                            data);

        QPixmap pixmap(tileWidth, tileHeight);
        QPainter qp(&pixmap);

        painter.DrawMap(projection,
                        drawParameter,
                        data,
                        &qp);

        std::string output=std::to_string(tileClass->zoom)+"_"+std::to_string(tileClass->x)+"_"+std::to_string(tileClass->y)+".ppm";

        pixmap.save(QString("offline_tiles/%0_100-l-%1-%2-%3-%4.png").arg("osm_custom").arg(1).arg(tileClass->zoom).arg(tileClass->x).arg(tileClass->y));
        emit endOfRender();
    }

}
void RenderClass::MergeTilesToMapData(const std::list<osmscout::TileRef>& centerTiles,
                                      const osmscout::MapService::TypeDefinition& ringTypeDefinition,
                                      const std::list<osmscout::TileRef>& ringTiles,
                                      osmscout::MapData& data)
{
    std::unordered_map<osmscout::FileOffset,osmscout::NodeRef> nodeMap(10000);
    std::unordered_map<osmscout::FileOffset,osmscout::WayRef>  wayMap(10000);
    std::unordered_map<osmscout::FileOffset,osmscout::AreaRef> areaMap(10000);
    std::unordered_map<osmscout::FileOffset,osmscout::WayRef>  optimizedWayMap(10000);
    std::unordered_map<osmscout::FileOffset,osmscout::AreaRef> optimizedAreaMap(10000);

    osmscout::StopClock uniqueTime;

    for (const auto& tile : centerTiles) {
        tile->GetNodeData().CopyData([&nodeMap](const osmscout::NodeRef& node) {
            nodeMap[node->GetFileOffset()]=node;
        });

        //---

        tile->GetOptimizedWayData().CopyData([&optimizedWayMap](const osmscout::WayRef& way) {
            optimizedWayMap[way->GetFileOffset()]=way;
        });

        tile->GetWayData().CopyData([&wayMap](const osmscout::WayRef& way) {
            wayMap[way->GetFileOffset()]=way;
        });

        //---

        tile->GetOptimizedAreaData().CopyData([&optimizedAreaMap](const osmscout::AreaRef& area) {
            optimizedAreaMap[area->GetFileOffset()]=area;
        });

        tile->GetAreaData().CopyData([&areaMap](const osmscout::AreaRef& area) {
            areaMap[area->GetFileOffset()]=area;
        });
    }

    for (const auto& tile : ringTiles) {
        tile->GetNodeData().CopyData([&ringTypeDefinition,&nodeMap](const osmscout::NodeRef& node) {
            if (ringTypeDefinition.nodeTypes.IsSet(node->GetType())) {
                nodeMap[node->GetFileOffset()]=node;
            }
        });

        //---

        tile->GetOptimizedWayData().CopyData([&ringTypeDefinition,&optimizedWayMap](const osmscout::WayRef& way) {
            if (ringTypeDefinition.optimizedWayTypes.IsSet(way->GetType())) {
                optimizedWayMap[way->GetFileOffset()]=way;
            }
        });

        tile->GetWayData().CopyData([&ringTypeDefinition,&wayMap](const osmscout::WayRef& way) {
            if (ringTypeDefinition.wayTypes.IsSet(way->GetType())) {
                wayMap[way->GetFileOffset()]=way;
            }
        });

        //---

        tile->GetOptimizedAreaData().CopyData([&ringTypeDefinition,&optimizedAreaMap](const osmscout::AreaRef& area) {
            if (ringTypeDefinition.optimizedAreaTypes.IsSet(area->GetType())) {
                optimizedAreaMap[area->GetFileOffset()]=area;
            }
        });

        tile->GetAreaData().CopyData([&ringTypeDefinition,&areaMap](const osmscout::AreaRef& area) {
            if (ringTypeDefinition.areaTypes.IsSet(area->GetType())) {
                areaMap[area->GetFileOffset()]=area;
            }
        });
    }

    uniqueTime.Stop();

    //std::cout << "Make data unique time: " << uniqueTime.ResultString() << std::endl;

    osmscout::StopClock copyTime;

    data.nodes.reserve(nodeMap.size());
    data.ways.reserve(wayMap.size()+optimizedWayMap.size());
    data.areas.reserve(areaMap.size()+optimizedAreaMap.size());

    for (const auto& nodeEntry : nodeMap) {
        data.nodes.push_back(nodeEntry.second);
    }

    for (const auto& wayEntry : wayMap) {
        data.ways.push_back(wayEntry.second);
    }

    for (const auto& wayEntry : optimizedWayMap) {
        data.ways.push_back(wayEntry.second);
    }

    for (const auto& areaEntry : areaMap) {
        data.areas.push_back(areaEntry.second);
    }

    for (const auto& areaEntry : optimizedAreaMap) {
        data.areas.push_back(areaEntry.second);
    }

    copyTime.Stop();

    if (copyTime.GetMilliseconds()>20) {
        osmscout::log.Warn() << "Copying data from tile to MapData took " << copyTime.ResultString();
    }

}



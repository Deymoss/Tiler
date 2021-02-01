#include "renderclass.h"
static const unsigned int tileWidth=256;
static const unsigned int tileHeight=256;
static const double       DPI=96.0;
static const int          tileRingSize=1;
static const QString map = "/home/deymos/imported_data";
static const QString style = "/home/deymos/SIMURAN/libosmscout/stylesheets/standard.oss";
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

    double lattitude, longitude, longitudeOfTheTopRightCorner, lattitudeOfTheTopRightCorner, longitudeOfTheBottomLeftCorner,lattitudeOfTheBottomLeftCorner;
    double stepLattitude, stepLongitude;
    osmscout::DatabaseParameter databaseParameter;
    osmscout::DatabaseRef       database=std::make_shared<osmscout::Database>(databaseParameter);
    osmscout::MapServiceRef     mapService=std::make_shared<osmscout::MapService>(database);
    osmscout::GeoBox    boundingBox;
    osmscout::MapData   data;
    while(!isInterruptionRequested())
    {
        tileClass = currentBuilder->getNext();
    if (!database->Open("/home/deymos/imported_data")) {//добавить потом struct
        std::cerr << "Cannot open database" << std::endl;

        exit();
    }

    osmscout::StyleConfigRef styleConfig=std::make_shared<osmscout::StyleConfig>(database->GetTypeConfig());

    if (!styleConfig->Load("/home/deymos/SIMURAN/libosmscout/stylesheets/standard.oss")) {//struct
        std::cerr << "Cannot open style" << std::endl;
    }
    osmscout::TileProjection      projection;
    osmscout::MapParameter        drawParameter;
    osmscout::MagnificationLevel level(tileClass->getZoom());
    osmscout::Magnification magnification(level);
    osmscout::AreaSearchParameter searchParameter;

    drawParameter.SetFontName("/usr/share/fonts/truetype/msttcorefonts/Verdana.ttf");
    drawParameter.SetFontName("/usr/share/fonts/TTF/DejaVuSans.ttf");
    drawParameter.SetFontSize(2.0);
    drawParameter.SetDrawFadings(false);
    drawParameter.SetDropNotVisiblePointLabels(false);
    searchParameter.SetUseLowZoomOptimization(true);
    searchParameter.SetMaximumAreaLevel(3);

    osmscout::MapService::TypeDefinition typeDefinition;

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
    osmscout::MapPainterQt painter(styleConfig);


    projection.Set(osmscout::OSMTileId(tileClass->getX(),tileClass->getY()),
                   magnification,
                   DPI,
                   tileWidth,
                   tileHeight);
    projection.GetDimensions(boundingBox);
    std::cout << "Drawing tile " << QString::number(tileClass->getZoom()).toInt() << "." << tileClass->getX() << "." << tileClass->getY() << " " << boundingBox.GetDisplayText() << std::endl;
    std::list<osmscout::TileRef> centerTiles;

    mapService->LookupTiles(magnification,
                            boundingBox,
                            centerTiles);

    mapService->LoadMissingTileData(searchParameter,
                                    *styleConfig,
                                    centerTiles);
    std::map<osmscout::TileKey,osmscout::TileRef> ringTileMap;

    for (uint32_t ringY=tileClass->getY()-tileRingSize; ringY<=tileClass->getY()+tileRingSize; ringY++) {
        for (uint32_t ringX=tileClass->getX()-tileRingSize; ringX<=tileClass->getX()+tileRingSize; ringX++) {
            if (ringX==tileClass->getX() && ringY==tileClass->getY()) {
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

    std::string output=std::to_string(tileClass->getZoom())+"_"+std::to_string(tileClass->getX())+"_"+std::to_string(tileClass->getY())+".ppm";


    uint32_t xOfTile = QString("%1").arg(tileClass->getX()).toUInt();
    uint32_t yOfTile = QString("%1").arg(tileClass->getY()).toUInt();
    uint32_t zoom = QString("%1").arg(tileClass->getZoom()).toUInt();
    longitude = (xOfTile/pow(2,zoom))*360-180;
    lattitude = atan(sinh(M_PI-(yOfTile/pow(2,zoom))*(2*M_PI)))*(180/M_PI);
    longitudeOfTheTopRightCorner = (xOfTile/pow(2,zoom))*360-180;
    lattitudeOfTheTopRightCorner = atan(sinh(M_PI-((yOfTile+1)/pow(2,zoom))*(2*M_PI)))*(180/M_PI);
    longitudeOfTheBottomLeftCorner = ((xOfTile+1)/pow(2,zoom))*360-180;
    lattitudeOfTheBottomLeftCorner = atan(sinh(M_PI-(yOfTile/pow(2,zoom))*(2*M_PI)))*(180/M_PI);
    stepLattitude = (lattitude - lattitudeOfTheTopRightCorner)/256;
    stepLongitude = (longitudeOfTheBottomLeftCorner - longitude)/256;
    //qDebug()<<stepLattitude<<" "<<stepLongitude;
    pixmap.save(QString("offline_tiles/%0_100-l-%1-%2-%3-%4-%5-%6.png").arg("osm_custom").arg(1).arg(tileClass->getZoom()).arg(tileClass->getX()).arg(tileClass->getY())
                .arg(stepLattitude).arg(stepLongitude));
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



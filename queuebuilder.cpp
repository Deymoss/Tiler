#include "queuebuilder.h"

QueueBuilder::QueueBuilder(MainStruct data)
{
    this->data = data;
}

void QueueBuilder::run()
{
    osmscout::DatabaseParameter databaseParameter;
    osmscout::DatabaseRef       database=std::make_shared<osmscout::Database>(databaseParameter);
    osmscout::MapServiceRef     mapService=std::make_shared<osmscout::MapService>(database);
    if (!database->Open(data.map.toStdString())) {
        std::cerr << "Cannot open database" << std::endl;

        exit();
    }
    osmscout::StyleConfigRef styleConfig=std::make_shared<osmscout::StyleConfig>(database->GetTypeConfig());
    if (!styleConfig->Load(data.style.toStdString())) {
        std::cerr << "Cannot open style" << std::endl;
        exit();
    }
    osmscout::AreaSearchParameter searchParameter;

    searchParameter.SetUseLowZoomOptimization(true);
    searchParameter.SetMaximumAreaLevel(3);
    for (osmscout::MagnificationLevel level=osmscout::MagnificationLevel(std::min(data.startLevel,data.endLevel));
         level<=osmscout::MagnificationLevel(std::max(data.startLevel,data.endLevel));
         level++) {
        osmscout::Magnification magnification(level);

        osmscout::OSMTileId     tileA(osmscout::OSMTileId::GetOSMTile(magnification,
                                                                      osmscout::GeoCoord(data.latBottom,data.lonLeft)));
        osmscout::OSMTileId     tileB(osmscout::OSMTileId::GetOSMTile(magnification,
                                                                      osmscout::GeoCoord(data.latTop,data.lonRight)));
        uint32_t                xTileStart=std::min(tileA.GetX(),tileB.GetX());
        uint32_t                xTileEnd=std::max(tileA.GetX(),tileB.GetX());
        uint32_t                xTileCount=xTileEnd-xTileStart+1;
        uint32_t                yTileStart=std::min(tileA.GetY(),tileB.GetY());
        uint32_t                yTileEnd=std::max(tileA.GetY(),tileB.GetY());
        uint32_t                yTileCount=yTileEnd-yTileStart+1;

        std::cout << "Drawing zoom " << level << ", " << (xTileCount)*(yTileCount) << " tiles [" << xTileStart << "," << yTileStart << " - " <<  xTileEnd << "," << yTileEnd << "]" << std::endl;

        double minTime=std::numeric_limits<double>::max();
        double maxTime=0.0;
        double totalTime=0.0;

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

            if (hasLabel) {
                std::cout << "TYPE " << type->GetName() << " might have labels" << std::endl;
            }
        }
        for (uint32_t y=yTileStart; y<=yTileEnd; y++) {
            for (uint32_t x=xTileStart; x<=xTileEnd; x++) {
                tileData.x = x;
                tileData.y = y;
                tileData.zoom = level.Get();
                tileVector.push_back(tileData);
                qDebug()<<tileData.x<<" "<<tileData.y<<" "<<tileData.zoom;
            }
        }
    }
    
}

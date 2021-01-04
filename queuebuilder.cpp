#include "queuebuilder.h"

QueueBuilder::QueueBuilder(MainStruct data)
{
    this->data = data;
    mutex = new QMutex();
}

QueueBuilder::~QueueBuilder()
{
    delete mutex;
}

void QueueBuilder::run()
{
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

        std::cout << "Managing zoom " << level << ", " << (xTileCount)*(yTileCount) << " tiles [" << xTileStart << "," << yTileStart << " - " <<  xTileEnd << "," << yTileEnd << "]" << std::endl;

        osmscout::MapService::TypeDefinition typeDefinition;

        for (uint32_t y=yTileStart; y<=yTileEnd; y++) {
            for (uint32_t x=xTileStart; x<=xTileEnd; x++) {
                tileData.x = x;
                tileData.y = y;
                tileData.zoom = level.Get();
                tileData.map = data.map;
                tileData.style = data.style;
                tileVector.push_back(tileData);
                qDebug()<<tileData.x<<" "<<tileData.y<<" "<<tileData.zoom;
            }
        }
    }
    
}

TileStruct QueueBuilder::getNext()
{
    TileStruct output;

    mutex->lock();
    output = tileVector.last();
    tileVector.pop_back();
    mutex->unlock();
    return output;
}

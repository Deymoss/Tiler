#include "queuebuilder.h"

QueueBuilder::QueueBuilder(MainStruct data)
{
    currentData = data;
    mutex = new QMutex();
}

QueueBuilder::~QueueBuilder()
{
    delete mutex;
}

void QueueBuilder::startWork()
{
    start();
}

void QueueBuilder::pauseWork()
{

}

void QueueBuilder::stopWork()
{

}

void QueueBuilder::run()
{
    emit signalBegin();
    QTemporaryFile file("zoom-1");
    tileVector = new QVector<TileStruct*>();
    osmscout::AreaSearchParameter searchParameter;
    searchParameter.SetUseLowZoomOptimization(true);
    searchParameter.SetMaximumAreaLevel(3);
    for (osmscout::MagnificationLevel level=osmscout::MagnificationLevel(std::min(currentData.startLevel,currentData.endLevel));
         level<=osmscout::MagnificationLevel(std::max(currentData.startLevel,currentData.endLevel));
         level++) {

        osmscout::Magnification magnification(level);

        osmscout::OSMTileId     tileA(osmscout::OSMTileId::GetOSMTile(magnification,
                                                                      osmscout::GeoCoord(currentData.latBottom,currentData.lonLeft)));
        osmscout::OSMTileId     tileB(osmscout::OSMTileId::GetOSMTile(magnification,
                                                                      osmscout::GeoCoord(currentData.latTop,currentData.lonRight)));
        uint32_t                xTileStart=std::min(tileA.GetX(),tileB.GetX());
        uint32_t                xTileEnd=std::max(tileA.GetX(),tileB.GetX());
        uint32_t                xTileCount=xTileEnd-xTileStart+1;
        uint32_t                yTileStart=std::min(tileA.GetY(),tileB.GetY());
        uint32_t                yTileEnd=std::max(tileA.GetY(),tileB.GetY());
        uint32_t                yTileCount=yTileEnd-yTileStart+1;

        std::cout << "Managing zoom " << level << ", " << (xTileCount)*(yTileCount) << " tiles [" << xTileStart << "," << yTileStart << " - " <<  xTileEnd << "," << yTileEnd << "]" << std::endl;
        j += (xTileCount)*(yTileCount)-1;
        osmscout::MapService::TypeDefinition typeDefinition;
        for (uint32_t y=yTileStart; y<=yTileEnd; y++) {
            for (uint32_t x=xTileStart; x<=xTileEnd; x++) {
                tileData = new TileStruct;
                tileData->x = x;
                tileData->y = y;
                tileData->zoom = level.Get();
             //   tileData->map = currentData.map;
               // tileData->style = currentData.style;
                tileVector->append(tileData);
                //qDebug()<<tileData.x<<" "<<tileData.y<<" "<<tileData.zoom;
            }
        }
    }
    qDebug()<<tileVector->size()<<" "<<j;
    emit signalEnd();
}

TileStruct QueueBuilder::getNext()
{
    TileStruct *output = new TileStruct;

    mutex->lock();
    output = tileVector->at(j);
    if(j!=1){
    j--;
    }
    mutex->unlock();
    qDebug()<<output->x<<" "<<output->y<<" "<<output->zoom;
    return *output;
}

#include "queuebuilder.h"

uint16_t QueueBuilder::FillInLevel = 0;

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
    int i=0;
        QString fileName = "queue-"+QString().number(i+1);
        QTemporaryFile * file = new QTemporaryFile(QDir::tempPath() + "/TileQueue/" + fileName);
        filesVector.push_back(file);
    if(filesVector.at(0)->open())
    {
        qDebug()<<"Opened"<<filesVector.at(0)->fileName();
    }
    QDataStream dataStream(filesVector.at(0));
    long int counterOfTiles = 0;
    tileVector = new QVector<TileDataClass*>();
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

        osmscout::MapService::TypeDefinition typeDefinition;

        for (uint32_t y=yTileStart; y<=yTileEnd; y++) {
            for (uint32_t x=xTileStart; x<=xTileEnd; x++) {
                tileData = new TileDataClass(x,y,level.Get());
                if(counterOfTiles>=30000000)
                {
                    filesVector.at(i)->flush();
                    i++;
                    counterOfTiles = 0;
                    QTemporaryFile * file = new QTemporaryFile(QDir::tempPath() + "/TileQueue/" + fileName);
                    filesVector.push_back(file);
                    if(filesVector.at(i)->open())
                    {
                    qDebug()<<"Opened "<<filesVector.at(i)->fileName();
                    }
                    else
                    {
                        qDebug()<<"Not opened";
                    }
                    dataStream.setDevice(filesVector.at(i));
                }
                counterOfTiles++;
                //tileVector->append(tileData);
                dataStream << TileDataClass(tileData->getX(),tileData->getY(),tileData->getZoom());
                //qDebug()<<tileData->getX()<<" "<<tileData->getY()<<" "<<tileData->getZoom();
                delete tileData;
            }
        }
       filesVector.at(i)->flush();

    }
    for(int j=0; j<filesVector.size();j++)
    {

     filesVector.at(j)->close();
    }
    FillInVector();
    emit signalEnd();
}

TileDataClass* QueueBuilder::getNext()
{
    TileDataClass *output = new TileDataClass(0,0,0);

    mutex->lock();
    output = tileVector->last();
    tileVector->pop_back();
    mutex->unlock();
    if(tileVector->size() == 1)
    {
        FillInVector();
    }
    //qDebug()<<output->x<<" "<<output->y<<" "<<output->zoom;
    return output;
}

QVector<TileDataClass *> QueueBuilder::FillInVector()
{

        if(filesVector.at(FillInLevel)->open())
        {
            qDebug()<<"Opened "<<FillInLevel;
        }
        QDataStream dataStream(filesVector.at(FillInLevel));
        dataStream.setDevice(filesVector.at(FillInLevel));
        while(!dataStream.atEnd())
        {
            TileDataClass *tiles = new TileDataClass();
            dataStream>>*tiles;
            tileVector->append(tiles);
        }
        filesVector.at(FillInLevel)->close();
        FillInLevel++;
    qDebug()<<tileVector->size();
    //qDebug()<<tileVector->at(1)->getX()<<" "<<tileVector->at(2)->getX()<<" "<<tileVector->at(50)->getX();
    return *tileVector;
}
/*
 * автоматически определяется оператива и 10% от оперативы кусок
 * или вручную
 */

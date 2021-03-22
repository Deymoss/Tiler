#include "queuebuilder.h"

uint16_t QueueBuilder::FillInLevel = 0;

QueueBuilder::QueueBuilder(MainStruct data)
{
    if(!QDir("/tmp/TileQueue").exists())
        QDir("/tmp/").mkdir("TileQueue");
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

QString QueueBuilder::getMapPath()
{
    return currentData.map;
}

QString QueueBuilder::getStypePath()
{
    return currentData.style;
}

void QueueBuilder::run()
{

    emit signalBegin();
    double lattitude, longitude, longitudeOfTheTopRightCorner, lattitudeOfTheTopRightCorner, longitudeOfTheBottomLeftCorner,lattitudeOfTheBottomLeftCorner;
    double stepLattitude, stepLongitude;
    qRegisterMetaType<QVector<QTemporaryFile*> >("QVector<QTemporaryFile*>");
    qRegisterMetaType<QVector<ConstantStruct> >("QVector<ConstantStruct>");
    ConstantStruct constants;
    int i=0;

    /*This is temporary files for saving coordinate structs, to avoid storing all structs from all zoom ​​in a vector an in a RAM*/
    QString fileName = "queue-"+QString().number(i+1);
    QTemporaryFile * file = new QTemporaryFile(QDir::tempPath() + "/TileQueue/" + fileName);
    filesVector.push_back(file);

    if(filesVector.at(0)->open())
    {
        qDebug()<<"Opened"<<filesVector.at(0)->fileName();
    }

    QDataStream dataStream(filesVector.at(0));//QDataStream for serializing structs
    long int counterOfTiles = 0;
    tileVector = new QVector<TileDataClass*>();

    osmscout::log.Debug(OsmScoutDebug);
       osmscout::log.Info(true);
       osmscout::log.Warn(true);
       osmscout::log.Error(true);

    osmscout::AreaSearchParameter searchParameter;
    searchParameter.SetUseLowZoomOptimization(true);
    searchParameter.SetMaximumAreaLevel(3);

    for (osmscout::MagnificationLevel level=osmscout::MagnificationLevel(std::min(currentData.startLevel,currentData.endLevel));
         level<=osmscout::MagnificationLevel(std::max(currentData.startLevel,currentData.endLevel));
         level++) {

        /* on this step i'm make a queue of x y z of tile for later rendering*/

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

        quint32 CountOfTiles = (xTileCount)*(yTileCount);
        emit signalThrowCountOfFiles(CountOfTiles);
        osmscout::MapService::TypeDefinition typeDefinition;

        for (quint32 y=yTileStart; y<=yTileEnd; y++) {
            for (quint32 x=xTileStart; x<=xTileEnd; x++) {
                // qDebug()<<x<<" "<<y;
                tileData = new TileDataClass(x,y,level.Get(),0,0,0,0);
                if(counterOfTiles>=30000000)/* 30 millions it's close to 2 gb RAM, fits to me */
                {
                    /* on this step i'm calculating longitude and lattitude of 1 pixel for the subsequent drawing of the route of the members of the search party*/
                    countLatLon(x,y,level.Get());
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
                dataStream << TileDataClass(tileData->x,tileData->y,tileData->zoom,stepLongitude, stepLattitude, 0,0);//output data to file
                delete tileData;
            }
        }
        filesVector.at(i)->flush();
        constants.countOfTiles = (xTileCount)*(yTileCount);/*in this step i'm saving data for header of my binary file which contains information about the tiles
                                                             and the tiles themselves, and thanks to these constants I can do a O(1) search*/
        constants.xTileStart = xTileStart;
        constants.yTileStart = yTileStart;
        constants.xTileCount = xTileCount;
        constants.yTileCount = yTileCount;
        constantVector.append(constants);
    }
    while(constantVector.size()!=20)//if i rendered <20 zooms, doesen't rendered zooms = 0
    {
        constants.countOfTiles = 0;
        constants.xTileStart = 0;
        constants.yTileStart = 0;
        constants.xTileCount = 0;
        constants.yTileCount = 0;
        constantVector.append(constants);
    }
    for(int j=0; j<filesVector.size();j++)//closing all files to throw them to saveToFileClass
    {

        filesVector.at(j)->close();
    }
    FindNecessaryTile *fnc = new FindNecessaryTile(constantVector);
    fnc->getTile(71,42,7);//just test of searching func
    FillInVector();
    emit signalEnd();
    this->exec();
}

void QueueBuilder::countLatLon(uint32_t x, uint32_t y, uint32_t zoom)
{
    longitude = (x/pow(2,zoom))*360-180;
    lattitude = atan(sinh(M_PI-(y/pow(2,zoom))*(2*M_PI)))*(180/M_PI);
    longitudeOfTheTopRightCorner = (x/pow(2,zoom))*360-180;
    lattitudeOfTheTopRightCorner = atan(sinh(M_PI-((y+1)/pow(2,zoom))*(2*M_PI)))*(180/M_PI);
    longitudeOfTheBottomLeftCorner = ((x+1)/pow(2,zoom))*360-180;
    lattitudeOfTheBottomLeftCorner = atan(sinh(M_PI-(y/pow(2,zoom))*(2*M_PI)))*(180/M_PI);
    stepLattitude = (lattitude - lattitudeOfTheTopRightCorner)/256;
    stepLongitude = (longitudeOfTheBottomLeftCorner - longitude)/256;
}

TileDataClass* QueueBuilder::getNext()//this function is responsible for throwing file information for the render class
{
    counterTiles++;
     mutex->lock();
    if(tileVector->size()>0)
    {
        output = tileVector->last();
        tileVector->pop_back();
        mutex->unlock();
    }
    else if(filesVector.size() != FillInLevel)
    {
        tileVector->clear();
        FillInVector();
        mutex->unlock();
    }
    else
    {
        emit signalRenderFinished(filesVector,constantVector);
    }
    return output;
}

QVector<TileDataClass *> QueueBuilder::FillInVector()//if vector out of structs, this fucn open next Temp file and fills the vector with x y zoom of tiles.
{
    if(filesVector.size() != FillInLevel)
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
        mutex->unlock();
        return *tileVector;
    }
    else
    {
        qDebug()<<"EMIT";

    }


}

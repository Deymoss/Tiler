#include "savetofileclass.h"

SaveToFileClass::SaveToFileClass(QVector<QTemporaryFile*> files, QVector<ConstantStruct> constants)
{
    this->files = files;
    this->constants = constants;
    qRegisterMetaType<uint32_t> ("uint32_t");
}

SaveToFileClass::~SaveToFileClass()
{

}
void SaveToFileClass::run()
{
    QFile file("file.bin");
    if(file.open(QIODevice::WriteOnly))
    {

        QDataStream stream(&file);
        for(int i =0; i<constants.size(); i++)//запись констант в файл
        {
            stream<<constants.at(i).countOfTiles;
            stream<<constants.at(i).xTileStart;
            stream<<constants.at(i).yTileStart;
            stream<<constants.at(i).xTileCount;
            stream<<constants.at(i).yTileCount;
        }
        qDebug()<<sizeof(constants.at(0))<< "ffck ";
        int countInputTiles = 0;
        for(int i=0;i<files.size();i++)//запись структур с данными о тайлах
        {
            files.at(i)->open();
            QDataStream dataStream(files.at(i));
            while(!dataStream.atEnd())
            {
                TileDataClass *tiles = new TileDataClass();
                dataStream>>*tiles;
                countInputTiles++;
                stream<<*tiles;
                delete tiles;
            }
            files.at(i)->close();
        }
        file.close();
        file.open(QIODevice::ReadWrite);
        file.seek(sizeof(constants.at(0))*constants.size());
        QDataStream dataStream(&file);
        int countOutputTiles = 0;
       // qDebug()<<countInputTiles<<" "<<countOutputTiles;
        while(countOutputTiles!=countInputTiles)//вывод и редактирование структур с учётом информации о размещении самой картинки
        {
            TileDataClass *tiles = new TileDataClass();
            dataStream>>*tiles;

            QString a = "offline_tiles/osm_custom_100-l-1-"+QString::number(tiles->zoom)+"-"+QString::number(tiles->x)+"-"+QString::number(tiles->y)+".png";
            QFile tilePic(a);
            tilePic.open(QIODevice::ReadOnly);
            tiles->size = tilePic.size();

            tiles->startPoint = file.size();
            file.seek(sizeof(constants.at(0))*constants.size()+sizeof(TileDataClass)*countOutputTiles);
            dataStream<<*tiles;
            file.seek(tiles->startPoint); 
           // qDebug()<<tiles->startPoint<<" "<<tiles->size;
            file.write(tilePic.readAll());
            countOutputTiles++;
            file.seek(sizeof(constants.at(0))*constants.size()+sizeof(TileDataClass)*countOutputTiles);

        }
        if(stream.status() != QDataStream::Ok)
        {
            qDebug() << "Ошибка записи";
        }//отправить сигнал который оповестит о завершении записи в файл, после этого запросить картинку из интерфейса и пробросить её в виджет для вывода.
        QElapsedTimer timer;
        timer.start();
        getTile(147,82,8);
        qDebug() << "The slow operation took " << timer.nsecsElapsed() << " nanoseconds";
        exit(0);
    }
    else
    {
        qDebug()<<"Файл не открыт";
    }
    this->exec();

}

void SaveToFileClass::getTile(int x, int y, int zoom)
{

    int countOfXNum = x-constants.at(zoom-1).xTileStart;
    int countOfYnum = y - constants.at(zoom-1).yTileStart;
    int countTls = constants.at(zoom-1).xTileCount*countOfYnum + countOfXNum;
    int result = 0;
    for(int k = 0; k<zoom-1; k++)
    {
        result += constants.at(k).countOfTiles;
    }
    countTls += result;
    QFile file("file.bin");
    if(file.open(QIODevice::ReadOnly))
    {
        TileDataClass *tile = new TileDataClass(0,0,0,0,0);
        file.seek(400 + sizeof(TileDataClass)*(countTls));
        QDataStream dataStream(&file);
        dataStream>>*tile;

        emit signalForEnd(tile->startPoint, tile->size);
    }

    //return countTls;
}

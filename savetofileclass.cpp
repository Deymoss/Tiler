#include "savetofileclass.h"

SaveToFileClass::SaveToFileClass(QVector<QTemporaryFile*> files, QVector<ConstantStruct> constants)
{

    QFile file("file.bin");
    if(file.open(QIODevice::WriteOnly))
    {
        QDataStream stream(&file);
        for(int i =0; i<constants.size(); i++)
        {
            stream<<constants.at(i).countOfTiles;
            stream<<constants.at(i).xTileStart;
            stream<<constants.at(i).yTileStart;
            stream<<constants.at(i).xTileCount;
            stream<<constants.at(i).yTileCount;
        }

        int countInputTiles = 0;
        for(int i=0;i<files.size();i++)
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
        //file.seek(sizeof(constants.at(0))*constants.size());
        file.seek(sizeof(constants.at(0))*constants.size());
        QDataStream dataStream(&file);
        int countOutputTiles = 0;
        while(countOutputTiles!=countInputTiles)
        {
            TileDataClass *tiles = new TileDataClass();
            dataStream>>*tiles;
            QString a = "offline_tiles/osm_custom_100-l-1-"+QString::number(tiles->zoom)+"-"+QString::number(tiles->x)+"-"+QString::number(tiles->y)+".png";
            QFile tilePic(a);
            tilePic.open(QIODevice::ReadOnly);
            tiles->size = tilePic.size();
            tiles->startPoint = file.size() - 1;
            file.seek(sizeof(constants.at(0))*constants.size()+sizeof(TileDataClass)*countOutputTiles);//записать в конец файла картинку
            dataStream<<*tiles;
            file.seek(tiles->startPoint);
            file.write(tilePic.readAll());
            countOutputTiles++;

        }
        if(stream.status() != QDataStream::Ok)
        {
            qDebug() << "Ошибка записи";
        }

    }
    else
    {
        qDebug()<<"Файл не открыт";
    }


}

SaveToFileClass::~SaveToFileClass()
{

}

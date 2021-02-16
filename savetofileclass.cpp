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

        tileVector = new QVector<TileDataClass*>();
        for(int i=0;i<files.size();i++)
        {
            files.at(i)->open();
            QDataStream dataStream(files.at(i));
            while(!dataStream.atEnd())
            {
                TileDataClass *tiles = new TileDataClass();
                dataStream>>*tiles;
                stream<<*tiles;
                delete tiles;
            }
            files.at(i)->close();
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

#ifndef SAVETOFILECLASS_H
#define SAVETOFILECLASS_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QTemporaryFile>
#include <QFileDevice>
#include <QImage>
#include "tiledataclass.h"
#include "MainStruct.h"

class SaveToFileClass : public QObject
{
    Q_OBJECT
public:
    SaveToFileClass(QVector<QTemporaryFile*> files, QVector<ConstantStruct> constants);
    QVector<TileDataClass*> *tileVector;
    ~SaveToFileClass();
};

#endif // SAVETOFILECLASS_H

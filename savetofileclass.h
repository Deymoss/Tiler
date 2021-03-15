#ifndef SAVETOFILECLASS_H
#define SAVETOFILECLASS_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <QTemporaryFile>
#include <QFileDevice>
#include <QImage>
#include <QThread>
#include "tiledataclass.h"
#include "MainStruct.h"

class SaveToFileClass : public QThread
{
    Q_OBJECT
public:
    SaveToFileClass(QVector<QTemporaryFile*> files, QVector<ConstantStruct> constants);
    QVector<TileDataClass*> *tileVector;
    QVector<ConstantStruct> constants;
    QVector<QTemporaryFile*> files;
    void run() override;
    void getTile(int x, int y, int zoom);
    ~SaveToFileClass();
signals:
    void signalForEnd();
};

#endif // SAVETOFILECLASS_H

#ifndef THREADCLASS_H
#define THREADCLASS_H

#include <QObject>
#include <QThread>
#include "MainStruct.h"

#include <QPainter>
#include <QDir>

#include <iostream>
#include <iomanip>
#include <QtDebug>
#include <limits>
#include <QVector>
#include <QMutex>

#include <osmscout/Database.h>
#include <osmscout/MapService.h>

#include <osmscout/MapPainterQt.h>

#include <osmscout/util/StopClock.h>
#include <osmscout/util/Tiling.h>
class QueueBuilder : public QThread
{
    Q_OBJECT
public:
    QueueBuilder(MainStruct data);
    ~QueueBuilder();
    void run() override;
    TileStruct getNext();
private:
    MainStruct data;
    TileStruct tileData;
    QMutex *mutex;
    QVector<TileStruct*> tileVector;
    QVector<TileStruct*> copyTileVector;
signals:
    void signalBegin();//когда начинает строить
    void signalEnd();//когда завершилось построение
};

#endif // THREADCLASS_H

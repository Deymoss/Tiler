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
#include <QTemporaryFile>
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
    void startWork();
    void pauseWork();
    void stopWork();
    TileStruct getNext();
private:
    MainStruct currentData;
    void run() override;
    TileStruct *tileData;
    QMutex *mutex;
    int j = 0;
    QVector<TileStruct*> *tileVector;
signals:
    void signalBegin();//когда начинает строить
    void signalEnd();//когда завершилось построениe
    void signalLastElement();
};

#endif // THREADCLASS_H

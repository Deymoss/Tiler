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
    void run() override;
private:
    MainStruct data;
    TileStruct tileData;
    QVector<TileStruct> tileVector;
signals:
    void zoomFinished();
};

#endif // THREADCLASS_H

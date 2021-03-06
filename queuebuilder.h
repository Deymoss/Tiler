#ifndef THREADCLASS_H
#define THREADCLASS_H

#include <QObject>
#include <QThread>
#include "MainStruct.h"
#include "tiledataclass.h"

#include <QPainter>
#include <QDir>
#include <limits>
#include <QMessageBox>
#include "findnecessarytile.h"
#include "savetofileclass.h"

#include <iostream>
#include <iomanip>
#include <QtDebug>
#include <limits>
#include <QVector>
#include <QTemporaryFile>
#include <QDataStream>
#include <QMutex>

#include <osmscout/Database.h>
#include <osmscout/MapService.h>

#include <osmscout/MapPainterQt.h>
#include <osmscout/util/StopClock.h>
#include <osmscout/util/Tiling.h>
constexpr bool OsmScoutDebug = false;
class QueueBuilder : public QThread
{
    Q_OBJECT
public:
    QueueBuilder(MainStruct data);
    ~QueueBuilder();

    void startWork();
    QString getMapPath();
    QString getStypePath();
    TileDataClass *getNext();
    TileDataClass *output;
    QVector<TileDataClass *> FillInVector();
    static int openFileCounter;
private:
    MainStruct currentData;
    void run() override;
    void countLatLon(uint32_t x, uint32_t y, uint32_t zoom);
    TileDataClass *tileData;
    QVector<QTemporaryFile*> filesVector;
    QVector<ConstantStruct> constantVector;
    static uint16_t FillInLevel;
    double lattitude, longitude, longitudeOfTheTopRightCorner, lattitudeOfTheTopRightCorner, longitudeOfTheBottomLeftCorner,lattitudeOfTheBottomLeftCorner;
    double stepLattitude;
    double stepLongitude;
    QMutex *mutex;
    int j = 0;
    int counterTiles = 0;
    QVector<TileDataClass*> *tileVector;
signals:
    void signalBegin();
    void signalEnd();
    void signalLastElement();
    void signalRenderFinished(QVector<QTemporaryFile*> files, QVector<ConstantStruct> constants);
    void signalThrowCountOfFiles(quint32 count);
};

#endif // THREADCLASS_H

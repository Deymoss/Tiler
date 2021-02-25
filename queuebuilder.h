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
class QueueBuilder : public QThread
{
    Q_OBJECT
public:
    QueueBuilder(MainStruct data);
    ~QueueBuilder();
    void startWork();
    QString getMapPath();
    QString getStypePath();
    TileDataClass* getNext();
    QVector<TileDataClass *> FillInVector();
    static int openFileCounter;
private:
    MainStruct currentData;
    void run() override;
    TileDataClass *tileData;
    QVector<QTemporaryFile*> filesVector;
    QVector<ConstantStruct> constantVector;
    static uint16_t FillInLevel;
    QMutex *mutex;
    int j = 0;
    QVector<TileDataClass*> *tileVector;
signals:
    void signalBegin();
    void signalEnd();
    void signalLastElement();
    void signalRenderFinished(QVector<QTemporaryFile*> files, QVector<ConstantStruct> constants);
    void signalThrowCountOfFiles(quint32 count);
};

#endif // THREADCLASS_H

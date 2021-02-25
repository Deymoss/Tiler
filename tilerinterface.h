#ifndef TILERINTERFACE_H
#define TILERINTERFACE_H

#include <QObject>
#include "MainStruct.h"
#include "queuebuilder.h"
#include <QString>
#include <QDebug>
#include "renderclass.h"
#include "savetofileclass.h"

class TilerInterface : public QObject
{
    Q_OBJECT
    Q_ENUMS(errors)
public:
    TilerInterface();
    ~TilerInterface();
    enum errors{errorNullData};//написать все ошибки
    void setMap(QString map);
    void setStyle(QString style);
    void setLatTop(double latTop);
    void setLatBottom(double latBottom);
    void setLonLeft(double lonLeft);
    void setLonRight(double lonRight);
    void setStartLevel(double startLevel);
    void setEndLevel(double endLevel);
    void setPluginName(QString pluginName);
    QString getMap();
    QString getStyle();
    double getLatTop();
    double getLatBottom();
    double getLonLeft();
    double getLonRight();
    unsigned int getStartLevel();
    unsigned int getEndLevel();
    QString getPluginName();
    void checkData();
    void throwData(MainStruct data);
    QueueBuilder * builder;
private:
    QString  map;
    QString  style;
    int countOfThreads = 4;
    QVector<RenderClass*> renderThreads;
    QVector<QTemporaryFile*> filesVector;
    QVector<ConstantStruct> constants;
    SaveToFileClass *saveToFile;
    double   latTop;
    double latBottom;
    double lonLeft;
    double lonRight;
    unsigned int startLevel;
    unsigned int endLevel;
    QString pluginName;
signals:
    void signalError(TilerInterface::errors err);//enum с ошибками
    void signalToWidget();
    void signalEndOfSaving();
    void signalThrowDataToWidget(quint32 count);
private slots:
    void slotBegin();
    void slotEnd();
    void slotLastElement(QVector<QTemporaryFile*> files, QVector<ConstantStruct> constants);
    void slotFinishedRenderTile();
    void slotCountOfTiles(quint32 count);
    void endOfSaving();

};

#endif // TILERINTERFACE_H

#ifndef TILERINTERFACE_H
#define TILERINTERFACE_H

#include <QObject>
#include "MainStruct.h"
#include "queuebuilder.h"
#include <QString>
#include <QDebug>
#include "renderclass.h"

class TilerInterface : public QObject
{
    Q_OBJECT
public:
    TilerInterface();
    ~TilerInterface();
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
    double   latTop;
    double latBottom;
    double lonLeft;
    double lonRight;
    unsigned int startLevel;
    unsigned int endLevel;
    QString pluginName;

signals:
    void signalError();//enum с ошибками
private slots:
    void slotBegin();
    void slotEnd();
};

#endif // TILERINTERFACE_H

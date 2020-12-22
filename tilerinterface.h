#ifndef TILERINTERFACE_H
#define TILERINTERFACE_H

#include <QObject>
#include "MainStruct.h"
#include <QString>
#include <QDebug>

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
private:
    QString  map;
    QString  style;
    double   latTop;
    double latBottom;
    double lonLeft;
    double lonRight;
    unsigned int startLevel;
    unsigned int endLevel;
    QString pluginName;
};

#endif // TILERINTERFACE_H

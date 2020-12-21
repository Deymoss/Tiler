#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QDir>

#include <iostream>
#include <iomanip>
#include <QtDebug>
#include <limits>
#include "threadclass.h"
#include "MainStruct.h"

#include <osmscout/Database.h>
#include <osmscout/MapService.h>

#include <osmscout/MapPainterQt.h>

#include <osmscout/util/StopClock.h>
#include <osmscout/util/Tiling.h>

class Widget : public QWidget
{
    Q_OBJECT
///home/deymos/imported_data /home/deymos/SIMURAN/libosmscout/stylesheets/standard.oss 56.529 21.094 50.938 38.342 1 12 osm_custom
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QGridLayout * mainLayout;
    QLineEdit * importedDataEdit;
    QLineEdit * stylesheetEdit;
    QLineEdit * lattitudeTopEdit;
    QLineEdit * longitudeLeftEdit;
    QLineEdit * lattitudeBottomEdit;
    QLineEdit * longitudeRightEdit;
    QLineEdit * startZoomEdit;
    QLineEdit * endZoomEdit;
    QLineEdit * pluginNameEdit;
    QLabel * importedDataLabel;
    QLabel * stylesheetLabel;
    QLabel * lattitudeTopLabel;
    QLabel * longitudeLeftLabel;
    QLabel * lattitudeBottomLabel;
    QLabel * longitudeRightLabel;
    QLabel * startZoomLabel;
    QLabel * endZoomLabel;
    QLabel * pluginNameLabel;
    QPushButton * startButton;
    void MergeTilesToMapData(const std::list<osmscout::TileRef>& centerTiles,
                             const osmscout::MapService::TypeDefinition& ringTypeDefinition,
                             const std::list<osmscout::TileRef>& ringTiles,
                             osmscout::MapData& data);
public slots:
    void drawTile();
};
#endif // WIDGET_H

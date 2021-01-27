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
#include "MainStruct.h"
#include "tilerinterface.h"

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
    TilerInterface * interface;
public slots:
    void drawTile();
    void slotError(TilerInterface::errors err);
};
#endif // WIDGET_H

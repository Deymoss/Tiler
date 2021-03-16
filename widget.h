#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QPainter>
#include <QDir>
#include <QProgressBar>
#include <QTimer>
#include <QStatusBar>
#include <QImage>
#include <QFile>
#include <QPixmap>

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
    QLabel * speedLabel;
    QLabel * imageLabel;
    QPushButton * startButton;
    QProgressBar * progressBar;
    TilerInterface * interface;
    QTimer * timer;
    long int countOfTiles;
    long int tilesFinished = 0;
    int counter = 0;
    void plusProgress();
signals:
    void signalMessage();
public slots:
    void drawTile();
    void slotError(TilerInterface::errors err);
    void slotTileRendered();
    void slotTakeDataTiles(quint32 count);
    void slotSpeed();
    void showImg(uint32_t one, uint32_t two);
};
#endif // WIDGET_H

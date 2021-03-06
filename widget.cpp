﻿#include "widget.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    resize(1280,720);
    mainLayout = new QGridLayout(this);
    importedDataEdit = new QLineEdit(this);
    stylesheetEdit = new QLineEdit(this);
    lattitudeTopEdit = new QLineEdit(this);
    longitudeLeftEdit = new QLineEdit(this);
    lattitudeBottomEdit = new QLineEdit(this);
    longitudeRightEdit = new QLineEdit(this);
    startZoomEdit = new QLineEdit(this);
    endZoomEdit = new QLineEdit(this);
    pluginNameEdit = new QLineEdit(this);
    stylesheetLabel = new QLabel(tr("Enter stylesheet Path: "),this);
    importedDataLabel = new QLabel(tr("Enter importedData Path: "),this);
    lattitudeTopLabel = new QLabel(tr("Enter top lattitude: "),this);
    longitudeLeftLabel = new QLabel(tr("Enter left longitude: "),this);
    lattitudeBottomLabel = new QLabel(tr("Enter bottom lattitude: "),this);
    longitudeRightLabel = new QLabel(tr("Enter right longitude: "),this);
    startZoomLabel = new QLabel(tr("Enter start zoom level: "),this);
    endZoomLabel = new QLabel(tr("Enter end zoom level: "),this);
    pluginNameLabel = new QLabel(tr("Enter name of plugin: "),this);
    speedLabel = new QLabel(this);
    startButton = new QPushButton(tr("Start"),this);
    progressBar = new QProgressBar(this);
    timer = new QTimer(this);
    imageLabel = new QLabel(tr("fuck"),this);
    progressBar->setMinimum(0);
    mainLayout->addWidget(importedDataLabel,0,0,1,1);
    mainLayout->addWidget(stylesheetLabel,1,0,1,1);
    mainLayout->addWidget(lattitudeTopLabel,2,0,1,1);
    mainLayout->addWidget(longitudeLeftLabel,3,0,1,1);
    mainLayout->addWidget(lattitudeBottomLabel,4,0,1,1);
    mainLayout->addWidget(longitudeRightLabel,5,0,1,1);
    mainLayout->addWidget(startZoomLabel,6,0,1,1);
    mainLayout->addWidget(endZoomLabel,7,0,1,1);
    mainLayout->addWidget(pluginNameLabel,8,0,1,1);
    mainLayout->addWidget(importedDataEdit,0,1,1,1);
    mainLayout->addWidget(imageLabel,1,2,1,1);
    imageLabel->setMinimumHeight(256);
    imageLabel->setMinimumWidth(256);
    importedDataEdit->setText("/home/deymos/imported_data");
    mainLayout->addWidget(stylesheetEdit,1,1,1,1);
    stylesheetEdit->setText("/home/deymos/SIMURAN/libosmscout/stylesheets/standard.oss");
    mainLayout->addWidget(lattitudeTopEdit,2,1,1,1);
    lattitudeTopEdit->setText("56.529");
    mainLayout->addWidget(longitudeLeftEdit,3,1,1,1);
    longitudeLeftEdit->setText("21.094");
    mainLayout->addWidget(lattitudeBottomEdit,4,1,1,1);
    lattitudeBottomEdit->setText("50.938");
    mainLayout->addWidget(longitudeRightEdit,5,1,1,1);
    longitudeRightEdit->setText("38.342");
    mainLayout->addWidget(startZoomEdit,6,1,1,1);
    mainLayout->addWidget(endZoomEdit,7,1,1,1);
    mainLayout->addWidget(pluginNameEdit,8,1,1,1);
    pluginNameEdit->setText("osm_custom");
    mainLayout->addWidget(startButton,11,0,1,2);
    mainLayout->addWidget(progressBar,9,0,1,2);
    mainLayout->addWidget(speedLabel,10,0,1,1);
    speedLabel->setMaximumHeight(20);
    speedLabel->setMaximumWidth(100);
    timer->setInterval(1000);
    connect(startButton, &QPushButton::clicked, this, &Widget::drawTile);
    connect(timer,&QTimer::timeout,this,&Widget::slotSpeed);


}

Widget::~Widget()
{
}

void Widget::plusProgress()
{
    progressBar->setMaximum(countOfTiles);
    QString text = QString("%1 of %2").arg(tilesFinished).arg(countOfTiles);
    progressBar->setFormat(text);
    progressBar->setValue(progressBar->value()+1);
    counter++;

}
void Widget::drawTile()
{
    interface = new TilerInterface();
    connect(interface, &TilerInterface::signalEndOfSaving, this, &Widget::showImg);
    connect(interface, SIGNAL(signalError(TilerInterface::errors)),this,SLOT(slotError(TilerInterface::errors)));
    connect(interface,SIGNAL(signalToWidget()),this,SLOT(slotTileRendered()));
    connect(interface, &TilerInterface::signalThrowDataToWidget,this,&Widget::slotTakeDataTiles);
    QString  map;
    QString  style;
    double       latTop,latBottom,lonLeft,lonRight;
    unsigned int startLevel;
    unsigned int endLevel;

    map=importedDataEdit->text();
    style=stylesheetEdit->text();

    QString pluginName = pluginNameEdit->text();
    latTop = lattitudeTopEdit->text().toDouble();
    latBottom = lattitudeBottomEdit->text().toDouble();
    lonLeft = longitudeLeftEdit->text().toDouble();
    lonRight = longitudeRightEdit->text().toDouble();
    startLevel = startZoomEdit->text().toUInt();
    endLevel = endZoomEdit->text().toUInt();
    interface->setMap(map);
    interface->setStyle(style);
    interface->setLatTop(latTop);
    interface->setLatBottom(latBottom);
    interface->setLonLeft(lonLeft);
    interface->setLonRight(lonRight);
    interface->setStartLevel(startLevel);
    interface->setEndLevel(endLevel);
    interface->setPluginName(pluginName);
    interface->checkData();
}

void Widget::slotError(TilerInterface::errors err)
{

}

void Widget::slotTileRendered()
{
    plusProgress();
    tilesFinished++;
}

void Widget::slotTakeDataTiles(quint32 count)
{
    countOfTiles = count;
    timer->start();
}

void Widget::slotSpeed()
{
    speedLabel->setText(QString::number(counter)+" tiles/s");
    speedLabel->setAlignment(Qt::AlignCenter);
    counter= 0;

}

void Widget::showImg(uint32_t one, uint32_t two)
{
  QPixmap pixmap;
  QByteArray arr;
  QFile file("file.bin");
  if(file.open(QIODevice::ReadOnly))
  {
  QDataStream stream(&file);
  file.seek(one);
  arr = file.read(two);
  QPixmap img;
  img.loadFromData(arr);
  QImage image;
  image.fromData(arr);
  imageLabel->setPixmap(img);
  this->update();
  }
  else
  {
      qDebug()<<"ne otkryto";
  }
}

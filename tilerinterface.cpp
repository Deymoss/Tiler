#include "tilerinterface.h"

TilerInterface::TilerInterface()
{

}

TilerInterface::~TilerInterface()
{

}

void TilerInterface::checkData()
{
   if(map != NULL && style !=NULL && latTop && latBottom && lonLeft && lonRight && startLevel && endLevel && pluginName != NULL)
   {
       MainStruct data;
       data.map = map;
       data.style = style;
       data.latTop = latTop;
       data.latBottom = latBottom;
       data.lonLeft = lonLeft;
       data.lonRight = lonRight;
       data.startLevel = startLevel;
       data.endLevel = endLevel;
       data.pluginName = pluginName;
       throwData(data);
   }
   else
   {
       qDebug()<<"some data is empty";
   }
}

void TilerInterface::throwData(MainStruct data)
{

}
void TilerInterface::setMap(QString map)
{
    this->map = map;
}

void TilerInterface::setStyle(QString style)
{
    this->style = style;
}

void TilerInterface::setLatTop(double latTop)
{
    this->latTop = latTop;
}

void TilerInterface::setLatBottom(double latBottom)
{
    this->latBottom = latBottom;
}

void TilerInterface::setLonLeft(double lonLeft)
{
    this->lonLeft = lonLeft;
}

void TilerInterface::setLonRight(double lonRight)
{
    this->lonRight = lonRight;
}

void TilerInterface::setStartLevel(double startLevel)
{
    this->startLevel = startLevel;
}

void TilerInterface::setEndLevel(double endLevel)
{
    this->endLevel = endLevel;
}

void TilerInterface::setPluginName(QString pluginName)
{
    this->pluginName = pluginName;
}

QString TilerInterface::getMap()
{
    return map;
}

QString TilerInterface::getStyle()
{
    return style;
}

double TilerInterface::getLatBottom()
{
    return latBottom;
}

double TilerInterface::getLonLeft()
{
    return lonLeft;
}

double TilerInterface::getLonRight()
{
    return lonRight;
}

unsigned int TilerInterface::getStartLevel()
{
    return startLevel;
}

unsigned int TilerInterface::getEndLevel()
{
    return endLevel;
}

QString TilerInterface::getPluginName()
{
    return pluginName;
}

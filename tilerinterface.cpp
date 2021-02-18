#include "tilerinterface.h"

TilerInterface::TilerInterface()
{

}

TilerInterface::~TilerInterface()
{

}

void TilerInterface::checkData()
{
    //сделать проверку каждого параметра отдельно и ошибку для каждого параметра
   if(!map.isEmpty() && !style.isEmpty() && latTop && latBottom && lonLeft && lonRight && startLevel && endLevel && !pluginName.isEmpty())
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
       qDebug()<<"some data is empty";//при оконании
   }
}

void TilerInterface::throwData(MainStruct data)
{
    builder = new QueueBuilder(data);
//    connect(builder, SIGNAL(signalThrowCountOfFiles(long)),this,SLOT(slotCountOfTiles(long)));
    connect(builder, &QueueBuilder::signalThrowCountOfFiles,this, &TilerInterface::slotCountOfTiles);
    connect(builder, SIGNAL(signalBegin()),this,SLOT(slotBegin()));
    connect(builder, &QueueBuilder::signalEnd, this, &TilerInterface::slotEnd);
    builder->startWork();
}

void TilerInterface::slotBegin()
{
    //при старте
}

void TilerInterface::slotEnd(QVector<QTemporaryFile*> files, QVector<ConstantStruct> constants)
{
//    при окончании
    this->constants = constants;
    countOfThreads = 4;
    RenderClass *renderClass;
    filesVector = files;
    for(int i=0;i<countOfThreads;i++)
    {
        renderClass = new RenderClass(builder);
        renderThreads.push_back(renderClass);
        connect(renderClass,SIGNAL(endOfRender()),this,SLOT(slotFinishedRenderTile()));
    }
}

void TilerInterface::slotLastElement()
{

}

void TilerInterface::slotFinishedRenderTile()
{
    emit signalToWidget();
}

void TilerInterface::slotCountOfTiles(quint32 count)
{
    emit throwDataToWidget(count);
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

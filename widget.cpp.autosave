#include "widget.h"
static const unsigned int tileWidth=256;
static const unsigned int tileHeight=256;
static const double       DPI=96.0;
static const int          tileRingSize=1;
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
    startButton = new QPushButton(tr("Start"),this);
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
    mainLayout->addWidget(startButton,9,0,1,2);
    connect(startButton, &QPushButton::clicked, this, &Widget::drawTile);
}

Widget::~Widget()
{
}
void Widget::drawTile()
{
    
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



    osmscout::DatabaseParameter databaseParameter;
    osmscout::DatabaseRef       database=std::make_shared<osmscout::Database>(databaseParameter);
    osmscout::MapServiceRef     mapService=std::make_shared<osmscout::MapService>(database);

    if (!database->Open(map.toStdString())) {
        std::cerr << "Cannot open database" << std::endl;

        close();
    }

    osmscout::StyleConfigRef styleConfig=std::make_shared<osmscout::StyleConfig>(database->GetTypeConfig());

    if (!styleConfig->Load(style.toStdString())) {
        std::cerr << "Cannot open style" << std::endl;
    }

    osmscout::TileProjection      projection;
    osmscout::MapParameter        drawParameter;
    osmscout::AreaSearchParameter searchParameter;

    // Change this, to match your system
    drawParameter.SetFontName("/usr/share/fonts/truetype/msttcorefonts/Verdana.ttf");
    drawParameter.SetFontName("/usr/share/fonts/TTF/DejaVuSans.ttf");
    drawParameter.SetFontSize(2.0);
    // Fadings make problems with tile approach, we disable it
    drawParameter.SetDrawFadings(false);
    // To get accurate label drawing at tile borders, we take into account labels
    // of other than the current tile, too.
    drawParameter.SetDropNotVisiblePointLabels(false);

    searchParameter.SetUseLowZoomOptimization(true);
    searchParameter.SetMaximumAreaLevel(3);

    osmscout::MapPainterQt painter(styleConfig);

    for (osmscout::MagnificationLevel level=osmscout::MagnificationLevel(std::min(startLevel,endLevel));
         level<=osmscout::MagnificationLevel(std::max(startLevel,endLevel));
         level++) {

    }

    database->Close();
}
void Widget::MergeTilesToMapData(const std::list<osmscout::TileRef>& centerTiles,
                                 const osmscout::MapService::TypeDefinition& ringTypeDefinition,
                                 const std::list<osmscout::TileRef>& ringTiles,
                                 osmscout::MapData& data)
{
    std::unordered_map<osmscout::FileOffset,osmscout::NodeRef> nodeMap(10000);
    std::unordered_map<osmscout::FileOffset,osmscout::WayRef>  wayMap(10000);
    std::unordered_map<osmscout::FileOffset,osmscout::AreaRef> areaMap(10000);
    std::unordered_map<osmscout::FileOffset,osmscout::WayRef>  optimizedWayMap(10000);
    std::unordered_map<osmscout::FileOffset,osmscout::AreaRef> optimizedAreaMap(10000);

    osmscout::StopClock uniqueTime;

    for (const auto& tile : centerTiles) {
        tile->GetNodeData().CopyData([&nodeMap](const osmscout::NodeRef& node) {
            nodeMap[node->GetFileOffset()]=node;
        });

        //---

        tile->GetOptimizedWayData().CopyData([&optimizedWayMap](const osmscout::WayRef& way) {
            optimizedWayMap[way->GetFileOffset()]=way;
        });

        tile->GetWayData().CopyData([&wayMap](const osmscout::WayRef& way) {
            wayMap[way->GetFileOffset()]=way;
        });

        //---

        tile->GetOptimizedAreaData().CopyData([&optimizedAreaMap](const osmscout::AreaRef& area) {
            optimizedAreaMap[area->GetFileOffset()]=area;
        });

        tile->GetAreaData().CopyData([&areaMap](const osmscout::AreaRef& area) {
            areaMap[area->GetFileOffset()]=area;
        });
    }

    for (const auto& tile : ringTiles) {
        tile->GetNodeData().CopyData([&ringTypeDefinition,&nodeMap](const osmscout::NodeRef& node) {
            if (ringTypeDefinition.nodeTypes.IsSet(node->GetType())) {
                nodeMap[node->GetFileOffset()]=node;
            }
        });

        //---

        tile->GetOptimizedWayData().CopyData([&ringTypeDefinition,&optimizedWayMap](const osmscout::WayRef& way) {
            if (ringTypeDefinition.optimizedWayTypes.IsSet(way->GetType())) {
                optimizedWayMap[way->GetFileOffset()]=way;
            }
        });

        tile->GetWayData().CopyData([&ringTypeDefinition,&wayMap](const osmscout::WayRef& way) {
            if (ringTypeDefinition.wayTypes.IsSet(way->GetType())) {
                wayMap[way->GetFileOffset()]=way;
            }
        });

        //---

        tile->GetOptimizedAreaData().CopyData([&ringTypeDefinition,&optimizedAreaMap](const osmscout::AreaRef& area) {
            if (ringTypeDefinition.optimizedAreaTypes.IsSet(area->GetType())) {
                optimizedAreaMap[area->GetFileOffset()]=area;
            }
        });

        tile->GetAreaData().CopyData([&ringTypeDefinition,&areaMap](const osmscout::AreaRef& area) {
            if (ringTypeDefinition.areaTypes.IsSet(area->GetType())) {
                areaMap[area->GetFileOffset()]=area;
            }
        });
    }

    uniqueTime.Stop();

    //std::cout << "Make data unique time: " << uniqueTime.ResultString() << std::endl;

    osmscout::StopClock copyTime;

    data.nodes.reserve(nodeMap.size());
    data.ways.reserve(wayMap.size()+optimizedWayMap.size());
    data.areas.reserve(areaMap.size()+optimizedAreaMap.size());

    for (const auto& nodeEntry : nodeMap) {
        data.nodes.push_back(nodeEntry.second);
    }

    for (const auto& wayEntry : wayMap) {
        data.ways.push_back(wayEntry.second);
    }

    for (const auto& wayEntry : optimizedWayMap) {
        data.ways.push_back(wayEntry.second);
    }

    for (const auto& areaEntry : areaMap) {
        data.areas.push_back(areaEntry.second);
    }

    for (const auto& areaEntry : optimizedAreaMap) {
        data.areas.push_back(areaEntry.second);
    }

    copyTime.Stop();

    if (copyTime.GetMilliseconds()>20) {
        osmscout::log.Warn() << "Copying data from tile to MapData took " << copyTime.ResultString();
    }
}

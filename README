# QOsmTilerGui #
### Utility for rendering tiles from map DB ###

Using this utility you can render tiles for any area on the earth, instructions are below, tyler works only in conjunction with my offline
version of the libosmscout plugin? see [there](https://github.com/Deymoss/OpenStreetMapOfflinePlugin/ "Offline plugin"). This is Qt project.

##Build and install##
First of all install all needeed packages
`apt install make gcc g++ cmake mc screen git pkg-config libmarisa0 libmarisa-dev libxml2 libxml2-dev libpthread-stubs0-dev libprotobuf-dev libiconv-hook-dev liblzma-dev libpthread-workqueue-dev libagg-dev libfreetype-dev libpango1.0-dev libharfbuzz-dev libharfbuzz-bin libgl-dev libglew-dev libglm-dev libxrandr-dev libxcursor-dev libxi-dev libglfw3-dev qt5-default libqt5qml5 qtdeclarative5-dev qtscript5-dev qtmultimedia5-dev libqt5svg5-dev qtpositioning5-dev openjdk-8-jdk default-jdk freeglut3 freeglut3-dev libglew1.5 libglew1.5-dev libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev`
##### Create directory with project #####
`mkdir /opt/osm`
##### Enter to the directory #####
`cd /opt/osm`
#####Clone all neccesary libriaries#####
`git clone https://github.com/Framstag/libosmscout/`
`cd libosmscout`
#####Clean up the garbage from the working directory#####
`git clean -d -f -x`
`-d` — delete the whole directories
`-f` — force
`-x` — also delete ignoring files
`cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=DEBUG -DOSMSCOUT_BUILD_BINDING_JAVA=OFF`
#####Explanations#####
-D — create or update an entry in the cmake cache
CMAKE_INSTALL_PREFIX=/usr — sets the values to variable CMAKE_INSTALL_PREFIX default value is /usr/local
CMAKE_BUILD_TYPE=DEBUG — sets build type
OSMSCOUT_BUILD_BINDING_JAVA=OFF - off java
#####Collect#####
`make -j5`
`make install`
#####Create DB directory#####
`mkdir /opt/osm/mapDB`
`cd /opt/osm`
#####Download map in .osm format#####
In my case it's map of Belarus
`wget https://download.geofabrik.de/europe/belarus-latest.osm.bz2`
`bunzip2 belarus-latest.osm.bz2`
#####Start creating database and all necessary files for rendering#####
`Import --destinationDirectory <Import Path*> --typefile <Path to file ost> <Path to map osm>`
Import Path* - here you need to specify the path to the folder in which the database and other files will be created for further use during rendering
Example:
Import --destinationDirectory /home/deymos/imported_data —typefile /home/deymos/libosmscout/stylesheets/map.ost /home/deymos/SIMURAN/belarus-latest.osm
#####Test of rendering map#####
`DrawMapQt <Import files*> <Style files*> <width*> <height*> <longitude*> <lattitude*> <zoom*> <output*>`
Import files* - The directory where the files from the previous step are collected (import)
style file* - the file with styles (drawing roads, vehicles, etc.) is located in libosmscout/stylesheets/
width* - image width in pixels
height* - image height in pixels
longitude* - longitude coordinate
lattitude* - lattitude coordinate
zoom* - proximity value (there is a zoom level and a zoom value, there are 20 zoom levels, 0 is the world map, 20 is the largest zoom, actually a house, and the zoom value, what is indicated in the example below is 2 ^ zoom level)
output* - the name of the image that will be rendered

If the image was created, then you did everything right:)
Now you can open and build QOsmTilerGui

When you run the utility, you will see a window with several lines, about almost all lines i tell before, last line is Name of plugin, enter name according to your plugin for succsessfull work of qml Map
After successful rendering, the tiles will be in the build, in the offline_tiles folder, but loading tiles directly from the folder to the map will lead to lags and long loading of tiles, so another binary file is created in the build folder 
with which in a split second you can load all the tiles in the required areas on the map, read more [here](https://github.com/Deymoss/OpenStreetMapOfflinePlugin)

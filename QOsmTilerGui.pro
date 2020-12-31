QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    queuebuilder.cpp \
    tilerinterface.cpp \
    widget.cpp

HEADERS += \
    MainStruct.h \
    queuebuilder.h \
    tilerinterface.h \
    widget.h

INCLUDEPATH += /usr/local/include

LIBS += -losmscoutd \
            -losmscout_gpxd \
            -losmscout_mapd \
            -losmscout_map_cairod \
            -losmscout_map_svgd \
            -losmscout_client_qtd \
            -losmscout_importd \
            -losmscout_map_aggd \
            -losmscout_map_qtd \
            -losmscout_testd

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

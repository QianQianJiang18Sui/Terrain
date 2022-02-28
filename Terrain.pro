QT       += core gui datavisualization

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(./MathTool/MathTool.pri)
include(./Graphics/Graphics.pri)

SOURCES += \
    GraphicObject.cpp \
    GraphicsModel.cpp \
    Material.cpp \
    Text2D.cpp \
    axis.cpp \
    cube.cpp \
    gridline.cpp \
    line.cpp \
    main.cpp \
    mainwindow.cpp \
    propertywidget.cpp \
    rgradient.cpp \
    terrain.cpp \
    terrainwidget.cpp \
    shader.cpp \
    terrain_widget.cpp \
    camera.cpp \
    triangle_strip.cpp

HEADERS += \
    GraphicObject.h \
    GraphicsModel.h \
    Material.h \
    Text2D.h \
    axis.h \
    cube.h \
    gridline.h \
    line.h \
    mainwindow.h \
    propertywidget.h \
    rgradient.h \
    terrain.h \
    terrainwidget.h \
    shader.h \
    terrain_widget.h \
    camera.h \
    triangle_strip.h

FORMS += \
    mainwindow.ui \
    propertywidget.ui \
    terrainwidget.ui

#include(.\openglwindow\openglwindow.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    rc.qrc

QMAKE_CXXFLAGS += -execution-charset:utf-8
QMAKE_CXXFLAGS += -source-charset:utf-8
QMAKE_CXXFLAGS_WARN_ON += -wd4819

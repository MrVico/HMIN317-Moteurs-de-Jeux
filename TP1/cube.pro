QT       += core gui widgets

TARGET = cube
TEMPLATE = app

SOURCES += main.cpp \
    seasontimer.cpp \
    quadtree.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h \
    seasontimer.h \
    quadtree.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/cube
INSTALLS += target

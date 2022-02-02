############################ TARGET AND FLAGS ############################

#App config
TARGET = mesh_curvature
TEMPLATE = app
CONFIG += c++17
CONFIG += qt
CONFIG += opengl
CONFIG -= app_bundle
QT += core gui opengl widgets

#Debug/release optimization flags
CONFIG(debug, debug|release){
    DEFINES += DEBUG
}
CONFIG(release, debug|release){
    DEFINES -= DEBUG
    #just uncomment next line if you want to ignore asserts and got a more optimized binary
    CONFIG += FINAL_RELEASE
}

#Final release optimization flag
FINAL_RELEASE {
    unix:!macx{
        QMAKE_CXXFLAGS_RELEASE -= -g -O2
        QMAKE_CXXFLAGS += -O3 -DNDEBUG
    }
}

macx {
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.13
    QMAKE_MAC_SDK = macosx10.13
}


############################ LIBRARIES ############################

NUVOLIB_PATH = $$PWD/../../..
EIGEN_PATH = /usr/include/eigen3
LIBGQLVIEWER_PATH = /usr/lib/x86_64-linux-gnu

#nuvolib (it includes eigen, libqglviewer ...)
include($$NUVOLIB_PATH/nuvolib.pri)

#Parallel computation
unix:!mac {
    QMAKE_CXXFLAGS += -fopenmp
    LIBS += -fopenmp
}
macx{
    QMAKE_CXXFLAGS += -Xpreprocessor -fopenmp -lomp -I/usr/local/include
    QMAKE_LFLAGS += -lomp
    LIBS += -L /usr/local/lib /usr/local/lib/libomp.dylib
}


############################ PROJECT FILES ############################

#Project files
SOURCES += \
    mesh_curvature.cpp


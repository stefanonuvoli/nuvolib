#Include path
INCLUDEPATH += $$PWD

qt {
    DEFINES += NVL_QT
}

#Loading modules
include($$PWD/nvl/utilities/utilities.pri)
include($$PWD/nvl/math/math.pri)
include($$PWD/nvl/structures/structures.pri)
include($$PWD/nvl/models/models.pri)
include($$PWD/nvl/viewer/viewer.pri)
include($$PWD/nvl/vcglib/vcglib.pri)
include($$PWD/nvl/libigl/libigl.pri)
include($$PWD/nvl/external/external.pri)

win32{
    QMAKE_CXXFLAGS += -bigobj
}

#Headers
HEADERS += \
    $$PWD/nvl/nuvolib.h

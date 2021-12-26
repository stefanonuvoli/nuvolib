#Include path
INCLUDEPATH += $$PWD

qt {
    DEFINES += NVL_QT
}

message(------------------------------------------------)
message(Loading nuvolib...)
message(------------------------------------------------)

#Loading libraries
include($$PWD/nvl/libraries/libraries.pri)
include($$PWD/nvl/math/math.pri)
include($$PWD/nvl/utilities/utilities.pri)
include($$PWD/nvl/structures/structures.pri)
include($$PWD/nvl/models/models.pri)
include($$PWD/nvl/viewer/viewer.pri)

win32{
    QMAKE_CXXFLAGS += -bigobj
}

#Headers
HEADERS += \
    $$PWD/nvl/nuvolib.h

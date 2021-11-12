DEFINES += NVL_UTILITIES_LOADED

#Utilities

HEADERS += \
    $$PWD/color.h \
    $$PWD/colorize.h \
    $$PWD/comparators.h \
    $$PWD/file_utils.h \
    $$PWD/iterator_wrapper.h \
    $$PWD/random.h \
    $$PWD/string_utils.h \
    $$PWD/timer.h \
    $$PWD/vector_utils.h

SOURCES += \
    $$PWD/color.cpp \
    $$PWD/colorize.cpp \
    $$PWD/comparators.cpp \
    $$PWD/file_utils.cpp \
    $$PWD/iterator_wrapper.cpp \
    $$PWD/random.cpp \
    $$PWD/string_utils.cpp \
    $$PWD/timer.cpp \
    $$PWD/vector_utils.cpp

message(Module \"utilities\" loaded.)

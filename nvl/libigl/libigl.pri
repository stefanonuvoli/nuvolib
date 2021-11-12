isEmpty(LIBIGL_PATH) {
    LIBIGL_ENV_VARIABLE = $$(LIBIGL_HOME)
}

isEmpty(LIBIGL_PATH):!isEmpty(LIBIGL_ENV_VARIABLE):exists($$LIBIGL_ENV_VARIABLE) {
    LIBIGL_PATH = $$LIBIGL_ENV_VARIABLE
}

!isEmpty(LIBIGL_PATH):exists($$LIBIGL_PATH) {
    INCLUDEPATH += $$LIBIGL_PATH/include/

    DEFINES += NVL_LIBIGL_LOADED

    HEADERS += \
        $$PWD/igl_geodesics.h

    SOURCES += \
        $$PWD/igl_geodesics.cpp

    message(Module \"libigl\" loaded.)
}

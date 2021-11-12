isEmpty(VCGLIB_PATH) {
    VCGLIB_ENV_VARIABLE = $$(VCGLIB_HOME)
}

isEmpty(VCGLIB_PATH):!isEmpty(VCGLIB_ENV_VARIABLE):exists($$VCGLIB_ENV_VARIABLE) {
    VCGLIB_PATH = $$VCGLIB_ENV_VARIABLE
}

!isEmpty(VCGLIB_PATH):exists($$VCGLIB_PATH) {
    INCLUDEPATH += $$VCGLIB_PATH

    DEFINES += NVL_VCGLIB_LOADED

    HEADERS += \
        $$PWD/vcg_collapse_borders.h \
        $$PWD/vcg_convert.h \
        $$PWD/vcg_curve_on_manifold.h \
        $$PWD/vcg_grid.h \
        $$PWD/vcg_mesh_refine.h \
        $$PWD/vcg_polygon_mesh.h \
        $$PWD/vcg_triangle_mesh.h

    SOURCES += \
        $$PWD/vcg_collapse_borders.cpp \
        $$PWD/vcg_convert.cpp \
        $$PWD/vcg_curve_on_manifold.cpp \
        $$PWD/vcg_grid.cpp \
        $$PWD/vcg_mesh_refine.cpp

    message(Module \"vcglib\" loaded.)
}

SOURCES += \
    $$PWD/vcg_remeshing.cpp

HEADERS += \
    $$PWD/vcg_remeshing.h

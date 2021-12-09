!contains(DEFINES, NVL_MATH_LOADED){
    error(Module "viewer" requires the module "math".)
}
!contains(DEFINES, NVL_UTILITIES_LOADED){
    error(Module "viewer" requires the module "utilities".)
}
contains(DEFINES, NVL_QT) {
    DEFINES += NVL_VIEWER_LOADED

    QT += core gui opengl xml widgets

    win32 {
        LIBS += -lglu32 -lopengl32 -lglew
    }
    unix {
        LIBS += -lGLU -lGLEW -lGL
    }

    HEADERS += \
        $$PWD/viewerwindow.h \
        $$PWD/widgets/animation_widget.h \ \
        $$PWD/widgets/qcanvas.h \
        $$PWD/widgets/colorpicker_button.h \
        $$PWD/widgets/drawable_list_widget.h \
        $$PWD/widgets/drawable_widget.h \
        $$PWD/widgets/model_animation_widget.h \
        $$PWD/widgets/model_drawer_widget.h \
        $$PWD/widgets/model_loader_widget.h \
        $$PWD/widgets/skeleton_joint_list_widget.h \
        $$PWD/widgets/face_mesh_drawer_widget.h \
        $$PWD/widgets/polyline_mesh_drawer_widget.h \
        $$PWD/widgets/skeleton_drawer_widget.h \
        $$PWD/widgets/vertex_mesh_drawer_widget.h

    SOURCES += \
        $$PWD/viewerwindow.cpp \
        $$PWD/widgets/animation_widget.cpp \
        $$PWD/widgets/qcanvas.cpp \
        $$PWD/widgets/colorpicker_button.cpp \
        $$PWD/widgets/drawable_list_widget.cpp \
        $$PWD/widgets/drawable_widget.cpp \
        $$PWD/widgets/model_animation_widget.cpp \
        $$PWD/widgets/model_drawer_widget.cpp \
        $$PWD/widgets/model_loader_widget.cpp \
        $$PWD/widgets/skeleton_joint_list_widget.cpp \
        $$PWD/widgets/face_mesh_drawer_widget.cpp \
        $$PWD/widgets/polyline_mesh_drawer_widget.cpp \
        $$PWD/widgets/skeleton_drawer_widget.cpp \
        $$PWD/widgets/vertex_mesh_drawer_widget.cpp

    RESOURCES += \
        $$PWD/gl/shaders.qrc

    FORMS +=  \
        $$PWD/viewerwindow.ui \
        $$PWD/widgets/animation_widget.ui \
        $$PWD/widgets/drawable_list_widget.ui \
        $$PWD/widgets/drawable_widget.ui \
        $$PWD/widgets/model_animation_widget.ui \
        $$PWD/widgets/model_drawer_widget.ui \
        $$PWD/widgets/model_loader_widget.ui \
        $$PWD/widgets/skeleton_joint_list_widget.ui \
        $$PWD/widgets/face_mesh_drawer_widget.ui \
        $$PWD/widgets/polyline_mesh_drawer_widget.ui \
        $$PWD/widgets/skeleton_drawer_widget.ui \
        $$PWD/widgets/vertex_mesh_drawer_widget.ui

    include($$PWD/qglviewer.pri)

    contains(DEFINES, NVL_QGLVIEWER_LOADED) {

        HEADERS += \
            $$PWD/gl/qgl_base_shader.h \
            $$PWD/gl/qgl_contour_shader.h \
            $$PWD/gl/qgl_ramp_shader.h \
            $$PWD/widgets/qglviewer_canvas.h \
            $$PWD/widgets/qglviewer_object.h

        SOURCES += \
            $$PWD/gl/qgl_base_shader.cpp \
            $$PWD/gl/qgl_contour_shader.cpp \
            $$PWD/gl/qgl_ramp_shader.cpp \
            $$PWD/widgets/qglviewer_canvas.cpp \
            $$PWD/widgets/qglviewer_object.cpp

    }

    message(Module \"viewer\" loaded.)
}

HEADERS +=  \
    $$PWD/drawables/face_mesh_drawer.h \
    $$PWD/drawables/face_mesh_drawer_base.h \
    $$PWD/drawables/mesh_drawer.h \
    $$PWD/drawables/mesh_drawer_base.h \
    $$PWD/drawables/model_drawer.h \
    $$PWD/drawables/model_drawer_base.h \
    $$PWD/drawables/polyline_mesh_drawer.h \
    $$PWD/drawables/polyline_mesh_drawer_base.h \
    $$PWD/drawables/skeleton_drawer.h \
    $$PWD/drawables/skeleton_drawer_base.h \
    $$PWD/drawables/vertex_mesh_drawer.h \
    $$PWD/drawables/vertex_mesh_drawer_base.h \
    $$PWD/gl/gl_draw.h \
    $$PWD/gl/gl_frameable.h \
    $$PWD/gl/gl_primitives.h \
    $$PWD/gl/gl_shader.h \
    $$PWD/gl/gl_textures.h \
    $$PWD/gl/opengl_headers.h \
    $$PWD/interfaces/animable.h \
    $$PWD/interfaces/drawable.h \
    $$PWD/interfaces/frameable.h \
    $$PWD/interfaces/pickable.h \
    $$PWD/widgets/canvas.h

SOURCES += \
    $$PWD/drawables/face_mesh_drawer.cpp \
    $$PWD/drawables/face_mesh_drawer_base.cpp \
    $$PWD/drawables/mesh_drawer.cpp \
    $$PWD/drawables/mesh_drawer_base.cpp \
    $$PWD/drawables/model_drawer.cpp \
    $$PWD/drawables/model_drawer_base.cpp \
    $$PWD/drawables/polyline_mesh_drawer.cpp \
    $$PWD/drawables/polyline_mesh_drawer_base.cpp \
    $$PWD/drawables/skeleton_drawer.cpp \
    $$PWD/drawables/skeleton_drawer_base.cpp \
    $$PWD/drawables/vertex_mesh_drawer.cpp \
    $$PWD/drawables/vertex_mesh_drawer_base.cpp \
    $$PWD/gl/gl_draw.cpp \
    $$PWD/gl/gl_frameable.cpp \
    $$PWD/gl/gl_primitives.cpp \
    $$PWD/gl/gl_shader.cpp \
    $$PWD/gl/gl_textures.cpp \
    $$PWD/interfaces/animable.cpp \
    $$PWD/interfaces/drawable.cpp \
    $$PWD/interfaces/frameable.cpp \
    $$PWD/interfaces/pickable.cpp \
    $$PWD/widgets/canvas.cpp

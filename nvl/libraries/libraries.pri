#Eigen

isEmpty(EIGEN_PATH) {
    EIGEN_ENV_VARIABLE = $$(EIGEN_HOME)

    !isEmpty(EIGEN_ENV_VARIABLE):exists($$EIGEN_ENV_VARIABLE) {
        EIGEN_PATH = $$EIGEN_ENV_VARIABLE
    }
    else {
        unix:!macx {
            exists(/usr/include/eigen3/) {
                EIGEN_PATH = /usr/include/eigen3/ #linux apt default
            }
        }
        macx {
            exists(/usr/local/include/eigen3) {
                EIGEN_PATH = /usr/local/include/eigen3 #mac brew default
            }
        }
    }
}

!isEmpty(EIGEN_PATH):exists($$EIGEN_PATH) {
    DEFINES += NVL_EIGEN_LOADED
    INCLUDEPATH += -I $$EIGEN_PATH

    message(library \"eigen\" loaded.)
}


#openGL

isEmpty(OPENGL_PATH) {
    OPENGL_ENV_VARIABLE = $$(OPENGL_HOME)

    !isEmpty(OPENGL_ENV_VARIABLE):exists($$OPENGL_ENV_VARIABLE) {
        OPENGL_PATH = $$OPENGL_ENV_VARIABLE
    }
    else {
        unix:!macx {
            exists(/usr/include/GL/) {
                OPENGL_PATH = /usr/include/GL/ #linux apt default
            }
        }
        macx {
            exists(/usr/local/include/OpenGL) {
                OPENGL_PATH = /usr/local/include/OpenGL #mac brew default
            }
        }
    }
}

!isEmpty(OPENGL_PATH):exists($$OPENGL_PATH) {
    DEFINES += NVL_OPENGL_LOADED

    win32 {
        LIBS += -lglu32 -lopengl32 -lglew
    }
    unix {
        LIBS += -lGLU -lGLEW -lGL
    }

    message(library \"opengl\" loaded.)
}



#QGLViewer

isEmpty(QGLVIEWER_PATH) {
    QGLVIEWER_ENV_VARIABLE = $$(QGLVIEWER_HOME)

    !isEmpty(QGLVIEWER_ENV_VARIABLE):exists($$QGLVIEWER_ENV_VARIABLE) {
        QGLVIEWER_PATH = $$QGLVIEWER_ENV_VARIABLE
    }
    else {
        unix:!macx {
            exists(/usr/lib/x86_64-linux-gnu/libQGLViewer-qt5.so)|exists(/usr/lib/x86_64-linux-gnu/libQGLViewer.so) {
                QGLVIEWER_PATH = /usr/lib/x86_64-linux-gnu #linux apt default
            }
        }
        macx {
            exists(/usr/local/lib/QGLViewer.framework)|exists(/usr/local/lib/QGLViewer.dylib) {
                QGLVIEWER_PATH = /usr/local/lib #mac brew default
            }
        }
    }
}

!isEmpty(QGLVIEWER_PATH):exists($$QGLVIEWER_PATH) {
    unix:!macx {
        exists($$QGLVIEWER_PATH/libQGLViewer.so) {
            LIBS += -lQGLViewer
            DEFINES += NVL_QGLVIEWER_LOADED
            message(library \"qglviewer\" loaded.)
        }
        exists($$QGLVIEWER_PATH/libQGLViewer-qt5.so) {
            LIBS += -lQGLViewer-qt5
            DEFINES += NVL_QGLVIEWER_LOADED
            message(library \"qglviewer\" loaded.)
        }

        LIBS += -lstdc++fs
    }

    macx {
        INCLUDEPATH += $$QGLVIEWER_PATH/QGLViewer.framework/Headers

        TARGETPATH = $${TARGET}.app/Contents/MacOS/$${TARGET}
        !isEmpty(DESTDIR) {
            TARGETPATH = $${DESTDIR}/$${TARGET}.app/Contents/MacOS/$${TARGET}
        }

        exists($$QGLVIEWER_PATH/QGLViewer.framework) {
            !plugin:QMAKE_POST_LINK=install_name_tool -change QGLViewer.framework/Versions/2/QGLViewer $$QGLVIEWER_PATH/QGLViewer.framework/Versions/2/QGLViewer $$TARGETPATH #VERSION_MAJOR
            LIBS += -F$$QGLVIEWER_PATH -framework QGLViewer
        }
        else {
            !plugin:QMAKE_POST_LINK=install_name_tool -change libQGLViewer.2.dylib $$QGLVIEWER_PATH/libQGLViewer.2.dylib $$TARGETPATH #VERSION_MAJOR
            LIBS *= -L$$QGLVIEWER_PATH -lQGLViewer
        }

        DEFINES += NVL_QGLVIEWER_LOADED
        message(library \"qglviewer\" loaded.)
    }

    win32 {
        INCLUDEPATH += -I $$quote($$QGLVIEWER_PATH)

        CONFIG(debug, debug|release){
            LIBS += -L$$quote($$QGLVIEWER_PATH/QGLViewer) -lQGLViewerd2
        }
        CONFIG(release, debug|release){
            LIBS += -L$$quote($$QGLVIEWER_PATH/QGLViewer) -lQGLViewer2
        }

        DEFINES += NVL_QGLVIEWER_LOADED
        message(library \"qglviewer\" loaded.)
    }
}



#libigl

isEmpty(LIBIGL_PATH) {
    LIBIGL_ENV_VARIABLE = $$(LIBIGL_HOME)
}

isEmpty(LIBIGL_PATH):!isEmpty(LIBIGL_ENV_VARIABLE):exists($$LIBIGL_ENV_VARIABLE) {
    LIBIGL_PATH = $$LIBIGL_ENV_VARIABLE
}

!isEmpty(LIBIGL_PATH):exists($$LIBIGL_PATH) {
    INCLUDEPATH += $$LIBIGL_PATH/include/

    DEFINES += NVL_LIBIGL_LOADED

    message(library \"libigl\" loaded.)
}


#vcglib

isEmpty(VCGLIB_PATH) {
    VCGLIB_ENV_VARIABLE = $$(VCGLIB_HOME)
}

isEmpty(VCGLIB_PATH):!isEmpty(VCGLIB_ENV_VARIABLE):exists($$VCGLIB_ENV_VARIABLE) {
    VCGLIB_PATH = $$VCGLIB_ENV_VARIABLE
}

!isEmpty(VCGLIB_PATH):exists($$VCGLIB_PATH) {
    INCLUDEPATH += $$VCGLIB_PATH

    DEFINES += NVL_VCGLIB_LOADED

    message(library \"vcglib\" loaded.)
}


#fbxsdk

isEmpty(FBXSDK_PATH) {
    FBXSDK_ENV_VARIABLE = $$(FBXSDK_HOME)

    !isEmpty(FBXSDK_ENV_VARIABLE):exists($$FBXSDK_ENV_VARIABLE) {
        FBXSDK_PATH = $$FBXSDK_ENV_VARIABLE
    }
}


!isEmpty(FBXSDK_PATH):exists($$FBXSDK_PATH) {
    CONFIG(debug, debug|release){
        LIBS += -L$$quote($$FBXSDK_PATH/lib/gcc/x64/debug) -lfbxsdk
    }
    CONFIG(release, debug|release){
        LIBS += -L$$quote($$FBXSDK_PATH/lib/gcc/x64/release) -lfbxsdk
    }

    LIBS += -ldl
    LIBS += -lxml2

    INCLUDEPATH += $$FBXSDK_PATH/include
    INCLUDEPATH += /usr/include/libxml2

    DEFINES += NVL_FBXSDK_LOADED

    message(library \"fbxsdk\" loaded.)
}



#stb

isEmpty(STB_PATH) {
    STB_ENV_VARIABLE = $$(STB_HOME)

    !isEmpty(STB_ENV_VARIABLE):exists($$STB_ENV_VARIABLE) {
        STB_PATH = $$STB_ENV_VARIABLE
    }
    else {
        STB_PATH = $$PWD/stb
    }
}

!isEmpty(STB_PATH):exists($$STB_PATH)  {
    DEFINES += NVL_STB_LOADED
    INCLUDEPATH += -I $$quote($$STB_PATH)

    message(library \"stb\" loaded.)
}

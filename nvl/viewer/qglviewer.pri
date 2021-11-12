#QGLViewer include

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
        }
        exists($$QGLVIEWER_PATH/libQGLViewer-qt5.so) {
            LIBS += -lQGLViewer-qt5
            DEFINES += NVL_QGLVIEWER_LOADED
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
    }
}

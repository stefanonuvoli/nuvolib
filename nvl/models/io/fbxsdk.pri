#FBXSDK include

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
}

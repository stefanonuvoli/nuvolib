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
    
    message(Library \"stb\" loaded.)
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

    message(Library \"libigl\" loaded.)
}

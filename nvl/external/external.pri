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
}

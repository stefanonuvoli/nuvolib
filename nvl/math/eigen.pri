#Eigen include

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
}

!contains(DEFINES, NVL_UTILITIES_LOADED){
    error(Module "structures" requires the module "utilities".)
}

DEFINES += NVL_STRUCTURES_LOADED

# ----- Containers -----

HEADERS += \
    $$PWD/disjoint_set.h \
    $$PWD/internal/vector_with_delete_iterator.h \
    $$PWD/vector_with_delete.h

SOURCES += \
    $$PWD/disjoint_set.cpp \
    $$PWD/internal/vector_with_delete_iterator.cpp \
    $$PWD/vector_with_delete.cpp


# ----- Graphs -----

HEADERS += \
    $$PWD/dijkstra.h \
    $$PWD/graph.h \
    $$PWD/internal/graph_adjacentiterator.h \
    $$PWD/internal/graph_edgeiterator.h \
    $$PWD/internal/graph_genericnodeiterator.h \
    $$PWD/internal/graph_node.h \
    $$PWD/internal/graph_nodeiterator.h

SOURCES += \
    $$PWD/dijkstra.cpp \
    $$PWD/graph.cpp \
    $$PWD/internal/graph_adjacentiterator.cpp \
    $$PWD/internal/graph_edgeiterator.cpp \
    $$PWD/internal/graph_genericnodeiterator.cpp \
    $$PWD/internal/graph_node.cpp \
    $$PWD/internal/graph_nodeiterator.cpp

message(Module \"structures\" loaded.)

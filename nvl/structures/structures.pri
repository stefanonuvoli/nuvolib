!contains(DEFINES, NVL_UTILITIES_LOADED){
    error(Module "structures" requires the module "utilities".)
}

DEFINES += NVL_STRUCTURES_LOADED

#Containers

HEADERS += \
    $$PWD/containers/disjoint_set.h \
    $$PWD/containers/internal/vector_with_delete_iterator.h \
    $$PWD/containers/vector_with_delete.h

SOURCES += \
    $$PWD/containers/disjoint_set.cpp \
    $$PWD/containers/internal/vector_with_delete_iterator.cpp \
    $$PWD/containers/vector_with_delete.cpp



#Graphs

HEADERS += \
    $$PWD/graphs/dijkstra.h \
    $$PWD/graphs/graph.h \
    $$PWD/graphs/internal/graph_adjacentiterator.h \
    $$PWD/graphs/internal/graph_edgeiterator.h \
    $$PWD/graphs/internal/graph_genericnodeiterator.h \
    $$PWD/graphs/internal/graph_node.h \
    $$PWD/graphs/internal/graph_nodeiterator.h

SOURCES += \
    $$PWD/graphs/dijkstra.cpp \
    $$PWD/graphs/graph.cpp \
    $$PWD/graphs/internal/graph_adjacentiterator.cpp \
    $$PWD/graphs/internal/graph_edgeiterator.cpp \
    $$PWD/graphs/internal/graph_genericnodeiterator.cpp \
    $$PWD/graphs/internal/graph_node.cpp \
    $$PWD/graphs/internal/graph_nodeiterator.cpp



#Trees

HEADERS += \
    $$PWD/trees/internal/tree_common.h \
    $$PWD/trees/internal/iterators/tree_genericiterator.h \
    $$PWD/trees/internal/iterators/tree_insertiterator.h \
    $$PWD/trees/internal/iterators/tree_iterator.h \
    $$PWD/trees/internal/iterators/tree_reverseiterator.h \
    $$PWD/trees/internal/iterators/tree_rangebased_iterators.h \
    $$PWD/trees/internal/bst_helpers.h \
    $$PWD/trees/internal/bstinner_helpers.h \
    $$PWD/trees/internal/bstleaf_helpers.h \
    $$PWD/trees/internal/avl_helpers.h \
    $$PWD/trees/bst.h \
    $$PWD/trees/bstinner.h \
    $$PWD/trees/bstleaf.h \
    $$PWD/trees/internal/nodes/bst_node.h \
    $$PWD/trees/avlinner.h \
    $$PWD/trees/avlleaf.h \
    $$PWD/trees/internal/nodes/avl_node.h \
    $$PWD/trees/rangetree.h \
    $$PWD/trees/internal/nodes/rangetree_node.h \
    $$PWD/trees/internal/rangetree_types.h \
    $$PWD/trees/aabbtree.h \
    $$PWD/trees/internal/nodes/aabb_node.h

SOURCES += \
    $$PWD/trees/internal/iterators/tree_insertiterator.cpp \
    $$PWD/trees/internal/iterators/tree_iterator.cpp \
    $$PWD/trees/internal/iterators/tree_reverseiterator.cpp \
    $$PWD/trees/internal/iterators/tree_rangebased_iterators.cpp \
    $$PWD/trees/internal/bst_helpers.cpp \
    $$PWD/trees/internal/bstinner_helpers.cpp \
    $$PWD/trees/internal/bstleaf_helpers.cpp \
    $$PWD/trees/internal/avl_helpers.cpp \
    $$PWD/trees/bstinner.cpp \
    $$PWD/trees/bstleaf.cpp \
    $$PWD/trees/internal/nodes/bst_node.cpp \
    $$PWD/trees/avlinner.cpp \
    $$PWD/trees/avlleaf.cpp \
    $$PWD/trees/internal/nodes/avl_node.cpp \
    $$PWD/trees/rangetree.cpp \
    $$PWD/trees/internal/nodes/rangetree_node.cpp \
    $$PWD/trees/internal/rangetree_types.cpp \
    $$PWD/trees/aabbtree.cpp \
    $$PWD/trees/internal/nodes/aabb_node.cpp


message(Module \"structures\" loaded.)

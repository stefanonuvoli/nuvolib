!contains(DEFINES, NVL_MATH_LOADED){
    error(Module "models" requires the module "math".)
}
!contains(DEFINES, NVL_UTILITIES_LOADED){
    error(Module "models" requires the module "utilities".)
}
!contains(DEFINES, NVL_STRUCTURES_LOADED){
    error(Module "models" requires the module "structures".)
}

DEFINES += NVL_MODELS_LOADED

#Models

HEADERS += \
    $$PWD/abstract_mesh.h \
    $$PWD/animation.h \
    $$PWD/animation_algorithms.h \
    $$PWD/animation_frame.h \
    $$PWD/animation_io.h \
    $$PWD/animation_io_data.h \
    $$PWD/animation_io_ska.h \
    $$PWD/animation_skinning.h \
    $$PWD/animation_transformations.h \
    $$PWD/edge_mesh.h \
    $$PWD/face_mesh.h \
    $$PWD/mesh_array_face.h \
    $$PWD/mesh_differentiation.h \
    $$PWD/mesh_edge.h \
    $$PWD/mesh_face.h \
    $$PWD/mesh_material.h \
    $$PWD/mesh_polyline.h \
    $$PWD/mesh_vector_face.h \
    $$PWD/mesh_vertex.h \
    $$PWD/mesh_edge_handler.h \
    $$PWD/mesh_face_handler.h \
    $$PWD/mesh_polyline_handler.h \
    $$PWD/mesh_vertex_handler.h \
    $$PWD/mesh_adjacencies.h \
    $$PWD/mesh_borders.h \
    $$PWD/mesh_cleaning.h \
    $$PWD/mesh_consistency.h \
    $$PWD/mesh_edge_handler.h \
    $$PWD/mesh_eigen_convert.h \
    $$PWD/mesh_face_handler.h \
    $$PWD/mesh_face_navigator.h \
    $$PWD/mesh_geometric_information.h \
    $$PWD/mesh_graph.h \
    $$PWD/mesh_implicit_function.h \
    $$PWD/mesh_io.h \
    $$PWD/mesh_io_data.h \
    $$PWD/mesh_io_obj.h \
    $$PWD/mesh_morphological_operations.h \
    $$PWD/mesh_normals.h \
    $$PWD/mesh_polyline_handler.h \
    $$PWD/mesh_smoothing.h \
    $$PWD/mesh_split.h \
    $$PWD/mesh_subdivision.h \
    $$PWD/mesh_transfer.h \
    $$PWD/mesh_transformations.h \
    $$PWD/mesh_triangulation.h \
    $$PWD/mesh_vertex_handler.h \
    $$PWD/meshes.h \
    $$PWD/model.h \
    $$PWD/model_deformation.h \
    $$PWD/model_io.h \
    $$PWD/model_io_data.h \
    $$PWD/model_io_rig.h \
    $$PWD/model_normalization.h \
    $$PWD/model_transfer.h \
    $$PWD/model_transformations.h \
    $$PWD/polyline_mesh.h \
    $$PWD/skeleton.h \
    $$PWD/skeleton_adjacencies.h \
    $$PWD/skeleton_io.h \
    $$PWD/skeleton_io_data.h \
    $$PWD/skeleton_io_skt.h \
    $$PWD/skeleton_joint.h \
    $$PWD/skeleton_transfer.h \
    $$PWD/skeleton_transformations.h \
    $$PWD/skinning_weights.h \
    $$PWD/skinning_weights_io.h \
    $$PWD/skinning_weights_io_data.h \
    $$PWD/skinning_weights_io_skw.h \
    $$PWD/vertex_mesh.h

SOURCES +=  \
    $$PWD/abstract_mesh.cpp \
    $$PWD/animation.cpp \
    $$PWD/animation_algorithms.cpp \
    $$PWD/animation_frame.cpp \
    $$PWD/animation_io.cpp \
    $$PWD/animation_io_data.cpp \
    $$PWD/animation_io_ska.cpp \
    $$PWD/animation_skinning.cpp \
    $$PWD/animation_transformations.cpp \
    $$PWD/edge_mesh.cpp \
    $$PWD/face_mesh.cpp \
    $$PWD/mesh_array_face.cpp \
    $$PWD/mesh_differentiation.cpp \
    $$PWD/mesh_edge.cpp \
    $$PWD/mesh_face.cpp \
    $$PWD/mesh_material.cpp \
    $$PWD/mesh_polyline.cpp \
    $$PWD/mesh_vector_face.cpp \
    $$PWD/mesh_vertex.cpp \
    $$PWD/mesh_edge_handler.cpp \
    $$PWD/mesh_face_handler.cpp \
    $$PWD/mesh_polyline_handler.cpp \
    $$PWD/mesh_vertex_handler.cpp \
    $$PWD/mesh_adjacencies.cpp \
    $$PWD/mesh_borders.cpp \
    $$PWD/mesh_cleaning.cpp \
    $$PWD/mesh_consistency.cpp \
    $$PWD/mesh_edge_handler.cpp \
    $$PWD/mesh_eigen_convert.cpp \
    $$PWD/mesh_face_handler.cpp \
    $$PWD/mesh_face_navigator.cpp \
    $$PWD/mesh_geometric_information.cpp \
    $$PWD/mesh_graph.cpp \
    $$PWD/mesh_implicit_function.cpp \
    $$PWD/mesh_io.cpp \
    $$PWD/mesh_io_data.cpp \
    $$PWD/mesh_io_obj.cpp \
    $$PWD/mesh_morphological_operations.cpp \
    $$PWD/mesh_normals.cpp \
    $$PWD/mesh_polyline_handler.cpp \
    $$PWD/mesh_smoothing.cpp \
    $$PWD/mesh_split.cpp \
    $$PWD/mesh_subdivision.cpp \
    $$PWD/mesh_transfer.cpp \
    $$PWD/mesh_transformations.cpp \
    $$PWD/mesh_triangulation.cpp \
    $$PWD/mesh_vertex_handler.cpp \
    $$PWD/model.cpp \
    $$PWD/model_deformation.cpp \
    $$PWD/model_io.cpp \
    $$PWD/model_io_data.cpp \
    $$PWD/model_io_rig.cpp \
    $$PWD/model_normalization.cpp \
    $$PWD/model_transfer.cpp \
    $$PWD/model_transformations.cpp \
    $$PWD/polyline_mesh.cpp \
    $$PWD/skeleton.cpp \
    $$PWD/skeleton_adjacencies.cpp \
    $$PWD/skeleton_io.cpp \
    $$PWD/skeleton_io_data.cpp \
    $$PWD/skeleton_io_skt.cpp \
    $$PWD/skeleton_joint.cpp \
    $$PWD/skeleton_transfer.cpp \
    $$PWD/skeleton_transformations.cpp \
    $$PWD/skinning_weights.cpp \
    $$PWD/skinning_weights_io.cpp \
    $$PWD/skinning_weights_io_data.cpp \
    $$PWD/skinning_weights_io_skw.cpp \
    $$PWD/vertex_mesh.cpp

message(Module \"models\" loaded.)

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


message(Module \"models\" loaded.)

HEADERS += \
    $$PWD/algorithms/animation_algorithms.h \
    $$PWD/algorithms/animation_skinning.h \
    $$PWD/algorithms/animation_transformations.h \
    $$PWD/algorithms/mesh_adjacencies.h \
    $$PWD/algorithms/mesh_borders.h \
    $$PWD/algorithms/mesh_cleaning.h \
    $$PWD/algorithms/mesh_consistency.h \
    $$PWD/algorithms/mesh_differentiation.h \
    $$PWD/algorithms/mesh_eigen_convert.h \
    $$PWD/algorithms/mesh_geometric_information.h \
    $$PWD/algorithms/mesh_graph.h \
    $$PWD/algorithms/mesh_implicit_function.h \
    $$PWD/algorithms/mesh_morphological_operations.h \
    $$PWD/algorithms/mesh_normals.h \
    $$PWD/algorithms/mesh_smoothing.h \
    $$PWD/algorithms/mesh_split.h \
    $$PWD/algorithms/mesh_subdivision.h \
    $$PWD/algorithms/mesh_transfer.h \
    $$PWD/algorithms/mesh_transformations.h \
    $$PWD/algorithms/mesh_triangulation.h \
    $$PWD/algorithms/model_deformation.h \
    $$PWD/algorithms/model_normalization.h \
    $$PWD/algorithms/model_transfer.h \
    $$PWD/algorithms/model_transformations.h \
    $$PWD/algorithms/skeleton_adjacencies.h \
    $$PWD/algorithms/skeleton_transfer.h \
    $$PWD/algorithms/skeleton_transformations.h \
    $$PWD/animation_3d.h \
    $$PWD/structures/handlers/mesh_face_handler.h \
    $$PWD/structures/handlers/mesh_face_material_handler.h \
    $$PWD/structures/handlers/mesh_face_normal_handler.h \
    $$PWD/structures/handlers/mesh_face_wedge_normal_handler.h \
    $$PWD/structures/handlers/mesh_face_wedge_uv_handler.h \
    $$PWD/structures/handlers/mesh_material_handler.h \
    $$PWD/structures/handlers/mesh_polyline_color_handler.h \
    $$PWD/structures/handlers/mesh_polyline_handler.h \
    $$PWD/structures/handlers/mesh_vertex_color_handler.h \
    $$PWD/structures/handlers/mesh_vertex_handler.h \
    $$PWD/structures/handlers/mesh_vertex_normal_handler.h \
    $$PWD/structures/handlers/mesh_vertex_uv_handler.h \
    $$PWD/structures/handlers/mesh_wedge_normal_handler.h \
    $$PWD/structures/handlers/mesh_wedge_uv_handler.h \
    $$PWD/io/animation_io.h \
    $$PWD/io/animation_io_data.h \
    $$PWD/io/animation_io_ska.h \
    $$PWD/io/mesh_io.h \
    $$PWD/io/mesh_io_data.h \
    $$PWD/io/mesh_io_obj.h \
    $$PWD/io/model_io.h \
    $$PWD/io/model_io_data.h \
    $$PWD/io/model_io_rig.h \
    $$PWD/io/skeleton_io.h \
    $$PWD/io/skeleton_io_data.h \
    $$PWD/io/skeleton_io_skt.h \
    $$PWD/io/skinning_weights_io.h \
    $$PWD/io/skinning_weights_io_data.h \
    $$PWD/io/skinning_weights_io_skw.h \
    $$PWD/mesh_3d.h \
    $$PWD/model_3d.h \
    $$PWD/skeleton_3d.h \
    $$PWD/skinning_weights_3d.h \
    $$PWD/structures/abstract_mesh.h \
    $$PWD/structures/animation.h \
    $$PWD/structures/animation_frame.h \
    $$PWD/structures/face_mesh.h \
    $$PWD/structures/mesh_array_face.h \
    $$PWD/structures/mesh_face.h \
    $$PWD/structures/mesh_face_navigator.h \
    $$PWD/structures/mesh_material.h \
    $$PWD/structures/mesh_polyline.h \
    $$PWD/structures/mesh_vector_face.h \
    $$PWD/structures/mesh_vertex.h \
    $$PWD/structures/model.h \
    $$PWD/structures/polyline_mesh.h \
    $$PWD/structures/skeleton.h \
    $$PWD/structures/skeleton_joint.h \
    $$PWD/structures/skinning_weights.h \
    $$PWD/structures/vertex_mesh.h

SOURCES += \
    $$PWD/algorithms/animation_algorithms.cpp \
    $$PWD/algorithms/animation_skinning.cpp \
    $$PWD/algorithms/animation_transformations.cpp \
    $$PWD/algorithms/mesh_adjacencies.cpp \
    $$PWD/algorithms/mesh_borders.cpp \
    $$PWD/algorithms/mesh_cleaning.cpp \
    $$PWD/algorithms/mesh_consistency.cpp \
    $$PWD/algorithms/mesh_differentiation.cpp \
    $$PWD/algorithms/mesh_eigen_convert.cpp \
    $$PWD/algorithms/mesh_geometric_information.cpp \
    $$PWD/algorithms/mesh_graph.cpp \
    $$PWD/algorithms/mesh_implicit_function.cpp \
    $$PWD/algorithms/mesh_morphological_operations.cpp \
    $$PWD/algorithms/mesh_normals.cpp \
    $$PWD/algorithms/mesh_smoothing.cpp \
    $$PWD/algorithms/mesh_split.cpp \
    $$PWD/algorithms/mesh_subdivision.cpp \
    $$PWD/algorithms/mesh_transfer.cpp \
    $$PWD/algorithms/mesh_transformations.cpp \
    $$PWD/algorithms/mesh_triangulation.cpp \
    $$PWD/algorithms/model_deformation.cpp \
    $$PWD/algorithms/model_normalization.cpp \
    $$PWD/algorithms/model_transfer.cpp \
    $$PWD/algorithms/model_transformations.cpp \
    $$PWD/algorithms/skeleton_adjacencies.cpp \
    $$PWD/algorithms/skeleton_transfer.cpp \
    $$PWD/algorithms/skeleton_transformations.cpp \
    $$PWD/structures/handlers/mesh_face_handler.cpp \
    $$PWD/structures/handlers/mesh_face_material_handler.cpp \
    $$PWD/structures/handlers/mesh_face_normal_handler.cpp \
    $$PWD/structures/handlers/mesh_face_wedge_normal_handler.cpp \
    $$PWD/structures/handlers/mesh_face_wedge_uv_handler.cpp \
    $$PWD/structures/handlers/mesh_material_handler.cpp \
    $$PWD/structures/handlers/mesh_polyline_color_handler.cpp \
    $$PWD/structures/handlers/mesh_polyline_handler.cpp \
    $$PWD/structures/handlers/mesh_vertex_color_handler.cpp \
    $$PWD/structures/handlers/mesh_vertex_handler.cpp \
    $$PWD/structures/handlers/mesh_vertex_normal_handler.cpp \
    $$PWD/structures/handlers/mesh_vertex_uv_handler.cpp \
    $$PWD/structures/handlers/mesh_wedge_normal_handler.cpp \
    $$PWD/structures/handlers/mesh_wedge_uv_handler.cpp \
    $$PWD/io/animation_io.cpp \
    $$PWD/io/animation_io_data.cpp \
    $$PWD/io/animation_io_ska.cpp \
    $$PWD/io/mesh_io.cpp \
    $$PWD/io/mesh_io_data.cpp \
    $$PWD/io/mesh_io_obj.cpp \
    $$PWD/io/model_io.cpp \
    $$PWD/io/model_io_data.cpp \
    $$PWD/io/model_io_rig.cpp \
    $$PWD/io/skeleton_io.cpp \
    $$PWD/io/skeleton_io_data.cpp \
    $$PWD/io/skeleton_io_skt.cpp \
    $$PWD/io/skinning_weights_io.cpp \
    $$PWD/io/skinning_weights_io_data.cpp \
    $$PWD/io/skinning_weights_io_skw.cpp \
    $$PWD/structures/abstract_mesh.cpp \
    $$PWD/structures/animation.cpp \
    $$PWD/structures/animation_frame.cpp \
    $$PWD/structures/face_mesh.cpp \
    $$PWD/structures/mesh_array_face.cpp \
    $$PWD/structures/mesh_face.cpp \
    $$PWD/structures/mesh_face_navigator.cpp \
    $$PWD/structures/mesh_material.cpp \
    $$PWD/structures/mesh_polyline.cpp \
    $$PWD/structures/mesh_vector_face.cpp \
    $$PWD/structures/mesh_vertex.cpp \
    $$PWD/structures/model.cpp \
    $$PWD/structures/polyline_mesh.cpp \
    $$PWD/structures/skeleton.cpp \
    $$PWD/structures/skeleton_joint.cpp \
    $$PWD/structures/skinning_weights.cpp \
    $$PWD/structures/vertex_mesh.cpp

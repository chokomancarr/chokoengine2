target_sources(ChokoEngine PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/asset.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/animclip.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/animgraph.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/armature.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/armature_bone.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/background.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/cubemap.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/font.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/material.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/mesh.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/rendertarget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/rendertarget3.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/shader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/texture.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/texture3.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/texture_internal.cpp
)

add_subdirectory("animclip")
add_subdirectory("animgraph")
add_subdirectory("shader")
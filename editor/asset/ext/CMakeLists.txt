target_sources(ChokoEditor PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/vshader_node.cpp
)

add_subdirectory(vshader/inputs)
add_subdirectory(vshader/outputs)
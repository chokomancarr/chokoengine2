target_sources(ChokoEngine PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/animator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/camera.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/collider.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/light.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/mesh_renderer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/rig.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/rigidbody.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/script.cpp
)

add_subdirectory(mesh)
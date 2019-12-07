target_sources(ChokoEngine PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/component.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/scene.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/scene_object.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transform.cpp
)

add_subdirectory(comp)
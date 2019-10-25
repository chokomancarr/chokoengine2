target_sources(ChokoEngine PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/component.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/object_base.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/scene.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/sceneobject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/transform.cpp
)

add_subdirectory(comp)
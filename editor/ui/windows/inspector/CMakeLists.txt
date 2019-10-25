target_sources(ChokoEditor PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/component.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/sceneobject.cpp
)

add_subdirectory(comp)
add_subdirectory(status)
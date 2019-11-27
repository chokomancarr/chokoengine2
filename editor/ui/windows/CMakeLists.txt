target_sources(ChokoEditor PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/ew_browser.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ew_gameview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ew_hierarchy.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ew_inspector.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ew_sceneview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ew_shadereditor.cpp
)

add_subdirectory(inspector)
add_subdirectory(ops)
add_subdirectory(sceneview)
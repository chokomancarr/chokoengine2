target_sources(ChokoEditor PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/colors.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dragdrop.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dropdown_menu.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/window.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/windowmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/overlaymanager.cpp
)

add_subdirectory(layout)
add_subdirectory(overlay)
add_subdirectory(windows)
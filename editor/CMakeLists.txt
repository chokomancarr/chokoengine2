target_sources(ChokoEditor PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/chokoeditor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/editor_paths.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_subdirectory(scripting)
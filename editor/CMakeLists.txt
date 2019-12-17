target_sources(ChokoEditor PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/chokoeditor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/editor_paths.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_subdirectory(asset)
add_subdirectory(builder)
add_subdirectory(exporters)
add_subdirectory(ext)
add_subdirectory(hooks)
add_subdirectory(preview)
add_subdirectory(res)
add_subdirectory(scene)
add_subdirectory(scripting)
add_subdirectory(serial)
add_subdirectory(ui)
add_subdirectory(utils)
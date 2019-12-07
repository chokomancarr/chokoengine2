target_sources(ChokoEngine PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/chokoengine.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/chokolait.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/object_base.cpp
)

add_subdirectory(3rdparty)
add_subdirectory(asset)
add_subdirectory(backend)
add_subdirectory(core)
add_subdirectory(ext)
add_subdirectory(parsers)
add_subdirectory(scene)
add_subdirectory(types)
add_subdirectory(utils)
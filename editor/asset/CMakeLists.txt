target_sources(ChokoEditor PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/asset_list.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/asset_loader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/asset_manager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/asset_writer.cpp
)

add_subdirectory(archiver)
add_subdirectory(ext)
add_subdirectory(ops)
add_subdirectory(writer)
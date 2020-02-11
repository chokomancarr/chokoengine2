target_sources(Module_AssetArchive PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/asset_loader.cpp
)

add_subdirectory(assets)
add_subdirectory(vfs)
target_sources(ChokoEditor PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/asset_list.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/asset_loader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/prefab.cpp
)

add_subdirectory(ext)
add_subdirectory("parsers/comp")
target_sources(ChokoEditor PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/asset_archiver.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/deflate.cpp
)

add_subdirectory(getdeps)
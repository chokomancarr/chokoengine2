target_sources(ChokoEditor PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/loader_generator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/script_val.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/scripting.cpp
)

add_subdirectory(comp)
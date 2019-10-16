target_sources(ChokoEngine PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/debug.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/display.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/input.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/interactive_ui.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/io.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/mvp.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/time.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ui.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/world_ui.cpp
)

add_subdirectory("types")
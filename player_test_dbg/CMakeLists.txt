# CMake script for building ChokoEditor projects. DO NOT EDIT.
# The following vars most be defined:
#   
# The following files must be available:
#   system/game_scripts.cmake

cmake_minimum_required(VERSION 3.3)
project (chokoeditor_project)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../share")

find_package(ChokoEngine REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (MSVC)
	SET(CMAKE_CXX_FLAGS "/EHsc /W3")
else()
	SET(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Wno-unknown-pragmas -Wno-sign-compare -Wno-unused-parameter -Wno-ignored-attributes")
	add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-std=c++1y> $<$<COMPILE_LANGUAGE:C>:-std=c99>)
if (LINUX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
else()
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing")
endif()
endif()

add_executable(chokoeditor_project "")
target_include_directories(chokoeditor_project PUBLIC ${CE2_INCLUDE_DIRS})
include(system/system_scripts.cmake)
include(system/game_scripts.cmake)
target_link_libraries(chokoeditor_project ${CE2_LIBRARIES})
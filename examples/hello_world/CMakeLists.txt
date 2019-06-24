cmake_minimum_required(VERSION 3.3)
project (hello_world)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${CMAKE_SOURCE_DIR}/../cmake/")

find_package(ChokoEngine REQUIRED)

if (MSVC)
	add_definitions(-DPLATFORM_WIN -DWIN32)
elseif (APPLE)
	add_definitions(-DPLATFORM_MAC -DGL_SILENCE_DEPRECATION -DCL_SILENCE_DEPRECATION)
elseif (UNIX)
	add_definitions(-DPLATFORM_LNX)
else()
	message(FATAL_ERROR "Platform not supported!")
endif()

if (MSVC)
	SET(CMAKE_CXX_FLAGS "/EHsc /W3")
else()
	SET(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")
endif()

include_directories("$<TARGET_PROPERTY:ChokoEngine,INTERFACE_INCLUDE_DIRECTORIES>")

add_executable(hello_world main.cpp)
target_link_libraries(hello_world ChokoEngine)
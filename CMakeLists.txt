cmake_minimum_required(VERSION 3.3)
project (chokoengine2)

if (UNIX AND NOT APPLE)
	set(LINUX TRUE)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(JPEG REQUIRED)
find_package(Freetype REQUIRED)
find_package(Threads REQUIRED)

if (MSVC)
	add_definitions(-DPLATFORM_WIN -DWIN32)
elseif (LINUX)
	add_definitions(-DPLATFORM_LNX)
elseif (APPLE)
	add_definitions(-DPLATFORM_OSX -DGL_SILENCE_DEPRECATION -DCL_SILENCE_DEPRECATION)
else()
	message(FATAL_ERROR "Platform not supported!")
endif()

if (MSVC)
	SET(CMAKE_CXX_FLAGS "/EHsc /W3")
else()
	SET(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Wno-unknown-pragmas -Wno-sign-compare -Wno-unused-parameter -Wno-ignored-attributes")
	add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-std=c++11> $<$<COMPILE_LANGUAGE:C>:-std=c99>)
if (LINUX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
else()
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-narrowing")
endif()
endif()

set(INC_DIRS
	${PROJECT_SOURCE_DIR}/include/ce2
	${PROJECT_SOURCE_DIR}/src
	${GLFW3_INCLUDE_DIR}
	${GLM_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${JPEG_INCLUDE_DIR}
	${FREETYPE_INCLUDE_DIRS}
)
if (MSVC)
    set(INC_DIRS
        ${INC_DIRS}
        ${PROJECT_SOURCE_DIR}/src/winonly
    )
endif()

set(LINK_DIRS )

set(LIBS
	${OPENGL_gl_LIBRARY}
	glfw
	${GLM_LIBRARY}
	${GLEW_LIBRARY}
	${JPEG_LIBRARY}
	${FREETYPE_LIBRARIES}
)
if(MSVC)
    set(LIBS
        ${LIBS}
    )
elseif(APPLE)
    set(LIBS
        ${LIBS}
        dl
        pthread
        util
    )
else()
    set(LIBS
        ${LIBS}
        dl
        pthread
        rt
        util
    )
endif()

if(MSVC)
    set(SOURCES
        cpp.hint
    )
else()
	set(SOURCES)
endif()

include_directories(${INC_DIRS})
link_directories(${LINK_DIRS})

add_subdirectory("src")
add_subdirectory("src/core")
add_subdirectory("src/utils")

add_library(chokoengine2 SHARED ${SOURCES} ${HEADERS})

target_link_libraries(chokoengine2 ${LIBS})
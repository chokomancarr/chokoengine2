cmake_minimum_required(VERSION 3.3)
project (ChokoEngine VERSION 2.0.0 DESCRIPTION "asdf")

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
	add_definitions(-DPLATFORM_MAC -DGL_SILENCE_DEPRECATION -DCL_SILENCE_DEPRECATION)
else()
	message(FATAL_ERROR "Platform not supported!")
endif()

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

set(INC_DIRS
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
	${GLFW3_INCLUDE_DIR}
	${GLM_INCLUDE_DIR}
	${GLEW_INCLUDE_DIR}
	${JPEG_INCLUDE_DIR}
	${FREETYPE_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/chokoballs/include
)
if (MSVC)
    set(INC_DIRS
        ${INC_DIRS}
        ${PROJECT_SOURCE_DIR}/include/ce2/winonly
    )
endif()

if (MSVC)
    set(LINK_DIRS
        ${PROJECT_SOURCE_DIR}/chokoballs/build/lib/Release
    )
else()
    set(LINK_DIRS
        ${PROJECT_SOURCE_DIR}/chokoballs/build/lib
    )
endif()

set(LIBS
	${OPENGL_gl_LIBRARY}
	glfw
	${GLM_LIBRARY}
	${GLEW_LIBRARY}
	${JPEG_LIBRARY}
	${FREETYPE_LIBRARIES}
	#ChokoBalls
)
if(MSVC)
    set(LIBS
        ${LIBS}
		-Profile
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
		-Wl,-z,defs
    )
endif()

include_directories(${INC_DIRS})
link_directories(${LINK_DIRS})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)

add_library(ChokoEngine SHARED "")
target_compile_definitions(ChokoEngine PRIVATE BUILD_CHOKOENGINE=1)

if(MSVC)
    target_sources(ChokoEngine PRIVATE cpp.hint)
endif()

add_subdirectory("src")

include(GenerateExportHeader)
generate_export_header(ChokoEngine)

set_target_properties(ChokoEngine PROPERTIES PUBLIC_HEADER include/ce2/chokoengine.hpp)

target_link_libraries(ChokoEngine ${LIBS})


#------editor-------

add_executable(ChokoEditor "")
add_subdirectory("editor")

target_include_directories(ChokoEditor PRIVATE editor)

set(LIBS
	${LIBS}
	ChokoEngine
)

target_link_libraries(ChokoEditor ${LIBS})

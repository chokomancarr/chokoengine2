cmake_minimum_required(VERSION 3.3)
project (ChokoEngine VERSION 2.0.0 DESCRIPTION "asdf")

if (UNIX AND NOT APPLE)
	set(LINUX TRUE)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
if (NOT APPLE)
	find_package(GLEW REQUIRED)
endif()
find_package(JPEG REQUIRED)
find_package(Freetype REQUIRED)
find_package(ZLIB REQUIRED)
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
	${JPEG_INCLUDE_DIR}
	${FREETYPE_INCLUDE_DIRS}
	${ZLIB_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/chokoballs/include
)
if (NOT APPLE)
set(INC_DIRS
	${INC_DIRS}
	${GLEW_INCLUDE_DIR}
)
endif()
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
	${JPEG_LIBRARY}
	${FREETYPE_LIBRARIES}
	${ZLIB_LIBRARIES}
	#ChokoBalls
)
if (NOT APPLE)
set(LIBS
	${LIBS}
	${GLEW_LIBRARY}
)
endif()
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

add_subdirectory(src)

set(enum_gen_strs
	${PROJECT_SOURCE_DIR}/include/ce2/asset/enums/asset_types
	${PROJECT_SOURCE_DIR}/include/ce2/core/enums/input_key
	${PROJECT_SOURCE_DIR}/include/ce2/scene/enums/component_types
)

foreach (enumfl ${enum_gen_strs})
	ADD_CUSTOM_COMMAND(OUTPUT "${enumfl}_str.hpp"
		COMMAND python "${PROJECT_SOURCE_DIR}/preproc/enum2str.py" "${enumfl}.hpp"
		DEPENDS "${enumfl}.hpp" "${PROJECT_SOURCE_DIR}/preproc/enum2str.py"
	)
	target_sources(ChokoEngine PRIVATE
		"${enumfl}_str.hpp"
	)
endforeach()

target_link_libraries(ChokoEngine ${LIBS})

set(LIBS
	${LIBS}
	ChokoEngine
)

#------modules-------

add_library(Module_AssetCommon STATIC "")
add_subdirectory(src/modules/assets_common)
target_link_libraries(Module_AssetCommon ${LIBS})

set(LIBS_MA
	${LIBS}
	Module_AssetCommon
)

add_library(Module_AssetArchive STATIC "")
target_compile_definitions(Module_AssetArchive PRIVATE BUILD_MODULE_AA=1)
add_subdirectory(src/modules/assets_archive)
target_include_directories(Module_AssetArchive PRIVATE include/ce2/modules/aa)
target_link_libraries(Module_AssetArchive ${LIBS_MA})

add_library(Module_AssetEditor STATIC "")
target_compile_definitions(Module_AssetEditor PRIVATE BUILD_MODULE_AE=1)
add_subdirectory(src/modules/assets_editor)
target_include_directories(Module_AssetEditor PRIVATE include/ce2/modules/ae)
target_link_libraries(Module_AssetEditor ${LIBS_MA})

set(LIBS_E
	${LIBS}
	Module_AssetEditor
)

add_library(Module_PlayerEditor STATIC "")
target_compile_definitions(Module_PlayerEditor PRIVATE BUILD_MODULE_PE=1)
add_subdirectory(src/modules/player_editor)
target_include_directories(Module_PlayerEditor PRIVATE include/ce2/modules/pe)
target_link_libraries(Module_PlayerEditor ${LIBS_E})

#------editor-------

add_executable(ChokoEditor "")
add_subdirectory(editor)

add_dependencies(ChokoEditor Module_AssetEditor Module_PlayerEditor)

set(enum_gen_strs
	${PROJECT_SOURCE_DIR}/editor/hooks/callback_list
)

foreach (enumfl ${enum_gen_strs})
	ADD_CUSTOM_COMMAND(OUTPUT "${enumfl}_str.hpp"
		COMMAND python "${PROJECT_SOURCE_DIR}/preproc/enum2str.py" "${enumfl}.hpp"
		DEPENDS "${enumfl}.hpp" "${PROJECT_SOURCE_DIR}/preproc/enum2str.py"
	)
	target_sources(ChokoEditor PRIVATE
		"${enumfl}_str.hpp"
	)
endforeach()

target_include_directories(ChokoEditor PRIVATE editor)
target_link_libraries(ChokoEditor ${LIBS_E})

#------tasks------

set(taskrunner_list
	builder_dbg
)

foreach(tasknm ${taskrunner_list})
	add_executable(${tasknm} "")
	add_subdirectory(editor/taskrunners/${tasknm})

	target_include_directories(${tasknm} PRIVATE editor/taskrunners/${tasknm})
	target_link_libraries(${tasknm} ${LIBS})
endforeach()
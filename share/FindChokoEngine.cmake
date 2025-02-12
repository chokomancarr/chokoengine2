# This module defines
#   CE2_LIBRARIES
#   CE2_FOUND
#   CE2_INCLUDE_DIRS

SET(CE2_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	${CE2_PATH}
)

if (MSVC)
	add_definitions(-DPLATFORM_WIN -DWIN32)
elseif (LINUX)
	add_definitions(-DPLATFORM_LNX)
elseif (APPLE)
	add_definitions(-DPLATFORM_MAC -DGL_SILENCE_DEPRECATION -DCL_SILENCE_DEPRECATION)
else()
	message(FATAL_ERROR "CE2: Platform not supported!")
endif()

FIND_PATH(CE2_INCLUDE_TEMP chokoengine.hpp
	HINTS
	$ENV{CE2DIR}
	PATH_SUFFIXES include/CE2 include
	PATHS ${CE2_SEARCH_PATHS}
)

FIND_LIBRARY(CE2_LIBRARY_TEMP
	NAMES ChokoEngine
	HINTS
	$ENV{CE2DIR}
	PATH_SUFFIXES lib64 lib
	PATHS ${CE2_SEARCH_PATHS}
)

IF(CE2_LIBRARY_TEMP)
	find_package(Threads REQUIRED)
	find_package(OpenGL REQUIRED)
	find_package(glfw3 REQUIRED)
	find_package(glm REQUIRED)
	find_package(JPEG REQUIRED)
	find_package(Freetype REQUIRED)
	find_package(ZLIB REQUIRED)
	if (NOT APPLE)
		find_package(GLEW REQUIRED)
	endif()

	if (MSVC)
		add_definitions(-DPLATFORM_WIN -DWIN32)
	elseif (LINUX)
		add_definitions(-DPLATFORM_LNX)
	elseif (APPLE)
		add_definitions(-DPLATFORM_MAC -DGL_SILENCE_DEPRECATION -DCL_SILENCE_DEPRECATION)
	else()
		message(FATAL_ERROR "Platform not supported for ChokoEngine!")
	endif()

	SET(CE2_INCLUDE_DIRS
		${CE2_INCLUDE_TEMP}
		${GLFW3_INCLUDE_DIR}
		${GLM_INCLUDE_DIR}
		${GLEW_INCLUDE_DIR}
		${JPEG_INCLUDE_DIR}
		${FREETYPE_INCLUDE_DIRS}
		${ZLIB_INCLUDE_DIRS}
	)

	IF(NOT APPLE)
		SET(CE2_LIBRARY_TEMP
			${CE2_LIBRARY_TEMP}
			${CMAKE_THREAD_LIBS_INIT}
		)
	ENDIF(NOT APPLE)

	SET(CE2_LIBRARIES
		${CE2_LIBRARY_TEMP}
		${OPENGL_gl_LIBRARY}
		glfw
		${GLM_LIBRARY}
		${JPEG_LIBRARY}
		${FREETYPE_LIBRARIES}
		${ZLIB_LIBRARIES}
	)
	if(MSVC)
		set(CE2_LIBRARIES
			${CE2_LIBRARIES}
			${GLEW_LIBRARY}
			-Profile
		)
	elseif(APPLE)
		set(CE2_LIBRARIES
			${CE2_LIBRARIES}
			dl
			pthread
			util
		)
	else()
		set(CE2_LIBRARIES
			${CE2_LIBRARIES}
			${GLEW_LIBRARY}
			dl
			pthread
			rt
			util
			-Wl,-z,defs
		)
	endif()

	get_filename_component(CE2_LIB_PATH "${CE2_LIBRARY_TEMP}" DIRECTORY)
	
	macro(define_module fullname alias)
		FIND_LIBRARY(CE2_MODULE_LIB_${alias}
			NAMES Module_${fullname}
			HINTS
			$ENV{CE2DIR}
			PATH_SUFFIXES lib64 lib
			PATHS ${CE2_SEARCH_PATHS}
		)
	endmacro()

	define_module(AssetArchive AA)
	define_module(AssetCommon AC)
	define_module(AssetEditor AE)
	define_module(PlayerEditor PE)
	define_module(PlayerRelease PR)

	set(CE2_INCLUDE_TEMP "${CE2_INCLUDE_TEMP}" CACHE INTERNAL "")
	set(CE2_INCLUDE_DIRS "${CE2_INCLUDE_DIRS}" CACHE STRING "")
	set(CE2_LIBRARY_TEMP "${CE2_LIBRARY_TEMP}" CACHE INTERNAL "")
	set(CE2_LIBRARIES "${CE2_LIBRARIES}" CACHE STRING "")

ENDIF()

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(CE2 REQUIRED_VARS CE2_LIBRARIES CE2_INCLUDE_DIRS)


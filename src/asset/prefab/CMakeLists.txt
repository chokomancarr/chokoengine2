target_sources(ChokoEngine PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/prefab_comp.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/prefab_compref.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/prefab_inc.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/prefab_item.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/prefab_link.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/prefab_mod.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/prefab_obj.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/prefab_objbase.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/prefab_objref.cpp
)

add_subdirectory(comp)
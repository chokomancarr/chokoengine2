target_sources(ChokoEditor PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/serializer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/serialized_component.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/serialized_item.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/serialized_object.cpp
)

add_subdirectory(comp)
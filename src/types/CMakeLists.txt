set(DIR src/types)

set(SOURCES
	${SOURCES}
	${DIR}/color.cpp
	${DIR}/depth_texture.cpp
	${DIR}/framebuffer.cpp
	${DIR}/mat4x4.cpp
	${DIR}/quat.cpp
	${DIR}/rect.cpp
	${DIR}/texturebuffer.cpp
	${DIR}/vec2.cpp
	${DIR}/vec3.cpp
	${DIR}/vec4.cpp
	${DIR}/vertexbuffer.cpp
	${DIR}/vertexobject.cpp
	PARENT_SCOPE
)

set(DIR src/asset)

set(SOURCES
	${SOURCES}
	${DIR}/asset.cpp
	${DIR}/animclip.cpp
	${DIR}/animgraph.cpp
	${DIR}/armature.cpp
	${DIR}/armature_bone.cpp
	${DIR}/background.cpp
	${DIR}/cubemap.cpp
	${DIR}/font.cpp
	${DIR}/material.cpp
	${DIR}/mesh.cpp
	${DIR}/rendertarget.cpp
	${DIR}/rendertarget3.cpp
	${DIR}/shader.cpp
	${DIR}/texture.cpp
	${DIR}/texture3.cpp
	${DIR}/texture_internal.cpp
	PARENT_SCOPE
)

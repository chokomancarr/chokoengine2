set(DIR src/utils)

set(SOURCES
	${SOURCES}
	${DIR}/math.cpp
	${DIR}/procedural_mesh.cpp
	${DIR}/random.cpp
	${DIR}/subprocess.cpp
	${DIR}/transform_feedback.cpp
	${DIR}/unique_caller.cpp
	PARENT_SCOPE
)

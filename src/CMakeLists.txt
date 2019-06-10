set(DIR include/ce2/core)

set(SOURCES
	${SOURCES}
	${DIR}/ChokoLait.cpp
	${DIR}/detector.cpp
	${DIR}/DummyEditor.cpp
	${DIR}/Editor.cpp
	${DIR}/EditorShortcuts.cpp
	${DIR}/Engine.cpp
	${DIR}/hdr.cpp
	${DIR}/lodepng.cpp
	${DIR}/main.cpp
	${DIR}/main_client.cpp
	${DIR}/main_controller.cpp
	${DIR}/RenderEngine.cpp
	${DIR}/Types.cpp
	${DIR}/virtuber.cpp
	${DIR}/vlcstream.cpp
	PARENT_SCOPE
)
set(HEADERS
	${HEADERS}
	${DIR}/ChokoLait.h
	${DIR}/Defines.h
	${DIR}/detector.hpp
	${DIR}/DummyEditor.h
	${DIR}/Editor.h
	${DIR}/Engine.h
	${DIR}/hdr.h
	${DIR}/lodepng.h
	${DIR}/mains.h
	${DIR}/SceneObjects.h
	${DIR}/virtuber.h
	${DIR}/vlcstream.h
	PARENT_SCOPE
)


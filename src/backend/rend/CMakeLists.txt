set(DIR src/backend/rend)

set(SOURCES
    ${SOURCES}
    ${DIR}/renderer.cpp
    ${DIR}/renderer_lights.cpp
    ${DIR}/renderer_probes.cpp
    PARENT_SCOPE
)
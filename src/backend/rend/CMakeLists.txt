target_sources(ChokoEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_lights.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_particles.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/renderer_probes.cpp
)

add_subdirectory(GI)
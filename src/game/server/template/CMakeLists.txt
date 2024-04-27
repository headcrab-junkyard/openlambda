project(ogs-game-server-template)

add_library(${PROJECT_NAME} MODULE)

target_include_directories(${PROJECT_NAME}
	PRIVATE ${OGS_ROOT}/game/shared
)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "server"
)

target_link_libraries(${PROJECT_NAME} ogs-tier1 ogs-mathlib)
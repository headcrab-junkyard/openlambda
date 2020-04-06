project(ogs-game-server)

set(LIB_TYPE MODULE)

if(${OPENLAMBDA_TARGET_ENGINE} STREQUAL "Legacy")
	set(LIB_TYPE STATIC)
endif()

add_library(${PROJECT_NAME} ${LIB_TYPE})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../shared)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
)

if(${OPENLAMBDA_TARGET_ENGINE} STREQUAL "Next")
	set_target_properties(${PROJECT_NAME} PROPERTIES
		OUTPUT_NAME "server"
	)

	target_link_libraries(${PROJECT_NAME} ogs-tier1) #ogs-pm_shared ogs-mathlib ogs_game_shared-server)
endif()
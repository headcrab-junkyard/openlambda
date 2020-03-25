project(ogs-game-client)

set(LIB_TYPE MODULE)

if(OPENLAMBDA_TARGET_ENGINE STREQUAL "Legacy")
	set(LIB_TYPE STATIC)
endif()

add_library(${PROJECT_NAME} ${LIB_TYPE})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME} ogs_game_shared-client)

if(OPENLAMBDA_TARGET_ENGINE STREQUAL "Next")
	set_target_properties(${PROJECT_NAME} PROPERTIES
		OUTPUT_NAME "client"
	)

	target_link_libraries(${PROJECT_NAME} ogs-tier1 ogs-pm_shared ogs-mathlib)
if(WIN32)
	target_link_libraries(${PROJECT_NAME} winmm) #joyGetPosEx
endif()
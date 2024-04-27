project(ogs-game-client)

###############################################################################

# For Next we build the code into a separate library
set(LIB_TYPE MODULE)

# While for legacy we put everything into a static lib to let another module link against it
if(${OPENLAMBDA_TARGET_ENGINE} STREQUAL "Legacy")
	set(LIB_TYPE STATIC)
endif()

add_library(${PROJECT_NAME} ${LIB_TYPE})

target_compile_definitions(${PROJECT_NAME}
	PRIVATE MODULE_EXPORT # TODO: MODULE_EXPORTS?
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/../shared
)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
)

if(${OPENLAMBDA_GAME_TYPE} STREQUAL "Template")
	add_subdirectory(template)
elseif(${OPENLAMBDA_GAME_TYPE} STREQUAL "Quake")
	add_subdirectory(quake)
elseif(${OPENLAMBDA_GAME_TYPE} STREQUAL "HL")
	add_subdirectory(hl)
elseif(${OPENLAMBDA_GAME_TYPE} STREQUAL "CStrike")
	add_subdirectory(cstrike)
endif()

###############################################################################

#target_link_libraries(${PROJECT_NAME} ogs-game-shared-client)

if(${OPENLAMBDA_TARGET_ENGINE} STREQUAL "Next")
	set_target_properties(${PROJECT_NAME} PROPERTIES
		OUTPUT_NAME "client"
	)
	
	target_link_libraries(${PROJECT_NAME} ogs-tier1 ogs-pm_shared ogs-mathlib)
endif()

###############################################################################

install(TARGETS ${PROJECT_NAME} DESTINATION ./${OGS_GAME_FOLDER_NAME}/bin)
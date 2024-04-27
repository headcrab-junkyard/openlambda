project(ogs-game-shared)

###############################################################################

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} COMMON_SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/hl HL_SOURCES)

###############################################################################

if(NOT OGS_DEDICATED)
	add_library(${PROJECT_NAME}-client STATIC)

	target_include_directories(${PROJECT_NAME}-client
		INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
	)

	target_compile_definitions(${PROJECT_NAME}-client PRIVATE OGS_CLIENT_DLL)

	target_sources(${PROJECT_NAME}-client
		PRIVATE ${COMMON_SOURCES}
		PRIVATE ${HL_SOURCES}
	)
endif()

###############################################################################

add_library(${PROJECT_NAME}-server STATIC)

target_include_directories(${PROJECT_NAME}-server
	INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(${PROJECT_NAME}-server PRIVATE OGS_GAME_DLL)

target_sources(${PROJECT_NAME}-server
	PRIVATE ${COMMON_SOURCES}
	PRIVATE ${HL_SOURCES}
)

###############################################################################

if(${OPENLAMBDA_GAME_TYPE} STREQUAL "Template")
	add_subdirectory(template)
elseif(${OPENLAMBDA_GAME_TYPE} STREQUAL "Quake")
	add_subdirectory(quake)
elseif(${OPENLAMBDA_GAME_TYPE} STREQUAL "HL")
	add_subdirectory(hl)
elseif(${OPENLAMBDA_GAME_TYPE} STREQUAL "CStrike")
	add_subdirectory(cstrike)	add_subdirectory(cstrike)endif()
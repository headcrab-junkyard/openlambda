project(ogs-dlls)

###############################################################################

add_library(${PROJECT_NAME} MODULE)

#target_include_directories(${PROJECT_NAME}
#	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../game/shared
#)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
)

###############################################################################

set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME ${OGS_GAMEDLL_OUTPUT_NAME}
)

target_link_libraries(${PROJECT_NAME} ogs-tier1 ogs-game-server) #ogs-pm_shared ogs-mathlib ogs-game-shared-server)

#target_include_directories(${PROJECT_NAME}
	#PRIVATE ${OGS_ROOT}/game_shared
#)

#target_link_libraries(${PROJECT_NAME} ogs-pm_shared ogs-mathlib ogs-game-shared-server)

###############################################################################

install(TARGETS ${PROJECT_NAME} DESTINATION ./${OGS_GAME_FOLDER_NAME}/dlls)
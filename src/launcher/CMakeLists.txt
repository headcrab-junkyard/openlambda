project(ogs-launcher)

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

if(WIN32)
	set(PROJECT_TARGET_PLATFORM win)
elseif(UNIX)
	set(PROJECT_TARGET_PLATFORM linux)
else()
	message(FATAL_ERROR "Unsupported platform!")
endif()

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_TARGET_PLATFORM} PROJECT_PLATFORM_SOURCES)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
	PRIVATE ${PROJECT_PLATFORM_SOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME ${OGS_LAUNCHER_APP_OUTPUT_NAME}
)

target_link_libraries(${PROJECT_NAME} ogs-tier1)

if(UNIX)
	target_link_libraries(${PROJECT_NAME} -ldl)
endif()

###############################################################################

install(TARGETS ${PROJECT_NAME} DESTINATION .)
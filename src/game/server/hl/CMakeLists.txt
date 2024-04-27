aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES_MOD)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES_MOD}
)

add_subdirectory(ammo)
add_subdirectory(items)
add_subdirectory(monsters)
add_subdirectory(xen)
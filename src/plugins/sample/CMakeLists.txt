project(ogs-plugins-sample)

#####################################################################

# Find the required packages

#####################################################################

add_library(${PROJECT_NAME} MODULE)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "sample"
)

target_link_directories(${PROJECT_NAME} ogs-tier1)
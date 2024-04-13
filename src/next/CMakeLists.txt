###############################################################################

# Add subdirectories

add_subdirectory(game)

# If not building as dedicated server then build these modules too
if(NOT OGS_DEDICATED)
	if(${OPENLAMBDA_TARGET_ENGINE} STREQUAL "Next")
		add_subdirectory(gameui)
		add_subdirectory(launcher_main)
	endif()
endif()

# For Next we can support external modules implemented via plugins
if(${OPENLAMBDA_TARGET_ENGINE} STREQUAL "Next")
	add_subdirectory(plugins)
endif()
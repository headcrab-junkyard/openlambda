###############################################################################

# Add subdirectories

add_subdirectory(game)

if(${OPENLAMBDA_TARGET_ENGINE} STREQUAL "Next")
	if(OGS_DEDICATED)
		add_subdirectory(dedicated)
	else()
		# If not building as dedicated server then build these modules too
		add_subdirectory(gameui)
		add_subdirectory(launcher)
		add_subdirectory(launcher_main) # TODO: launcher_app?
	endif()
	
	# For Next we can support external modules implemented via plugins
	add_subdirectory(plugins)
endif()
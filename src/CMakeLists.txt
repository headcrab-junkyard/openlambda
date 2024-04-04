# We want shared libraries (and modules) without any prefix in name
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_MODULE_PREFIX "")

###############################################################################

# Add include directories
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/common
	${CMAKE_CURRENT_SOURCE_DIR}/public
	${OGS_SDK_PATH}
	${OGS_SDK_PATH}/common
	${OGS_SDK_PATH}/engine
	${OGS_SDK_PATH}/public
	${OGS_SDK_PATH}/next
)

###############################################################################

# Add subdirectories

add_subdirectory(pm_shared)
add_subdirectory(game)

if(${OPENLAMBDA_TARGET_ENGINE} STREQUAL "Legacy")
	add_subdirectory(dlls)
else() # For Next we can support external modules implemented via plugins
	add_subdirectory(plugins)
endif()

add_subdirectory(dedicated)

# If not building as dedicated server then build these modules too
if(NOT OGS_DEDICATED)
	if(${OPENLAMBDA_TARGET_ENGINE} STREQUAL "Legacy")
		add_subdirectory(cl_dll)
	endif()
	
	add_subdirectory(gameui)
	add_subdirectory(launcher)
endif()

add_subdirectory(next)
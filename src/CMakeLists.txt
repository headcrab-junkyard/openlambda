# We want shared libraries (and modules) without any prefix in name
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_MODULE_PREFIX "")

set(OGS_GAME_FOLDER_NAME "goldsrctest" CACHE STRING "Game folder name to install the built game modules to")
set(OGS_LAUNCHER_APP_OUTPUT_NAME "launcher" CACHE STRING "Launcher application output name")
set(OGS_GAMEDLL_OUTPUT_NAME "hl" CACHE STRING "Legacy game dll output name")

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

add_subdirectory(game) # TODO: temp

if(${OPENLAMBDA_TARGET_ENGINE} STREQUAL "Legacy")
	add_subdirectory(dlls)
	
	if(OGS_DEDICATED)
		add_subdirectory(dedicated)
	else()
		# If not building as dedicated server then build these modules too
		add_subdirectory(cl_dll)
		add_subdirectory(gameui)
		add_subdirectory(launcher)
	endif()
endif()

add_subdirectory(next)
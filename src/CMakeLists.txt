# We want shared libraries (and modules) without any prefix in name
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_MODULE_PREFIX "")

#####################################################################

# Add include directories
include_directories(
	${OGS_SDK_PATH}
	${OGS_SDK_PATH}/common
	${OGS_SDK_PATH}/engine
	${OGS_SDK_PATH}/pm_shared
	${OGS_SDK_PATH}/public
	${OGS_SDK_PATH}/next
)

#####################################################################

# Add subdirectories

add_subdirectory(pm_shared)

add_subdirectory(game)

if(${OPENLAMBDA_TARGET_ENGINE} STREQUAL "Legacy")
	add_subdirectory(cl_dll)
	add_subdirectory(dlls)
elseif(${OPENLAMBDA_TARGET_ENGINE} STREQUAL "Next")
	add_subdirectory(gameui)
endif()

#add_subdirectory(dedicated)
add_subdirectory(launcher)
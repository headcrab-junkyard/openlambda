#####################################################################

# Add include directories
include_directories(
	${OGS_SDK_PATH}
)

#####################################################################

# Add subdirectories

add_subdirectory(pm_shared)

#add_subdirectory(cl_dll)
#add_subdirectory(dlls)
add_subdirectory(game)

#add_subdirectory(gameui)

#add_subdirectory(dedicated)
add_subdirectory(launcher)
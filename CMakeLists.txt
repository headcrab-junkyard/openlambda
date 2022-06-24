cmake_minimum_required(VERSION 3.14)
project(OpenLambda)

#####################################################################

# Find the required packages

#####################################################################

# Set C language standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
#set(CMAKE_C_STANDARD_EXTENSIONS ON)

# Set C++ language standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_STANDARD_EXTENSIONS ON)

#####################################################################


# Set path to SDK
set(OGS_SDK_PATH ${CMAKE_SOURCE_DIR}/../ogs-sdk/goldsrc
	CACHE PATH
	"Path to engine SDK"
)

# Set the target engine
set(OPENLAMBDA_TARGET_ENGINE "Next" CACHE STRING "Specify the target engine that the code will be built for")
set_property(CACHE OPENLAMBDA_TARGET_ENGINE PROPERTY STRINGS
	"Legacy"
	"Next"
)

message(STATUS "Building for ${OPENLAMBDA_TARGET_ENGINE} engine!")

# Set the game/mod type
set(OPENLAMBDA_GAME_TYPE "Template" CACHE STRING "Specify the game/mod that the code will be built for")
set_property(CACHE OPENLAMBDA_GAME_TYPE PROPERTY STRINGS
	"Template"
	"HL"
)

message(STATUS "Building for ${OPENLAMBDA_GAME_TYPE} game/mod!")

#####################################################################

# Add subdirectories

# Add engine sdk directory
add_subdirectory(${OGS_SDK_PATH} ${CMAKE_BINARY_DIR}/sdk)

# Add main sources directory
add_subdirectory(src)
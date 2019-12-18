cmake_minimum_required(VERSION 3.14)

# Set C language standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
#set(CMAKE_C_STANDARD_EXTENSIONS ON)

# Set C++ language standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_STANDARD_EXTENSIONS ON)


# Set path to SDK
set(OGS_SDK_PATH ${CMAKE_SOURCE_DIR}/../ogs-sdk/goldsrc
	CACHE PATH
	"Path to engine SDK"
)


# Add engine sdk directory
add_subdirectory(${OGS_SDK_PATH} ${CMAKE_BINARY_DIR}/sdk)

add_subdirectory(src)
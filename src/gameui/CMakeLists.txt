#add_subdirectory(null)

if(${OPENLAMBDA_TARGET_ENGINE} STREQUAL "Next")
	add_subdirectory(genta)
endif()

add_subdirectory(hla)
add_subdirectory(steamera)
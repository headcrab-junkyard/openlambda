#add_subdirectory(null)
add_subdirectory(hla)

if(${OPENLAMBDA_TARGET_ENGINE} STREQUAL "Next")
	add_subdirectory(genta)
elseif(${OPENLAMBDA_TARGET_ENGINE} STREQUAL "Legacy")
	add_subdirectory(steamera)
endif()
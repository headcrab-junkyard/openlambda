project(ogs-game-server)

###############################################################################

# For Next we build the code into a separate library
set(LIB_TYPE MODULE)

# While for legacy we put everything into a static lib to let another module link against it
if(${OPENLAMBDA_TARGET_ENGINE} STREQUAL "Legacy")
	set(LIB_TYPE STATIC)
endif()

add_library(${PROJECT_NAME} ${LIB_TYPE})

target_compile_definitions(${PROJECT_NAME}
	PRIVATE MODULE_EXPORT # TODO: MODULE_EXPORTS?
)

target_include_directories(${PROJECT_NAME}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../shared
)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_BASE_SOURCES)

target_sources(${PROJECT_NAME}
	#PRIVATE ${PROJECT_BASE_SOURCES}
	#PRIVATE ${PROJECT_GAME_SOURCES}
	PRIVATE BaseAmmo.cpp
	PRIVATE BaseAnimated.cpp
	PRIVATE BaseButton.cpp
	#PRIVATE BaseCharacter.cpp
	PRIVATE BaseCycler.cpp
	PRIVATE BaseDelay.cpp
	#PRIVATE BaseEntity.cpp
	#PRIVATE BaseGame.cpp
	PRIVATE BaseItem.cpp
	#PRIVATE BasePlayer.cpp
	PRIVATE BaseToggle.cpp
	PRIVATE BaseTrigger.cpp
	#PRIVATE BaseWeapon.cpp
	PRIVATE bmodels.cpp
	#PRIVATE FuncDoor.cpp
	PRIVATE FuncIllusionary.cpp
	PRIVATE FuncLadder.cpp
	PRIVATE FuncPlat.cpp
	#PRIVATE FuncTrain.cpp
	PRIVATE FuncWall.cpp
	PRIVATE FuncWater.cpp
	#PRIVATE GameClientEventListener.cpp
	#PRIVATE GameRules.cpp
	#PRIVATE GameServerEventListener.cpp
	#PRIVATE GameWorld.cpp
	#PRIVATE InfoIntermission.cpp
	PRIVATE InfoNull.cpp
	PRIVATE InfoPlayerCoop.cpp
	PRIVATE InfoPlayerDeathmatch.cpp
	PRIVATE InfoPlayerStart.cpp
	PRIVATE Light.cpp
	#PRIVATE MomentaryDoor.cpp
	#PRIVATE NetMsgHandlers.cpp
	PRIVATE PathCorner.cpp
	#PRIVATE PickupItem.cpp
	#PRIVATE PlayerMove.cpp
	#PRIVATE ResourceManager.cpp
	#PRIVATE RotatingDoor.cpp
	PRIVATE StringPool.cpp
	PRIVATE SystemEventListener_Game.cpp
	#PRIVATE TriggerAuto.cpp
	#PRIVATE TriggerAutoSave.cpp
	PRIVATE TriggerCDAudio.cpp
	#PRIVATE TriggerChangeLevel.cpp
	PRIVATE TriggerCounter.cpp
	#PRIVATE TriggerEndSection.cpp
	PRIVATE TriggerGravity.cpp
	PRIVATE TriggerHurt.cpp
	PRIVATE TriggerMonsterJump.cpp
	PRIVATE TriggerMulti.cpp
	PRIVATE TriggerOnce.cpp
	PRIVATE TriggerPush.cpp
	PRIVATE TriggerRelay.cpp
	PRIVATE TriggerTeleport.cpp
	PRIVATE Util.cpp
	#PRIVATE weapons.cpp
	#PRIVATE world.cpp
)

if(${OPENLAMBDA_GAME_TYPE} STREQUAL "Template")
	add_subdirectory(template)
elseif(${OPENLAMBDA_GAME_TYPE} STREQUAL "Quake")
	add_subdirectory(quake)
elseif(${OPENLAMBDA_GAME_TYPE} STREQUAL "HL")
	add_subdirectory(hl)
elseif(${OPENLAMBDA_GAME_TYPE} STREQUAL "CStrike")
	add_subdirectory(cstrike)
elseif(${OPENLAMBDA_GAME_TYPE} STREQUAL "CSO")
	#add_subdirectory(cstrike)
	add_subdirectory(cso)
endif()

###############################################################################

if(${OPENLAMBDA_TARGET_ENGINE} STREQUAL "Next")
	set_target_properties(${PROJECT_NAME} PROPERTIES
		OUTPUT_NAME "server"
	)
	
	target_link_libraries(${PROJECT_NAME} ogs-tier1) #ogs-pm_shared ogs-mathlib ogs_game_shared-server)
endif()

###############################################################################

install(TARGETS ${PROJECT_NAME} DESTINATION ./${OGS_GAME_FOLDER_NAME}/bin)
/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018-2019, 2021-2022 BlackPhrase
 *
 * OpenLambda Project is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenLambda Project is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenLambda Project. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include "exports.h"
#include "engine.h"
#include "BaseEntity.hpp"
#include "Game.hpp"
#include "GameSetup_Default.hpp"
#include "Util.hpp"
#include "IGameRules.hpp"

extern "C"
{
#include "pm_shared/pm_shared.h"
};

CGameWorld *gpGameWorld{nullptr};

void GameInit()
{
	auto pGameSetup{new CGameSetup_Default()}; // TODO
	
	//gpGameWorld = pGameSetup->CreateWorld();
	
	// Initialize the game (master) class
	//if(!gpGame)
		//gpGame = new CGame(pGameSetup->CreateRules(), gpGameWorld);
	
	gpGame = reinterpret_cast<IGame*>(Sys_GetFactoryThis()(OGS_GAME_INTERFACE_VERSION, nullptr));
	
	gpGame->Init();
	
	//gpGameWorld = gpGame->GetWorld();
};

int EntitySpawn(edict_t *pent)
{
	auto pBaseEntity{ToBaseEntity(pent)};
	
	if(!pBaseEntity)
		return 0;
	
	if(!pBaseEntity->PreSpawn())
	{
		gpEngine->pfnRemoveEntity(pent);
		return 0;
	};
	
	pBaseEntity->Spawn();
	return 1;
};

// TODO: ToBaseEntity could potentially return nullptr, so we shouldn't directly operate on its return value...
// Or, we should modify the ToBaseEntity func to accept the arg by reference

void EntityThink(edict_t *pent)
{
	ToBaseEntity(pent)->Think();
};

void EntityUse(edict_t *pent, edict_t *pother)
{
	ToBaseEntity(pent)->Use(ToBaseEntity(pother));
};

void EntityTouch(edict_t *pent, edict_t *pother)
{
	ToBaseEntity(pent)->Touch(ToBaseEntity(pother));
};

void EntityBlocked(edict_t *pent, edict_t *pother)
{
	ToBaseEntity(pent)->Blocked(ToBaseEntity(pother));
};

void EntityKeyValue(edict_t *pent, KeyValueData *pkvd)
{
	pkvd->fHandled = 0;
	
	auto bHandled{ToBaseEntity(pent)->HandleKeyValue(pkvd->szKeyName, pkvd->szValue)};
	if(bHandled)
		pkvd->fHandled = 1;
};

void EntitySave(edict_t *pent, SAVERESTOREDATA *pSaveRestoreData)
{
	//ToBaseEntity(pent)->Save(); // TODO
};

int EntityRestore(edict_t *pent, SAVERESTOREDATA *pSaveRestoreData, int globalentity)
{
	//ToBaseEntity(pent)->Restore(); // TODO
	return 1;
};

void EntitySetAbsPos(edict_t *pent)
{
	//ToBaseEntity(pent)->SetAbsPos(); // TODO
};

void SaveWriteFields(SAVERESTOREDATA *pSaveRestoreData, const char *sName, void *pBaseData, TYPEDESCRIPTION *pFields, int nFieldCount)
{
	// TODO
};

void SaveReadFields(SAVERESTOREDATA *pSaveRestoreData, const char *sName, void *pBaseData, TYPEDESCRIPTION *pFields, int nFieldCount)
{
	// TODO
};

void SaveGlobalState(SAVERESTOREDATA *pSaveRestoreData)
{
	// TODO
};

void RestoreGlobalState(SAVERESTOREDATA *pSaveRestoreData)
{
	// TODO
};

void ResetGlobalState()
{
	// TODO
};

void PM_Init_Game(struct playermove_s *ppmove)
{
	PM_Init(ppmove);
};

void PM_Move_Game(struct playermove_s *ppmove, qboolean server)
{
	PM_Move(ppmove, server);
};

char PM_FindTextureType_Game(const char *name)
{
	return PM_GetTextureType(name);
};

//

void OnFreeEntPrivateData(edict_t *pent)
{
	// TODO
};

void GameShutdown()
{
	gpGame->Shutdown();
};

int ShouldCollide(edict_t *pent, edict_t *pother)
{
	return gpGame->GetRules()->ShouldCollide(*ToBaseEntity(pent), *ToBaseEntity(pother));
};
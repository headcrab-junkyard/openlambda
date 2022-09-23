/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
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
/// @brief client functions

#include "exports.h"
#include "custom.h" // TODO: temp?
#include "progdefs.h"
#include "mathlib.h"
#include "edict.h"

#include <next/engine/IGameClientEventListener.hpp>

#include "Game.hpp"
#include "IGameRules.hpp"
#include "Util.hpp"
#include "BaseEntity.hpp"

void set_suicide_frame(entvars_t *self);


CGameClientEventListener gGameClientEventListener;
CGameClientEventListener /*IGameClientEventListener*/ *gpGameClientEventListener{&gGameClientEventListener}; // TODO: can't use the IGameClientEventListener here as we need to access the OnClientKill method of the CGameClientEventListener class


int ToClientID(edict_t *apEntDict)
{
	return 0; // TODO: get entity id, check that it's > 0 and less than maxplayers count, return it
};

/*
===========
ClientConnect

called when a player connects to a server
============
*/
qboolean ClientConnect(edict_t *pEntity, const char *name, const char *adr, char sRejectReason[128])
{
	return gpGameClientEventListener->OnClientConnect(ToClientID(pEntity), name, adr, sRejectReason);
};

/*
===========
ClientDisconnect

called when a player disconnects from a server
============
*/
void ClientDisconnect(edict_t *self)
{
	gpGameClientEventListener->OnClientDisconnect(ToClientID(self));
};

// called by ClientKill and DeadThink
void respawn(entvars_t *self)
{
/*
	// make a copy of the dead body for appearances sake
	CopyToBodyQue (self);
	
	// set default spawn parms
	SetNewParms (self);
	
	// respawn              
	ClientPutInServer (self);
*/
};

/*
============
ClientKill

Player entered the suicide command
============
*/
void ClientKill(edict_t *self)
{
	gpGameClientEventListener->OnClientKill(ToClientID(self));
};

/*
===========
ClientPutInServer

called each time a player enters a new level
============
*/
void ClientPutInServer(edict_t *client)
{
	gpGameClientEventListener->OnClientPutInServer(ToClientID(client));
};

/*
*/
void ClientCommand(edict_t *pclent)
{
	CCmdArgs CmdArgs;
	// TODO: cmd args
	gpGameClientEventListener->OnClientCommand(ToClientID(pclent), CmdArgs);
};

void ClientUserInfoChanged(edict_t *pclent, char *userinfo)
{
	gpGameClientEventListener->OnClientUserInfoChanged(ToClientID(pclent), userinfo);
};

void ServerActivate(edict_t *edicts, int edictcount, int maxclients)
{
	// TODO
};

void ServerDeactivate()
{
	// TODO
};

/*
================
PlayerPreThink

Called every frame before physics are run
================
*/
void PlayerPreThink(edict_t *self)
{
	auto pBaseEntity{ToBaseEntity(self)};
	
	if(!pBaseEntity)
		return;
	
	auto pBasePlayer{dynamic_cast<CBasePlayer*>(pBaseEntity)};
	
	if(pBasePlayer)
		pBasePlayer->PreThink();
};

/*
================
PlayerPostThink

Called every frame after physics are run
================
*/
void PlayerPostThink(edict_t *self)
{
	auto pBaseEntity{ToBaseEntity(self)};
	
	if(!pBaseEntity)
		return;
	
	auto pBasePlayer{dynamic_cast<CBasePlayer*>(pBaseEntity)};
	
	if(pBasePlayer)
		pBasePlayer->PostThink();
};

void ParmsNewLevel() // TODO: SetNewParms?
{
	// Nothing
};

void ParmsChangeLevel() // TODO: SetChangeParms?
{
	// TODO
};

void StartFrame()
{
	gpGame->Frame();
};

const char *GetGameDescription()
{
	return "Stub (Null)";
};

void Sys_Error_Game(const char *error)
{
	// Nothing
};

void PlayerCustomization(edict_t *pPlayer, customization_t *pCustom)
{
	// TODO
};

// Spectator functions (unused)

// Added Aug11'97 by Zoid <zoid@idsoftware.com>
//
// These functions are called from the server if they exist.
// Note that Spectators only have one think since they movement code doesn't
// track them much.  Impulse commands work as usual, but don't call
// the regular ImpulseCommand handler in weapons.qc since Spectators don't
// have any weapons and things can explode.
//
//   --- Zoid.

/*
===========
SpectatorConnect

called when a spectator connects to a server
============
*/
void SpectatorConnect(edict_t *self)
{
	// UNUSED
/*
	bprint (PRINT_MEDIUM, "Spectator ");
	bprint (PRINT_MEDIUM, self->v.netname);
	bprint (PRINT_MEDIUM, " entered the game\n");

	self->v.goalentity = world; // used for impulse 1 below
*/
};

/*
===========
SpectatorDisconnect

called when a spectator disconnects from a server
============
*/
void SpectatorDisconnect(edict_t *self)
{
	// UNUSED
/*
	bprint (PRINT_MEDIUM, "Spectator ");
	bprint (PRINT_MEDIUM, self->v.netname);
	bprint (PRINT_MEDIUM, " left the game\n");
*/
};

/*
================
SpectatorImpulseCommand

Called by SpectatorThink if the spectator entered an impulse
================
*/
void SpectatorImpulseCommand(edict_t *self)
{
/*
	if (self->v.impulse == 1)
	{
		// teleport the spectator to the next spawn point
		// note that if the spectator is tracking, this doesn't do
		// much
		self->v.goalentity = find(self->v.goalentity, classname, "info_player_deathmatch");
		if (self->v.goalentity == world)
			self->v.goalentity = find(self->v.goalentity, classname, "info_player_deathmatch");
		if (self->v.goalentity != world)
		{
			setorigin(self, self->v.goalentity.origin);
			self->v.angles = self->v.goalentity.angles;
			self->v.fixangle = TRUE;           // turn this way immediately
		};
	};

	self->v.impulse = 0;
*/
};

/*
================
SpectatorThink

Called every frame after physics are run
================
*/
void SpectatorThink(edict_t *self)
{
	// UNUSED

	// self.origin, etc contains spectator position, so you could
	// do some neat stuff here

	if(self->v.impulse)
		SpectatorImpulseCommand(self);
};

void SetupVisibility(edict_t *pViewEntity, edict_t *pClientEnt, byte **pvs, byte **pas){};

int AddToFullPack(struct entity_state_s *state, int e, edict_t *pent, edict_t *host_edict, int hostflags, int player, byte *pSet)
{
	return 0;
};

void CreateBaseline(int player, int entindex, struct entity_state_s *baseline, edict_t *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs){};

void RegisterEncoders(){};

int GetWeaponData(edict_t *player, struct weapon_data_s *data)
{
	return 0;
};

void UpdateClientData(const edict_t *pent, int sendweapons, struct clientdata_s *pcd){};

void CmdStart(const edict_t *player, const struct usercmd_s *cmd, uint random_seed){};

void CmdEnd(const edict_t *player){};

int ConnectionlessPacket(const struct netadr_s *net_from, const char *args, char *response_buffer, int *len)
{
	return 0;
};

int GetHullBounds(int hullnumber, float *mins, float *maxs)
{
	return 0;
};

void CreateInstancedBaselines(){};

int InconsistentFile(const edict_t *player, const char *filename, char *disconnectmsg)
{
	return gpGameClientEventListener->InconsistentFileFound(ToClientID(player), filename, disconnectmsg);
};

int AllowLagCompensation()
{
	return 0;
};
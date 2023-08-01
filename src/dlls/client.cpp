/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2019, 2021-2023 BlackPhrase
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

#include <engine/custom.h> // TODO: temp?
#include <engine/progdefs.h>
#include <common/mathlib.h>
#include <engine/edict.h>

#include <SystemEventListener_Game.hpp>
#include <GameServerEventListener.hpp>
#include <GameClientEventListener.hpp>

#include <BaseGame.hpp>
#include <IGameRules.hpp>
#include <Util.hpp>
#include <BaseEntity.hpp>

CSystemEventListener_Game gSystemEventListener;
ISystemEventListener *gpSystemEventListener{&gSystemEventListener};

CGameServerEventListener gGameServerEventListener(gpGame);
CGameServerEventListener *gpGameServerEventListener{&gGameServerEventListener};

CGameClientEventListener gGameClientEventListener(gpGame);
CGameClientEventListener /*IGameClientEventListener*/ *gpGameClientEventListener{&gGameClientEventListener}; // TODO: can't use the IGameClientEventListener here as we need to access the OnClientKill method of the CGameClientEventListener class

CBasePlayer *ToBasePlayer(edict_t *apEntity)
{
	return static_cast<CBasePlayer*>(apEntity->GetPrivateData());
};

CBasePlayer *ToBasePlayer(CBaseEntity *apEntity)
{
	return dynamic_cast<CBasePlayer*>(apEntity); // dynamic_cast<CBasePlayer*>(CBasePlayer::Instance(pev)); (pev == entvars_t *)
};


int ToClientID(edict_t *apEntDict)
{
	int nID{gpEngine->pfnIndexOfEdict(apEntDict)};
	
	// Check that this is a player entity
	if(nID < 1 || nID > gpGlobals->maxClients)
		return -1;
	
	return nID - 1;
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

/*
================
respawn

called by ClientKill and DeadThink
================
*/
void respawn(entvars_t *self, bool bCopyCorpse)
{
	if(gpGlobals->coop || gpGlobals->deathmatch)
	{
		if(bCopyCorpse)
		(
			// Make a copy of the dead body for appearances sake
			CopyToBodyQue(self);
		};
		
		// set default spawn parms
		//SetNewParms(self);
		
		// Respawn player    
		//ClientPutInServer(self);
		ToBaseEntity(self)->Spawn(); // GetClassPtr((CBasePlayer*)self)->Spawn();
	}
	else
		gpEngine->pfnServerCommand("reload\n"); // Restart the entire server
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

Called each time a player enters a new level
============
*/
void ClientPutInServer(edict_t *client)
{
	gpGameClientEventListener->OnClientPutInServer(ToClientID(client));
};

/*
================
ClientCommand
================
*/
void ClientCommand(edict_t *pclent)
{
	auto sCmdArgs{gpEngine->pfnCmd_Args()};
	CCmdArgs CmdArgs(sCmdArgs);
	gpGameClientEventListener->OnClientCommand(ToClientID(pclent), CmdArgs);
};

/*
================
ClientUserInfoChanged
================
*/
void ClientUserInfoChanged(edict_t *pclent, char *userinfo)
{
	gpGameClientEventListener->OnClientUserInfoChanged(ToClientID(pclent), userinfo);
};

/*
================
ServerActivate
================
*/
void ServerActivate(edict_t *edicts, int edictcount, int maxclients)
{
	gpGameServerEventListener->mvEdicts = edicts; // BP: meh...
	gpGameServerEventListener->OnActivate(edictcount, maxclients);
};

/*
================
ServerDeactivate
================
*/
void ServerDeactivate()
{
	gpGameServerEventListener->OnDeactivate();
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
	
	auto pBasePlayer{ToBasePlayer(pBaseEntity)};
	
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
	
	auto pBasePlayer{ToBasePlayer(pBaseEntity)};
	
	if(pBasePlayer)
		pBasePlayer->PostThink();
};

/*
================
ParmsNewLevel
================
*/
void ParmsNewLevel() // TODO: SetNewParms?
{
	// Nothing, unused
};

/*
================
ParmsChangeLevel
================
*/
void ParmsChangeLevel() // TODO: SetChangeParms?
{
	// TODO: gpGame->OnChangeLevel()?
	
	// Retrieve the pointer to the save data
	auto pSaveData{(SAVERESTOREDATA*)gpGlobals->pSaveData};
	// TODO: gpGame->GetSaveData()?
	
	if(pSaveData)
		pSaveData->connectionCount = BuildChangeList(pSaveData->levelList, MAX_LEVEL_CONNECTIONS);
};

/*
================
StartFrame
================
*/
void StartFrame()
{
	gpGame->Frame();
};

/*
================
GetGameDescription

Returns the descriptive name of the game
================
*/
const char *GetGameDescription()
{
	if(gpGame->GetRules())
		return gpGame->GetRules()->GetGameDescription(); // TODO: gpGame->GetDescription() or GetTitle()?
	return "Untitled game/mod";
};

/*
================
Sys_Error

Engine is going to shut down, allows setting a breakpoint here to catch that occasion
================
*/
void Sys_Error_Game(const char *error)
{
	gpSystemEventListener->OnError(error);
};

/*
================
PlayerCustomization

A new player customization has been registered on the server

NOTE: This only sets the # of frames of the spray can logo animation right now
================
*/
void PlayerCustomization(edict_t *pPlayer, customization_t *pCustom)
{
	auto pBaseEntity{ToBaseEntity(pPlayer)};
	
	//if(!pBaseEntity)
		//return;
	
	auto pBasePlayer{ToBasePlayer(pBaseEntity)};
	
	if(!pBasePlayer)
	{
		ALERT(at_console, "PlayerCustomization: Couldn't get player!\n");
		return;
	};
	
	if(pCustom)
	{
		ALERT(at_console, "PlayerCustomization: NULL customization!\n");
		return;
	};
	
	switch(pCustom->resource.type)
	{
	case t_decal:
		pBasePlayer->SetCustomDecalFrames(pCustom->nUserData2); // Second int is max # of frames
		break;
	case t_sound:
	case t_skin:
	case t_model:
		// Ignore for now
		break;
	default:
		ALERT(at_console, "PlayerCustomization: Unknown customization type!\n");
		break;
	};
};

/*
================
CvarValue
================
*/
void CvarValue(const edict_t *pent, const char *value)
{
	// TODO: how to handle that?
	//gpGameClientEventListener->OnCvarValueReceived(ToClientID(pent), 0, "TODO", value);
};

/*
================
CvarValue2
================
*/
void CvarValue2(const edict_t *pent, int requestid, const char *cvarname, const char *value)
{
	gpGameClientEventListener->OnCvarValueReceived(ToClientID(pent), requestid, cvarname, value);
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

Called when a spectator connects to a server
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

Called when a spectator disconnects from a server
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

/*
================
SetupVisibility

A client can have a separate "view entity" indicating that his/her view should
depend on the origin of that view entity. If that's the case, then pViewEntity will
be non-null and will be used. Otherwise, the current entity's origin is used
Either is offset by the view_ofs to get the eye position
From the eye position, we set up the PVS and PAS to use for filtering network
messages to the client. At this point, we could override the actual PAS or PVS
values, or use a different origin

NOTE: Do not cache the values of PAS and PVS, as they depend on reusable memory in
the engine, they are only good for this one frame
================
*/
void SetupVisibility(edict_t *pViewEntity, edict_t *pClientEnt, byte **pvs, byte **pas)
{
	edict_t *pView{pClientEnt};
	
	// Find the client's PVS
	if(pViewEntity)
		pView = pViewEntity;
	
	if(pClientEnt->v.flag & FL_PROXY)
	{
		*pvs = nullptr; // The spectator proxy sees
		*pas = nullptr; // and hears everything
		return;
	};
	
	idVec3 vOrigin{pView->v.origin + pView->v.view_ofs};
	
	if(pView->v.flags & FL_DUCKING)
		vOrigin += (VEC_HULL_MIN - VEC_DUCK_HULL_MIN);
	
	*pvs = gpEngine->pfnSetPVS((float*)&vOrigin);
	*pas = gpEngine->pfnSetPAS((float*)&vOrigin);
};

/*
================
AddToFullPack

Return 1 if the entity state has been filled in for the ent and the entity will be
propagated to the client, 0 otherwise

state is the server
================
*/
int AddToFullPack(struct entity_state_s *state, int e, edict_t *pent, edict_t *host_edict, int hostflags, int player, byte *pSet)
{
	// TODO
	return 0;
};

/*
================
CreateBaseline

Creates baselines used for network encoding, especially for player data
since players are not spawned until connect time
================
*/
void CreateBaseline(int player, int entindex, struct entity_state_s *baseline, edict_t *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs)
{
	baseline->origin = entity->v.origin;
	baseline->angles = entity->v.angles;
	baseline->frame = entity->v.frame;
	baseline->skin = (short)entity->v.skin;
	
	// Render info
	baseline->rendermode = (byte)entity->v.rendermode;
	baseline->renderamt = (byte)entity->v.renderamt;
	baseline->rendercolor.r = (byte)entity->v.rendercolor.x;
	baseline->rendercolor.g = (byte)entity->v.rendercolor.y;
	baseline->rendercolor.b = (byte)entity->v.rendercolor.z;
	baseline->renderfx = (byte)entity->v.renderfx;
	
	if(player)
	{
		baseline->mins = player_mins;
		baseline->maxs = player_maxs;
		
		baseline->colormap = eindex;
		baseline->modelindex = playermodelindex;
		baseline->friction = 1.0;
		baseline->movetype = MOVETYPE_WALK;
		
		baseline->scale = entity->v.scale;
		baseline->solid = SOLID_SLIDEBOX;
		baseline->framerate = 1.0;
		baseline->gravity = 1.0;
	}
	else
	{
		baseline->mins = entity->v.mins;
		baseline->maxs = entity->v.maxs;
		
		baseline->colormap = 0;
		baseline->modelindex = entity->v.modelindex;
		
		//SV_ModelIndex(pr_strings + entity->v.model);
		
		baseline->movetype = entity->v.movetype;
		
		baseline->scale = entity->v.scale;
		baseline->solid = entity->v.solid;
		baseline->framerate = entity->v.framerate;
		baseline->gravity = entity->v.gravity;
	};
};

/*
================
RegisterEncoders

Allow game module to override network encoding of certain types of entities and tweak values
================
*/
void RegisterEncoders()
{
	gpGame->RegisterEncoders();
};

/*
================
GetWeaponData
================
*/
int GetWeaponData(edict_t *player, weapon_data_t *data)
{
#ifdef OPENLAMBDA_USE_CLIENT_WEAPONS
	int i;
	weapon_data_t *pItem{nullptr};
	entvars_t *self = &player->v;
	CBasePlayer *pPlayer{dynamic_cast<CBasePlayer*>(CBasePlayer::Instance(self))};
	CBaseWeapon *pGun{nullptr};
	
	SItemInfo ItemInfo{};
	
	memset(data, 0, 32 * sizeof(weapon_data_t)); // TODO: 32 -> 64?
	
	if(!pPlayer)
		return 1;
	
	// Go through all of the weapons and make a list of the ones to pack
	for(i = 0; i < MAX_ITEM_TYPES; ++i)
	{
		if(pPlayer->mrgpPlayerItems[i])
		{
			// There's a weapon here. Should I pack it?
			CBaseItem *pPlayerItem{pPlayer->mrgpPlayerItems[i]};
			
			while(pPlayerItem)
			{
				pGun = dynamic_cast<CBaseWeapon*>(pPlayerItem->GetWeaponPtr());
				
				if(pGun && pGun->UseDecrement())
				{
					// Get the ID
					memset(&ItemInfo, 0, sizeof(ItemInfo));
					pGun->GetItemInfo(&ItemInfo);
					
					if(ItemInfo.iID >= 0 && ItemInfo.iID < 32) // TODO: 64?
					{
						pItem = &data[ItemInfo.iID];
						
						pItem->m_iId = ItemInfo.iID;
						pItem->m_iClip = pGun->mnClip;
						
						pItem->m_flTimeWeaponIdle = max(pGun->m_flTimeWeaponIdle, -0.001);
						pItem->m_flNextPrimaryAttack = max(pGun->m_flNextPrimaryAttack, -0.001);
						pItem->m_flNextSecondaryAttack = max(pGun->m_flNextSecondaryAttack, -0.001);
						
						pItem->m_fInReload = pGun->mbInReload;
						pItem->m_fInSpecialReload = pGun->mbInSpecialReload;
						
						pItem->fuser1 = max(pGun->self->fuser1, -0.001);
						pItem->fuser2 = pGun->m_flStartThrow;
						pItem->fuser3 = pGun->m_flReleaseThrow;
						
						pItem->iuser1 = pGun->m_chargeReady;
						pItem->iuser2 = pGun->m_fInAttack;
						pItem->iuser3 = pGun->m_fireState;
						
						//pItem->m_flPumpTime = max(pGun->m_flPumpTime, -0.001);
					};
				};
				
				pPlayerItem = pPlayerItem->mpNext;
			};
		};
	};
#else // if not defined OPENLAMBDA_USE_CLIENT_WEAPONS
	memset(data, 0, 32 * sizeof(weapon_data_t)); // TODO: 32 -> 64?
#endif // OPENLAMBDA_USE_CLIENT_WEAPONS
	return 1;
};

/*
================
UpdateClientData

Data sent to current client only
Engine sets the clientdata struct fields to 0 before calling
================
*/
void UpdateClientData(const edict_t *pent, int sendweapons, struct clientdata_s *pcd)
{
	if(!pent || !pent->pvPrivateData)
		return;
	
	entvars_t *pVars{&pent->v};
	auto pBaseEntity{ToBaseEntity(pent)};
	auto pBasePlayer{ToBasePlayer(pBaseEntity)};
	
	entvars_t *pVarsOrg{nullptr};
	
	// If user is spectating different player in first person, override some vars
	if(pBasePlayer && pBasePlayer->self->iuser1 == OBS_IN_EYE)
	{
		if(pBasePlayer->mhObserverTarget)
		{
			pVarsOrg = pVars;
			pVars = pBasePlayer->mhObserverTarget->self;
			pBasePlayer = dynamic_cast<CBasePlayer*>(CBasePlayer::Instance(pVars));
		};
	};
	
	pcd->flags = pVars->flags;
	pcd->health = pVars->health;
	
	pcd->viewmodel = gpEngine->pfnGetModexInfex(gpEngine->pfnGetString(pVars->viewmodel));
	
	pcd->waterlevel = pVars->waterlevel;
	pcd->watertype = pVars->watertype;
	
	pcd->weapons = pVars->weapons;
	
	// Vectors
	pcd->origin = pVars->origin;
	pcd->velocity = pVars->velocity;
	pcd->view_ofs = pVars->view_ofs;
	pcd->punchangle = pVars->punchangle;
	
	pcd->bInDuck = pVars->bInDuck;
	pcd->flTimeStepSound = pVars->flTimeStepSound;
	pcd->flDuckTime = pVars->flDuckTime;
	pcd->flSwimTime = pVars->flSwimTime;
	pcd->waterjumptime = pVars->teleport_time;
	
	strcpy(pcd->physinfo, gpEngine->pfnGetPhysicsInfo(pent));
	
	pcd->maxspeed = pVars->maxspeed;
	pcd->fov = pVars->fov;
	pcd->weaponanim = pVars->weaponanim;
	
	pcd->pushmsec = pVars->pushmsec;
	
	// Spectator mode
	if(pVarsOrg)
	{
		// Don't use spec vars from the chased player
		pcd->iuser1 = pVarsOrg->iuser1;
		pcd->iuser2 = pVarsOrg->iuser2;
	}
	else
	{
		pcd->iuser1 = pVars->iuser1;
		pcd->iuser2 = pVars->iuser2;
	};
	
#ifdef OPENLAMBDA_USE_CLIENT_WEAPONS
	if(sendweapons)
	{
		if(pBasePlayer)
		{
			pcd->m_flNextAttack = pBasePlayer->mfNextAttack;
			
			pcd->fuser2 = pBasePlayer->mfNextAmmoBurn;
			pcd->fuser3 = pBasePlayer->mfAmmoStartCharge;
			
			pcd->vuser1.x = pBasePlayer->ammo_9mm;
			pcd->vuser1.y = pBasePlayer->ammo_357;
			pcd->vuser1.z = pBasePlayer->ammo_argrens;
			pcd->ammo_nails = pBasePlayer->ammo_bolts;
			pcd->ammo_shells = pBasePlayer->ammo_buckshot;
			pcd->ammo_rockets = pBasePlayer->ammo_rockets;
			pcd->ammo_cells = pBasePlayer->ammo_uranium;
			pcd->vuser2.x = pBasePlayer->ammo_hornets;
			
			if(pBasePlayer->mpActiveItem)
			{
				pBaseWeapon *pGun{reinterpret_cast<CBaseWeapon*>(pBasePlayer->mpActiveItem->GetWeaponPtr())};
				if(pGun && pGun->UseDecrement())
				{
					SItemInfo ItemInfo{};
					memset(&ItemInfo, 0, sizeof(ItemInfo));
					pGun->GetItemInfo(&ItemInfo);
					
					pcd->m_iId = ItemInfo.iId;
					
					pcd->vuser3.z = pGun->mnSecondaryAmmoType;
					pcd->vuser4.x = pGun->mnPrimaryAmmoType;
					
					pcd->vuser4.y = pBasePlayer->m_rgAmmo[pGun->mnPrimaryAmmoType];
					pcd->vuser4.z = pBasePlayer->m_rgAmmo[pGun->mnSecondaryAmmoType];
					
					if(pBasePlayer->mpActiveItem->m_iId == WEAPON_RPG)
					{
						pcd->vuser2.y = ((CRpg*)pBasePlayer->mpActiveItem)->mfSpotActive;
						pcd->vuser2.z = ((CRpg*)pBasePlayer->mpActiveItem)->m_cActiveRockets;
					};
				};
			};
		};
	};
#endif // OPENLAMBDA_USE_CLIENT_WEAPONS
};

/*
================
CmdStart

We're about to run this user cmd for the specified player. We can set up
group info and masking here, for example
This is the time to examine the user cmd for anything extra. This call happens even
if think does not
================
*/
void CmdStart(const edict_t *player, const struct usercmd_s *cmd, uint random_seed)
{
	auto pBaseEntity{ToBaseEntity(player)}; // entvars_t *pev = (entvars_t*)&player->v
	auto pBasePlayer{ToBasePlayer(pBaseEntity)}; // dynamic_cast<CBasePlayer*>(CBasePlayer::Instance(pev);
	
	if(!pBasePlayer)
		return;
	
	if(pBasePlayer->GetGroupInfo() != 0)
		UTIL_SetGroupTrace(pBasePlayer->self->groupinfo, GROUP_OP_AND);
	
	pBasePlayer->SetRandomSeed(random_seed);
};

/*
================
CmdEnd

Each cmd start is exactly matched with a cmd end, clean up any group trace flags
(and such stuff) here
================
*/
void CmdEnd(const edict_t *player)
{
	auto pBaseEntity{ToBaseEntity(player)}; // entvars_t *pev = (entvars_t*)&player->v
	auto pBasePlayer{ToBasePlayer(pBaseEntity)}; // dynamic_cast<CBasePlayer*>(CBasePlayer::Instance(pev);
	
	if(!pBasePlayer)
		return;
	
	if(pBasePlayer->GetGroupInfo() != 0)
		UTIL_UnsetGroupTrace();
};

/*
================
ConnectionlessPacket

Return 1 if the packet is valid. Set len if you want to send a response packet
Incoming, it holds the max size of the response_buffer, so you must zero it out if
you choose not to respond
================
*/
int ConnectionlessPacket(const struct netadr_s *net_from, const char *args, char *response_buffer, int *len)
{
	return gpGameServerEventListener->OnConnectionlessPacketReceived(net_from, args, response_buffer, len);
};

/*
================
GetHullBounds

Engine calls this to enumerate player collision hulls, for prediction
Return 0 if the hullnumber doesn't exist
================
*/
int GetHullBounds(int hullnumber, float *mins, float *maxs)
{
	int nResult{0};
	
	switch(hullnumber)
	{
	case 0: // Normal player hull
		mins = VEC_HULL_MIN;
		maxs = VEC_HULL_MAX;
		nResult = 1;
		break;
	case 1: // Crouched player hull
		mins = VEC_DUCK_HULL_MIN;
		maxs = VEC_DUCK_HULL_MAX;
		nResult = 1;
		break;
	case 2: // Point based hull
		mins = vec3_origin; // TODO: idVec3::Origin?
		maxs = vec3_origin; // TODO: idVec3::Origin?
		nResult = 1;
		break;
	};
	
	return nResult;
};

/*
================
CreateInstancedBaselines

Create pseudo-baselines for items that aren't placed in the map at spawn time,
but which are likely to be created during the play (e.g, grenades, ammo packs,
projectiles, corpses, etc)
================
*/
void CreateInstancedBaselines()
{
	//entity_state_t EntState{};
	
	// Create any additional baselines here for things like grenades, etc
	//auto nResult{gpEngine->pfnInstanceBaseline(pc->pev->classname, &EntState)};
	
	// Destroy objects
	//gpGame->GetWorld->RemoveEntity(pc);
};

/*
================
InconsistentFile

One of the ENGINE_FORCE_UNMODIFIED files failed the consistency check for the specified player
Return 0 to allow the client to continue, 1 to force immediate disconnection (with an optional disconnect message of up to 256 characters)
================
*/
int InconsistentFile(const edict_t *player, const char *filename, char *disconnectmsg)
{
	return gpGameClientEventListener->InconsistentFileFound(ToClientID(player), filename, disconnectmsg);
};

/*
================
AllowLagCompensation
================
*/
int AllowLagCompensation()
{
	return 0;
};
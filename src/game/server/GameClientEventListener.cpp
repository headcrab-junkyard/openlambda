/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-2001 Id Software, Inc.
 * Copyright (C) 2018, 2022 BlackPhrase
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

#include "GameClientEventListener.hpp"

extern edict_t gEntities[MAX_EDICTS];

CGameClientEventListener::CGameClientEventListener() = default;
CGameClientEventListener::~CGameClientEventListener() = default;

/*
===========
ClientConnect

called when a player connects to a server
============
*/
bool CGameClientEventListener::OnClientConnect(int anClientID /*edict_t *self*/, const char *asName, const char *asNetAdr, char asRejectReason[128])
{
	if(!gEntities[anClientID])
		return;
	
	bprint(PRINT_HIGH, gEntities[anClientID]->v.netname);
	bprint(PRINT_HIGH, " entered the game\n");

	// a client connecting during an intermission can cause problems
	//if(intermission_running)
	//GotoNextMap();

	return true;
};

/*
===========
ClientDisconnect

Called when a player disconnects/drops from a server
Will not be called between levels
============
*/
void CGameClientEventListener::OnClientDisconnect(int anClientID /*edict_t *self*/)
{
	if(!gEntities[anClientID])
		return;
	
	// let everyone else know
	bprint(PRINT_HIGH, gEntities[anClientID]->v.netname);
	bprint(PRINT_HIGH, " left the game");
	
	gpEngine->pfnEmitSound(gEntities[anClientID], CHAN_BODY, "player/tornoff2.wav", 1, ATTN_NONE);
	
	set_suicide_frame(gEntities[anClientID]->v);
};

/*
===========
ClientPutInServer

called each time a player enters a new level
============
*/
void CGameClientEventListener::OnClientPutInServer(int anClientID /*edict_t *client*/)
{
	if(!gEntities[anClientID])
		return;
	
	CBasePlayer *pPlayer{ToBasePlayer(gEntities[anClientID])};
	
	string s;

	// clear entity values
	pPlayer->SetClassName("player");
	pPlayer->SetHealth(100);
	pPlayer->SetMaxHealth(100);
	pPlayer->v.deadflag = DEAD_NO;
	pPlayer->v.takedamage = DAMAGE_AIM;
	pPlayer->SetSolidType(SOLID_SLIDEBOX);
	pPlayer->SetMoveType(MOVETYPE_WALK);
	pPlayer->v.show_hostile = 0;
	pPlayer->SetFlags(FL_CLIENT);
	pPlayer->v.dmg = 2; // initial water damage
	
	pPlayer->v.air_finished = /*level.*/time + 12;
	pPlayer->v.super_damage_finished = 0;
	pPlayer->v.radsuit_finished = 0;
	pPlayer->v.invisible_finished = 0;
	pPlayer->v.invincible_finished = 0;
	
	pPlayer->SetEffects(0);
	pPlayer->v.invincible_time = 0;

	DecodeLevelParms();

	pPlayer->SetCurrentAmmo();

	pPlayer->v.attack_finished = time;
	
	pPlayer->v.th_pain = CPlayer::Pain;
	pPlayer->v.th_die = CPlayer::Die;
	
	// paustime is set by teleporters to keep the player from moving a while
	pPlayer->v.pausetime = 0;

	// oh, this is a hack!
	pPlayer->SetModel("models/eyes.mdl");
	modelindex_eyes = pPlayer->v.modelindex;

	pPlayer->SetModel("models/player.mdl");
	modelindex_player = pPlayer->v.modelindex;

	pPlayer->SetSize(VEC_HULL_MIN, VEC_HULL_MAX);

	pPlayer->v.view_ofs = idVec3(0, 0, 22);

	// Mod - Xian (May.20.97)
	// Bug where player would have velocity from their last kill

	pPlayer->SetVelocity(0.0f, 0.0f, 0.0f);

	player_stand1();

	gpEngine->pfnMakeVectors(pPlayer->v.angles);
	spawn_tfog(pPlayer->GetOrigin() + gpGlobals->v_forward * 20);
	
	spawn_tdeath(pPlayer->v.origin, pPlayer->v);
	
	mpGame->GetRules()->OnClientPutInServer(anClientID);
};

/*
=================
ClientCommand
=================
*/
void CGameClientEventListener::OnClientCommand(int anClientID, const ICmdArgs &aArgs)
{
	if(!gEntities[anClientID])
		return;
	
	// not fully in game yet
	//if(!gEntities[anClientID]->client)
		//return;

	char *cmd{aArgs.GetByIndex(0)};

	if (Q_stricmp (cmd, "players") == 0)
	{
		Cmd_Players_f(gEntities[anClientID]);
		return;
	};
	
	if (Q_stricmp (cmd, "say") == 0)
	{
		Cmd_Say_f (gEntities[anClientID], false, false);
		return;
	};
	
	if (Q_stricmp (cmd, "say_team") == 0)
	{
		Cmd_Say_f (gEntities[anClientID], true, false);
		return;
	};
	
	if (Q_stricmp (cmd, "score") == 0)
	{
		Cmd_Score_f (gEntities[anClientID]);
		return;
	};
	
	if (Q_stricmp (cmd, "help") == 0)
	{
		Cmd_Help_f (gEntities[anClientID]);
		return;
	};

	if (level.intermissiontime)
		return;

	if (Q_stricmp (cmd, "use") == 0)
		Cmd_Use_f (gEntities[anClientID]);
	else if (Q_stricmp (cmd, "drop") == 0)
		Cmd_Drop_f (gEntities[anClientID]);
	else if (Q_stricmp (cmd, "give") == 0)
		Cmd_Give_f (gEntities[anClientID]);
	else if (Q_stricmp (cmd, "god") == 0)
		Cmd_God_f (gEntities[anClientID]);
	else if (Q_stricmp (cmd, "notarget") == 0)
		Cmd_Notarget_f (gEntities[anClientID]);
	else if (Q_stricmp (cmd, "noclip") == 0)
		Cmd_Noclip_f (gEntities[anClientID]);
	else if (Q_stricmp (cmd, "inven") == 0)
		Cmd_Inven_f (gEntities[anClientID]);
	else if (Q_stricmp (cmd, "invnext") == 0)
		SelectNextItem (gEntities[anClientID], -1);
	else if (Q_stricmp (cmd, "invprev") == 0)
		SelectPrevItem (gEntities[anClientID], -1);
	else if (Q_stricmp (cmd, "invnextw") == 0)
		SelectNextItem (gEntities[anClientID], IT_WEAPON);
	else if (Q_stricmp (cmd, "invprevw") == 0)
		SelectPrevItem (gEntities[anClientID], IT_WEAPON);
	else if (Q_stricmp (cmd, "invnextp") == 0)
		SelectNextItem (gEntities[anClientID], IT_POWERUP);
	else if (Q_stricmp (cmd, "invprevp") == 0)
		SelectPrevItem (gEntities[anClientID], IT_POWERUP);
	else if (Q_stricmp (cmd, "invuse") == 0)
		Cmd_InvUse_f (gEntities[anClientID]);
	else if (Q_stricmp (cmd, "invdrop") == 0)
		Cmd_InvDrop_f (gEntities[anClientID]);
	else if (Q_stricmp (cmd, "weapprev") == 0)
		Cmd_WeapPrev_f (gEntities[anClientID]);
	else if (Q_stricmp (cmd, "weapnext") == 0)
		Cmd_WeapNext_f (gEntities[anClientID]);
	else if (Q_stricmp (cmd, "weaplast") == 0)
		Cmd_WeapLast_f (gEntities[anClientID]);
	else if (Q_stricmp (cmd, "kill") == 0)
		Cmd_Kill_f (gEntities[anClientID]);
	else if (Q_stricmp (cmd, "putaway") == 0)
		Cmd_PutAway_f (gEntities[anClientID]);
	else if (Q_stricmp (cmd, "wave") == 0)
		Cmd_Wave_f (gEntities[anClientID]);
	else if (Q_stricmp(cmd, "playerlist") == 0)
		Cmd_PlayerList_f(gEntities[anClientID]);
	else if(Q_stricmp(cmd, "kill") == 0)
		OnClientKill(anClientID);
	else	// anything that doesn't match a command will be a chat
		Cmd_Say_f (gEntities[anClientID], false, true);
};

void CGameClientEventListener::OnClientUserInfoChanged(int anClientID, char *asUserInfo)
{
	if(!gEntities[anClientID])
		return;
	
	// TODO
};

int CGameClientEventListener::OnInconsistentFileFound(int anClientID, const char *asFileName, char *asDisconnectMsg)
{
	return 0;
};

/*
============
ClientKill

Player entered the suicide command
============
*/
void CGameClientEventListener::OnClientKill(int anClientID /*edict_t *self*/) // TODO: -> OnClientCommand?
{
	if(!gEntities[anClientID])
		return;
	
	bprint(PRINT_MEDIUM, gEntities[anClientID]->v.netname);
	bprint(PRINT_MEDIUM, " suicides\n");

	set_suicide_frame(gEntities[anClientID]->v);

	//gEntities[anClientID]->v.modelindex = modelindex_player;
	//logfrag(gEntities[anClientID], gEntities[anClientID]);
	//gEntities[anClientID]->v.frags -= 2; // extra penalty
	//respawn(&gEntities[anClientID]->v);
};
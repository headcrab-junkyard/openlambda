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
#include "BaseGame.hpp"

extern edict_t *gpEntities;

void set_suicide_frame(entvars_t *self);

/*
===========
ClientConnect

called when a player connects to a server
============
*/
bool CGameClientEventListener::OnClientConnect(int anClientID /*edict_t *self*/, const char *asName, const char *asNetAdr, char asRejectReason[128])
{
	if(!gpEntities[anClientID])
		return false;
	
	return mpGame->GetRules()->HandleClientConnect(gpEntities[anClientID] /*ToBaseEntity(pEntity)*/, asName, asNetAdr, asRejectReason); // TODO: ClientConnected?

	//BroadcastPrint(PRINT_HIGH, "%s entered the game\n", gpEntities[anClientID]->v.netname); // TODO: unused

	// a client connecting during an intermission can cause problems
	//if(intermission_running)
		//ExitIntermission(); // TODO: was GotoNextMap
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
	if(gbGameOver)
		return;
	
	if(!gpEntities[anClientID])
		return;
	
	auto pClientEnt{ToBaseEntity(self)};
	
	if(!pClientEnt)
		return;
	
	char sText[256]{};
	
	if(gpEntities[anClientID]->v.netname)
		_snprintf(sText, sizeof(sText), "- %s has left the game\n", gpEngine->pfnGetString(gpEntities[anClientID]->v.netname));
	sText[sizeof(sText) - 1] = 0;
	
	//bprint(PRINT_HIGH, );
	
	// Let everyone else know
	gpEngine->pfnMessageBegin(MSG_ALL, gMsgSayText, nullptr);
	gpEngine->pfnWriteByte(gpEngine->pfnIndexOfEdict(pClientEnt));
	gpEngine->pfnWriteString(sText);
	gpEngine->pfnMessageEnd();
	
	// TODO: BroadcastPrint()?
	
	// TODO
	//CSound *pSound{};
	//if(pSound)
		//pSound->Reset();
	
	// Since the edict doesn't get deleted, fix it so it doesn't interfere
	pClientEnt->SetDamageable(CBaseEntity::Damageable::No); // Don't attract autoaim
	pClientEnt->SetSolidity(CBaseEntity::Solidity::None); // Make non-solid
	pClientEnt->SetOrigin(pClientEnt->GetOrigin());
	
	mpGame->GetRules()->HandleClientDisconnect(pClientEnt);
/*
	gpEngine->pfnEmitSound(gpEntities[anClientID], CHAN_BODY, "player/tornoff2.wav", 1, ATTN_NONE);
	
	set_suicide_frame(gpEntities[anClientID]->v);
*/
};

/*
===========
ClientPutInServer

called each time a player enters a new level
============
*/
void CGameClientEventListener::OnClientPutInServer(int anClientID /*edict_t *client*/)
{
	if(!gpEntities[anClientID])
		return;
	
	auto pClientEnt{ToBaseEntity(gpEntities[anClientID])};
	
	if(!pClientEnt)
		return;
	
	auto pBasePlayer{ToBasePlayer(pClientEnt)};
	
	if(!pBasePlayer)
		return;
	
	pBasePlayer->SetCustomDecalFrames(-1); // Assume none
	
	// Allocate a class for ent vars and call its spawn functionality
	pBasePlayer->Spawn();
	
	// Reset interpolation during first frame
	pBasePlayer->AddEffects(EF_NOINTERP);
	
	pBasePlayer->self->iuser1 = 0; // Disable any spectator modes
	pBasePlayer->self->iuser2 = 0;
	
	//mpGame->GetRules()->OnClientPutInServer(anClientID);
};

/*
=================
ClientCommand
=================
*/
void CGameClientEventListener::OnClientCommand(int anClientID, const ICmdArgs &aArgs)
{
	if(!gpEntities[anClientID])
		return;
	
	auto pClientEnt{ToBaseEntity(pclent)};
	
	if(!pClientEnt)
		return;
	
	auto pBasePlayer{ToBasePlayer(pClientEnt)}; // GetClassPtr((CBasePlayer*)self)
	auto sCmd{aArgs.GetByIndex(0)};
	
	// TODO
	//else if(!strcmp(sCmd, "lastinv"))
		//pBasePlayer->SelectLastItem();
	
	if(Q_stricmp(sCmd, "say") == 0)
	{
		//Cmd_Say_f(gpEntities[anClientID], false, false);
		Host_Say(pClientEnt, 0);
		return;
	}
	else if(Q_stricmp(sCmd, "say_team") == 0)
	{
		//Cmd_Say_f(gpEntities[anClientID], true, false);
		Host_Say(pClientEnt, 1);
		return;
	}
	else if(!strcmp(sCmd, "fullupdate"))
		pBasePlayer->ForceClientDllUpdate();
	else if(!strcmp(sCmd, "give")) // else if (Q_stricmp (cmd, "give") == 0)
	{
		//Cmd_Give_f(gpEntities[anClientID]);
		
		if(gbWeaponCheat != false)
		{
			// Make a copy of the classname
			int nItemName = gpEngine->pfnAllocString(aArgs.GetByIndex(1));
			
			pBasePlayer->GiveNamedItem(gpEngine->pfnGetString(nItemName));
		};
	}
	else if(!strcmp(sCmd, "drop")) // else if (Q_stricmp (cmd, "drop") == 0)
		// Player is dropping an item
		//Cmd_Drop_f(gpEntities[anClientID]);
		pBasePlayer->DropItem(aArgs.GetByIndex(1)); // DropPlayerItem
	else if(!strcmp(sCmd, "fov"))
	{
		if(gbWeaponCheat && aArgs.GetCount() > 1)
			pBasePlayer->mnFOV = atoi();
		else
			ClientPrintf();
	}
	else if(!strcmp(sCmd, "use")) // if (Q_stricmp (cmd, "use") == 0)
		//Cmd_Use_f(gpEntities[anClientID]);
		pBasePlayer->SelectItem(aArgs.GetByIndex(1));
	else if(!strcmp(sCmd, "weapon_"))
		;
	else if(!strcmp(sCmd, "spectate"))
		if()
		{
		}
		else
			ClientPrint();
	else if(!strcmp(sCmd, "specmode")) // New spectator mode
		if(pBasePlayer->IsObserver())
			pBasePlayer->Observer_SetMode(atoi(aArgs.GetByIndex(1)));
	else if(!strcmp(sCmd, "follownext")) // Follow next player
		if(pBasePlayer->IsObserver())
			pBasePlayer->Observer_FindNextPlayer(atoi(aArgs.GetByIndex(1)) ? true : false);
	else if(!Q_stricmp(sCmd, "kill"))
		OnClientKill(anClientID);
	else if(gpGame->GetRules()->HandleClientCommand(pBasePlayer, sCmd))
	{
		// MenuSelect returns true only if the command is properly handled, so don't print a warninsg
	}
	else
	{
		char sLimitedCmd[128]{};
		
		// Check the length of the command (prevent crash)
		// Max total length is 192 and we're adding a string below ("Unknown command: %s\n")
		strncpy(sLimitedCmd, sCmd, sizeof(sLimitedCmd) - 1);
		sLimitedCmd[127] = '\0';
		
		// Tell the user they entered an unknown command
		ClientPrint(anClientID, HUD_PRINTCONSOLE, va("Unknown command: %s\n", sLimitedCmd));
	};
	
	// TODO

/*
	// not fully in game yet
	//if(!gpEntities[anClientID]->client)
		//return;

	char *cmd{aArgs.GetByIndex(0)};

	if(Q_stricmp(cmd, "players") == 0)
	{
		Cmd_Players_f(gpEntities[anClientID]);
		return;
	};
	
	if(Q_stricmp(cmd, "score") == 0)
	{
		Cmd_Score_f(gpEntities[anClientID]);
		return;
	};
	
	if(Q_stricmp(cmd, "help") == 0)
	{
		Cmd_Help_f(gpEntities[anClientID]);
		return;
	};

	if(level.intermissiontime)
		return;
		
	else if (Q_stricmp (cmd, "god") == 0)
		Cmd_God_f (gpEntities[anClientID]);
	else if (Q_stricmp (cmd, "notarget") == 0)
		Cmd_Notarget_f (gpEntities[anClientID]);
	else if (Q_stricmp (cmd, "noclip") == 0)
		Cmd_Noclip_f (gpEntities[anClientID]);
	else if (Q_stricmp (cmd, "inven") == 0)
		Cmd_Inven_f (gpEntities[anClientID]);
	else if (Q_stricmp (cmd, "invnext") == 0)
		SelectNextItem (gpEntities[anClientID], -1);
	else if (Q_stricmp (cmd, "invprev") == 0)
		SelectPrevItem (gpEntities[anClientID], -1);
	else if (Q_stricmp (cmd, "invnextw") == 0)
		SelectNextItem (gpEntities[anClientID], IT_WEAPON);
	else if (Q_stricmp (cmd, "invprevw") == 0)
		SelectPrevItem (gpEntities[anClientID], IT_WEAPON);
	else if (Q_stricmp (cmd, "invnextp") == 0)
		SelectNextItem (gpEntities[anClientID], IT_POWERUP);
	else if (Q_stricmp (cmd, "invprevp") == 0)
		SelectPrevItem (gpEntities[anClientID], IT_POWERUP);
	else if (Q_stricmp (cmd, "invuse") == 0)
		Cmd_InvUse_f (gpEntities[anClientID]);
	else if (Q_stricmp (cmd, "invdrop") == 0)
		Cmd_InvDrop_f (gpEntities[anClientID]);
	else if (Q_stricmp (cmd, "weapprev") == 0)
		Cmd_WeapPrev_f (gpEntities[anClientID]);
	else if (Q_stricmp (cmd, "weapnext") == 0)
		Cmd_WeapNext_f (gpEntities[anClientID]);
	else if (Q_stricmp (cmd, "weaplast") == 0)
		Cmd_WeapLast_f (gpEntities[anClientID]);
	else if (Q_stricmp (cmd, "kill") == 0)
		Cmd_Kill_f (gpEntities[anClientID]);
	else if (Q_stricmp (cmd, "putaway") == 0)
		Cmd_PutAway_f (gpEntities[anClientID]);
	else if (Q_stricmp (cmd, "wave") == 0)
		Cmd_Wave_f (gpEntities[anClientID]);
	else if (Q_stricmp(cmd, "playerlist") == 0)
		Cmd_PlayerList_f(gpEntities[anClientID]);
	else	// anything that doesn't match a command will be a chat
		Cmd_Say_f (gpEntities[anClientID], false, true);
*/
};

void CGameClientEventListener::OnClientUserInfoChanged(int anClientID, char *asUserInfo)
{
	// Is the client spawned yet?
	if(!gpEntities[anClientID])
		return;
	
	// Message everyone if someone changes their name and it isn't the first time
	// (changing no name to current name)
	if()
	{
		char sName[256]{};
		char *pName{};
		sName[sizeof(sName) - 1] = '\0';
		
		//
		for()
		{
			// Replace it with a space
			if(*p == '%')
				*p = ' ';
		};
		
		// Set the name
		gpEngine->pfnSetClientKeyValue();
		
		if(gpGlobals->maxClients > 1)
		{
			char sText[256]{};
			sprintf(sText, "", gpEngine->pfnInfoKeyValue());
			
			gpEngine->pfnMessageBegin();
			gpEngine->pfnMessageEnd();
		};
		
		// Team match?
		if(g_teamplay)
		{
		}
		else
		{
		};
	};
	
	mpGame->GetRules()->ClientUserInfoChanged(anClientID, asUserInfo);
};

int CGameClientEventListener::OnInconsistentFileFound(int anClientID, const char *asFileName, char *asDisconnectMsg)
{
	// Check if player should be kicked
	if(gpEngine->pfnGetCvarFloat("mp_consistency") == 1)
	{
		// Let him know why
		sprintf(asDisconnectMsg, "Server is enforcing file consistency for %s\n", asFileName);
		
		// Kick now with specified disconnect message
		return 1;
	};
	
	// Server doesn't care
	return 0;
};

void CGameClientEventListener::OnCvarValueReceived(int anClientID, int anRequestID, const char *asCvarName, const char *asCvarValue)
{
	// TODO
};

/*
============
ClientKill

Player entered the suicide command
============
*/
void CGameClientEventListener::OnClientKill(int anClientID /*edict_t *self*/) // TODO: -> OnClientCommand?
{
	if(!gpEntities[anClientID])
		return;
	
	//auto pBaseEntity{ToBaseEntity(self)};
	auto pBaseEntity{gpEntities[anClientID]}; // TODO: mpGame->GetWorld()->GetEntity(anClientID)?
	
	if(!pBaseEntity)
		return;
	
	auto pBasePlayer{ToBasePlayer(pBaseEntity)}; // TODO: = (CBasePlayer*)CBasePlayer::Instance(self);?
	
	if(!pBasePlayer)
		return;
	
	// Prevent suiciding too often
	if(pBasePlayer->mfNextSuicideTime > gpGlobals->time)
		return;
	
	// Don't let them suicide for 5 seconds after suiciding
	pBasePlayer->mfNextSuicideTime = gpGlobals->time + 1;
	
	// Have the player kill themself
	pBasePlayer->SetHealth(0);
	pBasePlayer->Killed(pBaseEntity, CBaseEntity::GibType::Never); // TODO: last inflictor
	
	// TODO: unused
/*
	bprint(PRINT_MEDIUM, gpEntities[anClientID]->v.netname);
	bprint(PRINT_MEDIUM, " suicides\n");
	
	set_suicide_frame(gpEntities[anClientID]->v);
*/
	
	// NOTE: commented out in original
	//gpEntities[anClientID]->v.modelindex = modelindex_player;
	//logfrag(gpEntities[anClientID], gpEntities[anClientID]); // TODO: remove?
	//gpEntities[anClientID]->v.frags -= 2; // extra penalty
	//respawn(&gpEntities[anClientID]->v);
};
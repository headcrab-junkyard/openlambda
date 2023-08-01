/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2022-2023 BlackPhrase
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

#include "GameServerEventListener.hpp"

bool mbServerActive{false}; // g_serveractive

void LinkUserMessages();

// TODO: the content of this method was part of the server spawn functionality so maybe it should be placed inside of OnSpawn method instead
void CGameServerEventListener::OnSpawn(int anMaxEntites, int anMaxClients) // TODO: OnChangeLevel?
{
	// TODO: non-qw
	// Allocate server memory
	sv.max_edicts = anMaxEntites;

	// Allocate edicts
	sv.edicts = (edict_t*)Hunk_AllocName(sv.max_edicts * sizeof(edict_t), "edicts");
	
	edict_t *ent{nullptr};
	
	// Leave slots at start for clients only
	sv.num_edicts = anMaxClients + 1;
	for(int i = 0; i < anMaxClients; i++)
	{
		ent = EDICT_NUM(i + 1);
		mpGame->GetPlayer(i)->SetEntity(ent);
		//ZOID - make sure we update frags right
		//svs.clients[i].old_frags = 0; // TODO: qw
	};
	
	//
	// Load/spawn the rest of the entities on the map
	//
	
	ent = EDICT_NUM(0);
	Q_memset(&ent->v, 0, sizeof(entvars_t)); // TODO: non-qw
	ent->free = false;
	ent->v.model = sv.worldmodel->name - pr_strings; // TODO: PR_SetString
	// TODO: non-qw
	ent->v.modelindex = 1; // world model
	ent->v.solid = SOLID_BSP;
	ent->v.movetype = MOVETYPE_PUSH;
	//
	
	// Load and spawn all other entities
	ED_LoadFromFile(sv.worldmodel->entities);
};

void CGameServerEventListener::OnActivate(int anMaxEntites, int anMaxClients)
{
	int i;
	CBaseEntity *pClass{nullptr};
	
	// Every call to ServerActivate should be matched by a call to ServerDeactivate
	mbServerActive = true;
	
	// NOTE: mvEdicts == pEdictList
	
	// Clients have not been initialized yet
	for(i = 0; i < anMaxEntites; ++i)
	{
		if(mvEdicts[i].free)
			continue;
		
		// Clients aren't necessarily initialized until ClientPutInServer call
		if(i < anMaxClients || !mvEdicts[i].pvPrivateData)
			continue;
		
		pClass = CBaseEntity::Instance(&mvEdicts[i]);
		
		// Activate this entity if it's got a class and isn't dormant
		if(pClass && !pClass->IsDormant())
			pClass->Activate();
		else
			ALERT(at_console, "Can't instance %s\n", gpEngine->pfnGetString(mvEdicts[i].v.classname));
	};
	
	// Link user messages here to make sure first client can get them
	LinkUserMessages();
};

void CGameServerEventListener::OnDeactivate()
{
	// It's possible that the engine will call this more times than is necessary
	// Therefore, run it only one time for each call to ServerActivate
	if(!mbServerActive)
		return;
	
	mbServerActive = false;
	
	// NOTE: Perform any shutdown operations here
};

//HandleConnectionlessPacket
int CGameServerEventListener::OnConnectionlessPacketReceived(const struct &aNetAdrFrom, const char *asArgs, char *apResponseBuffer, int *anResponseBufferSize)
{
	*anResponseBufferSize = 0;
	return 0;
};
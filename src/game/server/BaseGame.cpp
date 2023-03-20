/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2019-2023 BlackPhrase
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

#include "BaseGame.hpp"

#include "next/filesystem/IFileSystem.hpp"
#include "next/engine/IVoiceServer.hpp"
//#include "next/physics/IPhysicsSystem.hpp"
//#include "next/scriptsystem/IScriptSystem.hpp"

bool CBaseGame::Init(CreateInterfaceFn afnEngineFactory)
{
	mpFileSystem = reinterpret_cast<IFileSystem*>(afnEngineFactory(OGS_FILESYSTEM_INTERFACE_VERSION, nullptr));
	mpVoiceServer = reinterpret_cast<IVoiceServer*>(afnEngineFactory(OGS_VOICESERVER_INTERFACE_VERSION, nullptr));
	//mpPhysics = reinterpret_cast<IPhysicsSystem*>(afnEngineFactory(OGS_PHYSICSSYSTEM_INTERFACE_VERSION, nullptr));
	//mpScript = reinterpret_cast<IScriptSystem*>(afnEngineFactory(OGS_SCRIPTSYSTEM_INTERFACE_VERSION, nullptr));
	
	if(!mpFileSystem)
		return false;
	
	if(!mpVoiceServer)
		return false;
	
	//if(!mpPhysics)
		//return false;
	
	RegisterEvents();
	//if(!mpScript)
		//return false;
	
	
	// Manually init some parts for new engine as legacy engine inits them individually
	// TODO: if not manual initialization?
	if constexpr(TargetAPI == TargetEngineAPI::Next)
	{
		RegisterEncoders();
	
		InitPlayerMovement(); // TODO: move sv pmove struct instance here
	};
	
	return true;
};

void CBaseGame::Shutdown()
{
	// NOTE: free resources here
};

void CBaseGame::Update()
{
	if(mpRules)
		mpRules->Think();
	
	//
	// cvars checked each frame
	//
	//timelimit = cvar("timelimit") * 60;
	//fraglimit = cvar("fraglimit");
	gpGlobals->teamplay = teamplay.value; // teamplay = cvar("teamplay");
	//deathmatch = cvar("deathmatch");

	framecount++;
};

bool CBaseGame::HandleClientMessage(int anClientID, const INetMsg /*IReadBuffer*/ &net_message)
{
	client_t *cl = TODO_GetClientByIndex(anClientID);
	edict_t *sv_player = TODO_GetPlayerEntity(cl);
	
	INetMsgHandler *handler{NetMsgHandlerManager->GetHandlerForMsg(net_message.GetID())};
	
	if(!handler)
		return false;
	
	/*
	switch(net_message.GetID())
	{
	default:
		return false;

	case clc_move:
		CCLC_MoveMsgHandler handler;
		break;

	case clc_delta:
		CCLC_DeltaMsgHandler handler;
		break;

	case clc_resourcelist:
		CCLC_ResourceListMsgHandler handler;
		break;

	case clc_tmove:
		CCLC_TeleportMoveMsgHandler handler;
		break;

	case clc_voicedata:
		CCLC_VoiceDataMsgHandler handler;
		break;

	case clc_hltv:
		CCLC_HLTVMsgHandler handler;
		break;

	case clc_cvarvalue:
		CCLC_CVarValueMsgHandler handler;
		break;
	};
	*/
	
	return handler->Handle(cl, net_message);
};

};
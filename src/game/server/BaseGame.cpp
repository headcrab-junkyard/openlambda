/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2019-2022 BlackPhrase
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

#include "Game.hpp"

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

bool CBaseGame::HandleClientMessage(int anClientID, int anMsgID, INetMsg /*IReadBuffer*/ *net_message)
{
	client_t *cl = TODO_GetClientByIndex(anClientID);
	edict_t *sv_player = TODO_GetPlayerEntity(cl);
	
	switch(anMsgID)
	{
	default:
		return false;

	case clc_move:
		SV_ReadClientMove(cl, &cl->lastcmd); // TODO
		// TODO: qw
		/*
		if(move_issued)
			return; // someone is trying to cheat...

		move_issued = true;

		//int checksumIndex = net_message->GetReadCount(); // TODO
		byte checksum = (byte)net_message->ReadByte();

		// read loss percentage
		//cl->lossage = net_message->ReadByte(); // TODO

		//net_message->ReadDeltaUsercmd(&nullcmd, &oldest); // TODO
		net_message->ReadDeltaUsercmd(&oldest, &oldcmd);
		net_message->ReadDeltaUsercmd(&oldcmd, &newcmd);
		*/
		
		if(!cl->spawned)
			break;
		
		/*
		// if the checksum fails, ignore the rest of the packet
		// TODO
		//byte calculatedChecksum = COM_BlockSequenceCRCByte(
			//net_message->data + checksumIndex + 1,
			//net_message->GetReadCount() - checksumIndex - 1,
			//seq_hash);

		if(calculatedChecksum != checksum)
		{
			mpSystem->DevPrintf("Failed command checksum for %s(%d) (%d != %d)\n", 
				cl->GetName(), cl->GetNetChan()->incoming_sequence, checksum, calculatedChecksum);
			return;
		};
		*/

		if(!sv.paused)
		{
			SV_PreRunCmd(cl->edict, &cl->lastcmd, 0); // TODO: get random seed

			if(net_drop < 20)
			{
				while(net_drop > 2)
				{
					SV_RunCmd(&cl->lastcmd);
					net_drop--;
				};
				if(net_drop > 1)
					SV_RunCmd(&oldest);
				if(net_drop > 0)
					SV_RunCmd(&oldcmd);
			};
			SV_RunCmd(&newcmd);

			SV_PostRunCmd();
		};

		cl->lastcmd = newcmd;
		cl->lastcmd.buttons = 0; // avoid multiple fires on lag
		break;

	case clc_stringcmd:
		SV_ParseStringCommand(cl);
		//char *s = net_message->ReadString();
		//SV_ExecuteUserCommand(s); // TODO
		break;

	case clc_delta:
		SV_ParseDelta(cl);
		//cl->delta_sequence = net_message->ReadByte();
		break;

	case clc_resourcelist:
		SV_ParseResourceList(cl);
		//SV_NextUpload();
		break;

	case clc_tmove:
		// only allowed by spectators
		//if(cl->spectator) // TODO
		{
			vec3_t o;
			
			o[0] = net_message->ReadCoord();
			o[1] = net_message->ReadCoord();
			o[2] = net_message->ReadCoord();
			
			VectorCopy(o, sv_player->v.origin);
			SV_LinkEdict(sv_player, false);
		};
		break;

	case clc_fileconsistency:
		SV_ParseConsistencyResponse(cl);
		break;

	case clc_voicedata:
		SV_ParseVoiceData(cl);
		break;

	case clc_hltv:
		SV_ParseHLTV(cl);
		break;

	case clc_cvarvalue:
		SV_ParseCvarValueResponse(cl);
		break;

	case clc_cvarvalue2:
		SV_ParseCvarValueResponseEx(cl);
		break;
	};
	
	return true;
};

};
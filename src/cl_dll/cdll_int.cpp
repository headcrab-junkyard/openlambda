/*
 * This file is part of OpenLambda Project
 * Copyright (C) 2018-2021, 2023 BlackPhrase
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

#include <common/const.h>
#include "engine.h"
#include <tier1/interface.h>
#include "input.h"

#include <next/game/client/IClientGame.hpp>

extern "C"
{
#include <pm_shared/pm_shared.h>
};

extern IClientGame *gpClientGame;

cl_enginefunc_t *gpEngine{nullptr};

IBaseInterface *EngineFactoryLocal(const char *name, int *retval);

// TODO
/*
int UserMsgHook_Test(const char *sName, int nSize, void *pBuf)
{
	return 0;
};
*/

int Initialize(cl_enginefunc_t *apEngFuncs, int anVersion)
{
	if(anVersion != CLDLL_INTERFACE_VERSION)
		return 0;
	
	gpEngine = apEngFuncs;
	
	if(!gpClientGame->Init(EngineFactoryLocal))
		return 0;
	//EV_HookEvents();
	//CL_LoadParticleMan();
	
	//gpEngine->pfnHookUserMsg("Test", UserMsgHook_Test);
	
	// Get tracker interface (if any)
	return 1;
};

void /*CL_DLLEXPORT*/ HUD_Init()
{
	//RecClHudInit();
	
	//InitInput(); // TODO: do we need to call that multiple times?
	//gHUD.Init(); // TODO: do we need to call that multiple times?
	Scheme_Init();
};

int /*CL_DLLEXPORT*/ HUD_VidInit()
{
	//RecClHudVidInit();
	
	gHUD.VidInit();
	//VGui_Startup(); // TODO: do we need to call that multiple times?
	return 1;
};

void /*CL_DLLEXPORT*/ HUD_Shutdown()
{
	gpClientGame->Shutdown();
};

int /*CL_DLLEXPORT*/ HUD_Redraw(float time, int intermission)
{
	//RecClHudRedraw(time, intermission);
	
	gHUD.Redraw(time, intermission);
	return 1;
};

int /*CL_DLLEXPORT*/ HUD_UpdateClientData(struct clientdata_s *pData, float fTime)
{
	//RecClUpdateClientData(pData, fTime);
	
	IN_Commands();
	return gHUD.UpdateClientData(pData, fTime);
};

void /*CL_DLLEXPORT*/ HUD_Reset()
{
	//RecClHudReset();
	
	gHUD.VidInit();
};

// HUD_PlayerMove
void /*CL_DLLEXPORT*/ HUD_ClientMove(struct playermove_s *ppmove, qboolean server)
{
	//RecClClientMove(ppmove, server);
	
	PM_Move(ppmove, server);
};

// HUD_PlayerMoveInit
void /*CL_DLLEXPORT*/ HUD_ClientMoveInit(struct playermove_s *ppmove)
{
	//RecClClientMoveInit(ppmove);
	
	PM_Init(ppmove);
};

// HUD_PlayerMoveTexture
char /*CL_DLLEXPORT*/ HUD_ClientTextureType(const char *name)
{
	//RecClClientTextureType(name);
	
	return PM_GetTextureType(name);
};

int /*CL_DLLEXPORT*/ HUD_ConnectionlessPacket(const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size)
{
	//RecClConnectionlessPacket(net_from, args, response_buffer, response_buffer_size);
	
	// Parse stuff from args
	int nMaxBufferSize = *response_buffer_size;
	
	// Zero it out since we aren't going to respond
	// If we wanted to respond, we'd write data into response_buffer
	*response_buffer_size = 0;
	
	// Since we don't listen for anything here, just respond that it's a bogus message
	// If we didn't reject the message, we'd return 1 for success instead
	return 0;
};

int /*CL_DLLEXPORT*/ HUD_GetHullBounds(int hullnumber, float *mins, float *maxs)
{
	//RecClGetHullBounds(hullnumber, mins, maxs);
	
	//if(!mins || !maxs)
		//return 0;
	
	int nResult{0};
	
	switch(hullnumber)
	{
	case 0: // Normal player hull
		mins = idVec3(-16, -16, -36); // TODO: convert to vec3_t
		maxs = idVec3(16, 16, 36); // TODO: convert to vec3_t
		nResult = 1;
		break;
	case 1: // Crouched player hull
		mins = idVec3(-16, -16, -18); // TODO: convert to vec3_t
		maxs = idVec3(16, 16, 18); // TODO: convert to vec3_t
		nResult = 1;
		break;
	case 2: // Point based hull
		mins = idVec3(0, 0, 0); // TODO: convert to vec3_t
		maxs = idVec3(0, 0, 0); // TODO: convert to vec3_t
		nResult = 1;
		break;
	};
	
	return nResult;
};

void /*CL_DLLEXPORT*/ HUD_Frame(double time)
{
	//RecClHudFrame(time);
	
	gpClientGame->Update(/*time*/);
};

void /*CL_DLLEXPORT*/ HUD_VoiceStatus(int entindex, qboolean bTalking)
{
	//RecClVoiceStatus(entindex, bTalking);
	
	GetClientVoiceMgr()->UpdateSpeakerStatus(entindex, bTalking);
};

void /*CL_DLLEXPORT*/ HUD_DirectorMessage(int nSize, void *pBuf)
{
	//RecClDirectorMessage(nSize, pBuf);
	
	gHUD.mSpectator.DirectorMessage(nSize, pBuf);
};

void HUD_ChatInputPosition(int *x, int *y)
{
	if(x)
		*x = 0;
	
	if(y)
		*y = 0;
};
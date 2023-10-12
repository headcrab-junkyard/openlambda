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
	
	//gpEngine->pfnHookUserMsg("Test", UserMsgHook_Test);
	return 1;
};

void /*CL_DLLEXPORT*/ HUD_Init()
{
	//RecClHudInit();
	
	InitInput();
	gHUD.Init();
	Scheme_Init();
};

int /*CL_DLLEXPORT*/ HUD_VidInit()
{
	//RecClHudVidInit();
	
	gHUD.VidInit();
	VGui_Startup();
	return 1;
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

int /*CL_DLLEXPORT*/ CL_IsThirdPerson()
{
	//RecClIsThirdPerson();
	
	return (cam_thirdperson ? 1 : 0) || (g_iUser1 && (g_iUser2 == gpEngine->GetLocalPlayer()->index));
};

void /*CL_DLLEXPORT*/ CL_GetCameraOffsets(float *ofs)
{
	//RecClGetCameraOffsets(ofs);
	
	//if(!ofs)
		//return;
	
	VectorCopy(cam_ofs, ofs);
};

struct kbutton_s /*CL_DLLEXPORT*/ *KB_FindKey(const char *name)
{
	//RecClFindKey(name);
	
	kblist_t *pList{g_kbkeys};
	
	while(pList)
	{
		if(!stricmp(name, pList->name))
			return pList->pkey;
		
		pList = pList->next;
	};
	
	return nullptr;
};

void /*CL_DLLEXPORT*/ CAM_Think()
{
	//RecClCamThink();
	
	vec3_t origin;
	vec3_t ext, pnt, camForward, camRight, camUp;
	moveclip_t clip;
	float dist;
	vec3_t camAngles;
	float flSensitivity;
#ifdef LATER
	int i;
#endif
	vec3_t viewangles;
	
	switch()
	{
	};
	
	if(!cam_thirdperson)
		return;
	
#ifdef LATER
	if(cam_contain->value)
	{
		gpEngine->GetClientOrigin(origin);
		ext[0] = ext[1] = ext[2] = 0.0f;
	};
#endif
	
	camAngles[PITCH] = cam_idealpitch->value;
	camAngles[YAW] = cam_idealyaw->value;
	dist = cam_idealdist->value;
	
	// Movement of the camera with the mouse
	if(cam_mousemove)
	{
		SDL_GetCursorPos(&cam_mouse);
		
		if(!cam_distancemove)
		{
		};
	};
	
	if()
		camAngles[PITCH]
	else if()
		camAngles[PITCH]
};

int /*CL_DLLEXPORT*/ HUD_AddEntity(int type, struct cl_entity_s *ent, const char *modelname)
{
	//RecClAddEntity(type, ent, modelname);
	
	switch(type)
	{
	case ET_NORMAL:
		Bench_CheckEntity(type, ent, modelname);
		break;
	case ET_PLAYER:
	case ET_BEAM:
	case ET_TEMPENTITY:
	case ET_FRAGMENTED:
	default:
		break;
	};
	
	// Each frame every entity passes this function so the overview hooks it to filter the overview entities
	
	if(g_iUser1)
	{
		gHUD.mSpectator.AddOverviewEntity(type, ent, modelname);
		
		if((g_iUser1 == OBS_IN_EYE || gHUD.mSpectator.m_pip->value == INSET_IN_EYE) && ent->index == g_iUser2)
			return 0; // Don't draw the player we are following in eye
	};
	
	return 1;
};

void /*CL_DLLEXPORT*/ HUD_CreateEntities()
{
	//RecClCreateEntities();
	
#ifdef BEAM_TEST
	Beams();
#endif
	
	Bench_AddObjects();
	
	// Add in any game-specfic objects
	Game_AddObjects();
	
	GetClientVoiceMgr()->CreateEntities();
};

void HUD_DrawNormalTriangles()
{
	// TODO
};

void HUD_DrawTransparentTriangles()
{
	// TODO
};

void /*CL_DLLEXPORT*/ HUD_StudioEvent(const struct mstudioevent_s *event, const struct cl_entity_s *entity)
{
	//RecClStudioEvent(event, entity);
	
	int nMuzzleFlash{1};
	
#ifdef _TFC
	if(gbACSpinning[entity->index - 1])
		nMuzzleFlash = 0;
#endif
	
	switch(event->event)
	{
	case 5001:
		break;
	case 5002:
		break;
	case 5004: // Client-side sound
		break;
	case 5011:
		break;
	case 5021:
		break;
	case 5031:
		break;
	default:
		break;
	};
	
	// TODO
};

void /*CL_DLLEXPORT*/ HUD_PostRunCmd(struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed)
{
	//RecClPostRunCmd(from, to, cmd, runfuncs, time, random_seed);
	
	g_runfuncs = runfuncs;
	
#ifdef CLIENT_WEAPONS
	if(cl_lw && cl_lw->value)
		HUD_WeaponsPostThink(from, to, cd, time, random_seed);
	else
#endif
		to->client.fov = g_lastFOV;
	
	if(g_irunninggausspred == 1)
	{
		idVec3 vForward{};
		gpEngine->pfnAngleVectors(v_angles, vForward, nullptr, nullptr);
		to->client.velocity = to->client.velocity - vForward * g_flApplyVel * 5;
		g_irunninggausspred = false;
	};
	
	// All games can use FOV state
	g_lastFOV = to->client.fov;
};

void /*CL_DLLEXPORT*/ HUD_Shutdown()
{
	gpClientGame->Shutdown();
};

void /*CL_DLLEXPORT*/ HUD_TxferLocalOverrides(struct entity_state_s *state, const struct clientdata_s *client)
{
	//RecClTxferLocalOverrides(state, client);
	
	VectorCopy(client->origin, state->origin);
	
	// Spectator
	state->iuser1 = client->iuser1;
	state->iuser2 = client->iuser2;
	
	// Duck prevention
	state->iuser3 = client->iuser3;
	
	// Fire prevention
	state->iuser4 = client->iuser4;
};

void /*CL_DLLEXPORT*/ HUD_ProcessPlayerState(struct entity_state_s *dst, const struct entity_state_s *src)
{
	//RecClProcessPlayerState(dst, src);
	
	// Copy in network data
	VectorCopy(src->origin, dst->origin);
	VectorCopy(src->angles, dst->angles);
	
	VectorCopy(src->velocity, dst->velocity);
	
	dst->frame = src->frame;
	dst->modelindex = src->modelindex;
	dst->skin = src->skin;
	dst->effects = src->effects;
	dst->weaponmodel = src->weaponmodel;
	dst->movetype = src->movetype;
	dst->sequence = src->sequence;
	dst->animtime = src->animtime;
	
	dst->solid = src->solid;
	
	dst->rendermode = src->rendermode;
	dst->renderamt = src->renderamt;
	dst->rendercolor.r = src->rendercolor.r;
	dst->rendercolor.g = src->rendercolor.g;
	dst->rendercolor.b = src->rendercolor.b;
	dst->renderfx = src->renderfx;
	
	dst->framerate = src->framerate;
	dst->body = src->body;
	
	memcpy(&dst->controller[0], &src->controller[0], 4 * sizeof(byte));
	memcpy(&dst->blending[0], &src->blending[0], 4 * sizeof(byte));
	
	VectorCopy(src->basevelocity, dst->basevelocity);
	
	dst->friction = src->friction;
	dst->gravity = src->gravity;
	dst->gaitsequence = src->gaitsequence;
	dst->spectator = src->spectator;
	dst->usehull = src->usehull;
	dst->playerclass = src->playerclass;
	dst->team = src->team;
	dst->colormap = src->colormap;

#ifdef _TFC
	dst->fuser1 = src->fuser1;
#endif
	
	// Save off some data so other areas of the client game module can get to it
	auto pPlayer{gpEngine->GetLocalPlayer()}; // Get the local player's index
	
	if(dst->number == pPlayer->index)
	{
		g_iPlayerClass = dst->playerclass;
		g_iTeamNumber = dst->team;
		
		g_iUser1 = src->iuser1;
		g_iUser2 = src->iuser2;
		g_iUser3 = src->iuser3;
	};
};

void /*CL_DLLEXPORT*/ HUD_TxferPredictionData(struct entity_state_s *ps, const struct entity_state_s *pps, struct clientdata_s *pcd, const struct clientdata_s *ppcd, struct weapon_data_s *wd, const struct weapon_data_s *pwd)
{
	//RecClTxferPredictionData(ps, pps, pcd, ppcd, wd, pwd);
	
	ps->oldbuttons = pps->oldbuttons;
	ps->flFallVelocity = pps->flFallVelocity;
	ps->iStepLeft = pps->iStepLeft;
	ps->playerclass = pps->playerclass;
	
	pcd->viewmodel = ppcs->viewmodel;
	pcd->m_iId = ppcd->m_iId;
	
	pcd->ammo_shells = ppcd->ammo_shells;
	pcd->ammo_nails = ppcd->ammo_nails;
	pcd->ammo_cells = ppcd->ammo_cells;
	pcd->ammo_rockets = ppcd->ammo_rockets;
	
	pcd->m_flNextAttack = ppcd->m_flNextAttack;
	pcd->fov = ppcd->fov;
	pcd->weaponanim = ppcd->weaponanim;
	pcd->tfstate = ppcd->tfstate;
	pcd->maxspeed = ppcd->maxspeed;
	
	pcd->deadflag = ppcd->deadflag;
	
	// If spectating or not dead -> get control over view angles
	g_iAlive = (ppcd->iuser1 || (pcd->deadflag == DEAD_NO)) ? 1 : 0;
	
	// Spectator
	pcd->iuser1 = ppcd->iuser1;
	pcd->iuser2 = ppcd->iuser2;
	
	// Duck prevention
	pcd->iuser3 = ppcd->iuser3;
	
	if(gpEngine->IsSpectateOnly())
	{
		// In spectator mode we tell the engine who we want to spectate and how
		// iuser3 is not used for duck prevention (since the spectator can't duck at all)
		pcd->iuser1 = g_iUser1; // Observer mode
		pcd->iuser2 = g_iUser2; // First target
		pcd->iuser3 = g_iUser3; // Second target
	};
	
	// Fire prevention
	pcd->iuser4 = ppcd->iuser4;
	
	pcd->fuser2 = ppcd->fuser2;
	pcs->fuser3 = ppcs->fuser3;
	
	VectorCopy(ppcd->vuser1, pcd->vuser1);
	VectorCopy(ppcd->vuser2, pcd->vuser2);
	VectorCopy(ppcd->vuser3, pcd->vuser3);
	VectorCopy(ppcd->vuser4, pcd->vuser4);
	
	memcpy(wd, pwd, 32 * sizeof(weapon_data_t)); // TODO: 64?
};

void /*CL_DLLEXPORT*/ CL_ReadDemoBuffer(int size, byte *buffer)
{
	// TODO
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
	//RecClGetHullBounsa(hullnumber, mins, maxs);
	
	if(!mins || !maxs)
		return 0;
	
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

int /*CL_DLLEXPORT*/ HUD_Key_Event(int down, int keynum, const char *pszCurrentBinding)
{
	//RecClKeyEvent(down, keynum, pszCurrentBinding);
	
	if(gpViewPort)
		gpViewPort->HandleKeyInput(down, keynum, pszCurrentBinding);
	
	return 1;
};

void /*CL_DLLEXPORT*/ HUD_TempEntUpdate(double frametime, double client_time, double cl_gravity, struct tempent_s **ppFreeTE, struct tempent_s **ppActiveTE, int (*addEntity)(struct cl_entity_s *pEntity), void (*playTESound)(struct tempent_s *pTemp, float damp))
{
	//RecClTempEntUpdate(frametime, client_time, cl_gravity, ppFreeTE, ppActiveTE, addEntity, playTESound);
	
	// TODO
};

struct cl_entity_s /*CL_DLLEXPORT*/ *HUD_GetUserEntity(int index)
{
	//RecClGetUserEntity(index);
	
#ifdef BEAM_TEST
	// None by default, you would return a valid pointer if you create a client-side beam and attach it to a client-side entity
	if(index > 0 && index <= 1)
		return &beams[index];
	else
		return nullptr;
#else
	return nullptr;
#endif
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

int HUD_GetPlayerTeam(int playernum)
{
	return 0;
};

void *HUD_ClientFactory()
{
	return (void*)Sys_GetFactoryThis();
};
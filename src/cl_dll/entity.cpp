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
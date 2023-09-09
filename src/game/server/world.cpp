/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2023 BlackPhrase
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
/// @brief main/world setup functions

#include <engine/edict.h>

#include "exports.h"
#include "engine.h"
#include "BaseEntity.hpp"
#include "Util.hpp"
#include "World.hpp"

//============================================================================

extern enginefuncs_t gEngFuncs;
enginefuncs_t *gpEngine = &gEngFuncs;

edict_t *bodyque_head{nullptr};

CBaseEntity *lastspawn{nullptr};

extern int gbDisplayTitle;

static void InitBodyQue();

/// World spawn flags
constexpr auto SF_WORLD_DARK{0x0001}; ///< Fade from black at startup
constexpr auto SF_WORLD_TITLE{0x0002}; ///< Display game title at startup
constexpr auto SF_WORLD_FORCETEAM{0x0004}; ///< Force teams

//=======================
/*QUAKED worldspawn (0 0 0) ?
Only used for the world entity.
Set message to the level name.
Set sounds to the cd track to play.

World Types:
0: medieval
1: metal
2: base
*/
//=======================
LINK_ENTITY_TO_CLASS(worldspawn, CWorldSpawn);

void CWorldSpawn::Spawn()
{
	//Precache();
	{
		lastspawn = nullptr; // nullptr = world entity
		
		// Custom map attributes
		
		gpEngine->pfnCVarSetString("sv_gravity", "800"); // 67 ft/sec
		//gpEngine->pfnCVarSetString("sv_stepsize", "18");
		
		gpEngine->pfnCVarSetString("room_type", "0"); // Clear DSP
		
		// Set up game rules
		if(gpGameRules)
			delete gpGameRules;
		
		gpGameRules = gpGame->InstallRules();
		
		//pSoundEnt->Spawn();
		
		InitBodyQue();
		
		// Init sentence group playback stuff from sentences.txt
		// Ok to call this multiple times, calls after first will be ignored
		//SENTENCEG_Init();
		
		// Init texture type array from materials.txt
		//TEXTURETYPE_Init();
		
		// The area based ambient sounds MUST be the first precache_sounds
		
		// Player precaches     
		//W_Precache(); // Get weapon precaches // TODO
		
		//ClientPrecache();
		
		// Sounds used from C physics code
		PrecacheSound("common/null.wav"); // Clears sound channels
		
		PrecacheSound("demon/dland2.wav");            // landing thud
		PrecacheSound("misc/h2ohit1.wav");            // landing splash
		
		// Setup precaches always needed
		PrecacheSound("items/itembk2.wav");           // item respawn sound
		PrecacheSound("player/plyrjmp8.wav");         // player jump
		PrecacheSound("player/land.wav");                     // player landing
		PrecacheSound("player/land2.wav");            // player hurt landing
		PrecacheSound("player/drown1.wav");           // drowning pain
		PrecacheSound("player/drown2.wav");           // drowning pain
		PrecacheSound("player/gasp1.wav");            // gasping for air
		PrecacheSound("player/gasp2.wav");            // taking breath
		PrecacheSound("player/h2odeath.wav");         // drowning death
		
		PrecacheSound("misc/talk.wav");                       // talk
		PrecacheSound("player/teledth1.wav");         // telefrag
		
		PrecacheSound("misc/r_tele1.wav");            // teleport sounds
		PrecacheSound("misc/r_tele2.wav");
		PrecacheSound("misc/r_tele3.wav");
		PrecacheSound("misc/r_tele4.wav");
		PrecacheSound("misc/r_tele5.wav");
		
		PrecacheSound("weapons/lock4.wav");           // ammo pick up
		PrecacheSound("weapons/pkup.wav");            // weapon up
		PrecacheSound("items/armor1.wav");            // armor up
		PrecacheSound("weapons/lhit.wav");            //lightning
		PrecacheSound("weapons/lstart.wav");          //lightning start
		PrecacheSound("items/damage3.wav");
		
		PrecacheSound("misc/power.wav");                      //lightning for boss
		
		// Player gib sounds
		PrecacheSound("player/gib.wav");                      // player gib sound
		PrecacheSound("player/udeath.wav");           // player gib sound
		PrecacheSound("player/tornoff2.wav");         // gib sound
		
		// Player pain sounds
		PrecacheSound("player/pain1.wav");
		PrecacheSound("player/pain2.wav");
		PrecacheSound("player/pain3.wav");
		PrecacheSound("player/pain4.wav");
		PrecacheSound("player/pain5.wav");
		PrecacheSound("player/pain6.wav");
		
		// Player death sounds
		PrecacheSound("player/death1.wav");
		PrecacheSound("player/death2.wav");
		PrecacheSound("player/death3.wav");
		PrecacheSound("player/death4.wav");
		PrecacheSound("player/death5.wav");
		
		PrecacheSound("boss1/sight1.wav");
		
		// Axe sounds    
		PrecacheSound("weapons/ax1.wav");                     // ax swoosh
		PrecacheSound("player/axhit1.wav");           // ax hit meat
		PrecacheSound("player/axhit2.wav");           // ax hit world
		
		PrecacheSound("player/h2ojump.wav");          // player jumping into water
		PrecacheSound("player/slimbrn2.wav");         // player enter slime
		PrecacheSound("player/inh2o.wav");            // player enter water
		PrecacheSound("player/inlava.wav");           // player enter lava
		PrecacheSound("misc/outwater.wav");           // leaving water sound
		
		PrecacheSound("player/lburn1.wav");           // lava burn
		PrecacheSound("player/lburn2.wav");           // lava burn
		
		PrecacheSound("misc/water1.wav");                     // swimming
		PrecacheSound("misc/water2.wav");                     // swimming
		
		// Invulnerability sounds
		PrecacheSound("items/protect.wav");
		PrecacheSound("items/protect2.wav");
		PrecacheSound("items/protect3.wav");
		
		//PrecacheModel("models/player.mdl"); // TODO: studio models are not supported yet...
		/*
		PrecacheModel("models/eyes.mdl");
		PrecacheModel("models/h_player.mdl");
		PrecacheModel("models/gib1.mdl");
		PrecacheModel("models/gib2.mdl");
		PrecacheModel("models/gib3.mdl");
		
		PrecacheModel("sprites/s_bubble.spr"); // drowning bubbles
		PrecacheModel("sprites/s_explod.spr"); // sprite explosion
		
		PrecacheModel("models/v_axe.mdl");
		PrecacheModel("models/v_shot.mdl");
		PrecacheModel("models/v_nail.mdl");
		
		PrecacheModel("models/bolt.mdl"); // for lightning gun
		PrecacheModel("models/bolt2.mdl"); // for lightning gun
		PrecacheModel("models/bolt3.mdl"); // for boss shock
		PrecacheModel("models/lavaball.mdl"); // for testing
		
		PrecacheModel("models/missile.mdl");
		PrecacheModel("models/grenade.mdl");
		PrecacheModel("models/spike.mdl");
		PrecacheModel("models/s_spike.mdl");
		
		PrecacheModel("models/backpack.mdl");
		
		PrecacheModel("models/zom_gib.mdl");
		
		PrecacheModel("models/v_light.mdl");
		*/

	//
	// Setup light animation tables. 'a' is total darkness, 'z' is maxbright.
	//

		// 0 normal
		gpEngine->pfnLightStyle(0, "m");
		
		// 1 FLICKER (first variety)
		gpEngine->pfnLightStyle(1, "mmnmmommommnonmmonqnmmo");
		
		// 2 SLOW STRONG PULSE
		gpEngine->pfnLightStyle(2, "abcdefghijklmnopqrstuvwxyzyxwvutsrqponmlkjihgfedcba");
		
		// 3 CANDLE (first variety)
		gpEngine->pfnLightStyle(3, "mmmmmaaaaammmmmaaaaaabcdefgabcdefg");
		
		// 4 FAST STROBE
		gpEngine->pfnLightStyle(4, "mamamamamama");
		
		// 5 GENTLE PULSE 1
		gpEngine->pfnLightStyle(5,"jklmnopqrstuvwxyzyxwvutsrqponmlkj");
		
		// 6 FLICKER (second variety)
		gpEngine->pfnLightStyle(6, "nmonqnmomnmomomno");
		
		// 7 CANDLE (second variety)
		gpEngine->pfnLightStyle(7, "mmmaaaabcdefgmmmmaaaammmaamm");
		
		// 8 CANDLE (third variety)
		gpEngine->pfnLightStyle(8, "mmmaaammmaaammmabcdefaaaammmmabcdefmmmaaaa");
		
		// 9 SLOW STROBE (fourth variety)
		gpEngine->pfnLightStyle(9, "aaaaaaaazzzzzzzz");
		
		// 10 FLUORESCENT FLICKER
		gpEngine->pfnLightStyle(10, "mmamammmmammamamaaamammma");

		// 11 SLOW PULSE NOT FADE TO BLACK
		gpEngine->pfnLightStyle(11, "abcdefghijklmnopqrrqponmlkjihgfedcba");
		
		// styles 32-62 are assigned by the light program for switchable lights

		// 63 testing
		gpEngine->pfnLightStyle(63, "a");
		
		//
		
		gpEngine->pfnCVarSetFloat("sv_zmax", 4096);
		
		if(self->speed > 0)
			gpEngine->pfnCVarSetFloat("sv_zmax", self->speed);
		
		if(self->netname)
		{
			auto pEntity{nullptr};
			if(pEntity)
			{
			};
		};
		
		gpEngine->pfnCVarSetFloat("v_dark", 0.0f);
		
		if(GetSpawnFlags() & SF_WORLD_DARK)
			gpEngine->pfnCVarSetFloat("v_dark", 1.0f);
		
		gbDisplayTitle = false;
		
		if(GetSpawnFlags() & SF_WORLD_TITLE)
			gbDisplayTitle = true;
		
		gpEngine->pfnCVarSetFloat("mp_defaultteam", 0);
		
		if(GetSpawnFlags() & SF_WORLD_FORCETEAM)
			gpEngine->pfnCVarSetFloat("mp_defaultteam", 1);
	};
};

bool CWorldSpawn::HandleKeyValue(ogs::tStringView asKey, ogs::tStringView asValue)
{
	if(asKey == "skyname")
	{
		gpEngine->pfnCVarSetString("sv_skyname", asValue.data());
		return true;
	}
	else if(asKey == "sounds")
	{
		gpGlobals->cdAudioTrack = std::stoi(asValue.data());
		return true;
	}
	else if(asKey == "WaveHeight")
	{
		self->scale = std::stof(asValue.data()) * (1.0 / 8.0);
		gpEngine->pfnCVarSetFloat("sv_wateramp", self->scale);
		return true;
	}
	else if(asKey == "MaxRange")
	{
		self->speed = std::stof(asValue.data());
		return true;
	}
	else if(asKey == "chaptertitle")
	{
		self->netname = gpEngine->pfnAllocString(asValue.c_str());
		return true;
	}
	else if(asKey == "startdark")
	{
		if(std::stoi(asValue.data()))
			self->spawnflags |= SF_WORLD_DARK;
		return true;
	}
	else if(asKey == "newunit")
	{
		if(std::stoi(asValue.data()))
			gpEngine->pfnCVarSetFloat("sv_newunit", 1.0f);
		return true;
	}
	else if(asKey == "gametitle")
	{
		if(std::stoi(asValue.data()))
			self->spawnflags |= SF_WORLD_TITLE;
		return true;
	}
	else if(asKey == "mapteams")
	{
		self->team = gpEngine->pfnAllocString(asValue.c_str());
		return true;
	}
	else if(asKey == "defaultteam")
	{
		if(std::stoi(asValue.data()))
			self->spawnflags |= SF_WORLD_FORCETEAM;
		return true;
	};
	
	return CBaseEntity::HandleKeyValue(asKey, asValue);
};

/*
==============================================================================

BODY QUE

==============================================================================
*/

class CBodyQue : public CBaseEntity
{
};

LINK_ENTITY_TO_CLASS(bodyque, CBodyQue);

static void InitBodyQue()
{
	// TODO
/*
	bodyque_head = gpEngine->pfnSpawn();
	bodyque_head->v.classname = "bodyque";
	
	// Reserve 3 more slots for dead bodies
	//for(int i = 0; i < 3; ++i)
	//{
		bodyque_head->v.owner = gpEngine->pfnSpawn();
		bodyque_head->v.owner->v.classname = "bodyque";
		
		bodyque_head->v.owner->v.owner = gpEngine->pfnSpawn();
		bodyque_head->v.owner->v.owner->v.classname = "bodyque";
		
		bodyque_head->v.owner->v.owner->v.owner = gpEngine->pfnSpawn();
		bodyque_head->v.owner->v.owner->v.owner->v.classname = "bodyque";
	//};
	
	bodyque_head->v.owner->v.owner->v.owner->v.owner = bodyque_head;
*/
};

// make a body que entry for the given ent so the ent can be
// respawned elsewhere
void CopyToBodyQue(entvars_t *ent)
{
	if(ent->effects & EF_NODRAW)
		return;
	
	auto pBodyQueVars{gpEngine->pfnGetVarsOfEnt(bodyque_head)};
	
	//pBodyQueVars->angles = ent->angles; // TODO
	pBodyQueVars->angles[0] = ent->angles[0];
	pBodyQueVars->angles[1] = ent->angles[1];
	pBodyQueVars->angles[2] = ent->angles[2];
	pBodyQueVars->model = ent->model;
	pBodyQueVars->modelindex = ent->modelindex;
	pBodyQueVars->frame = ent->frame;
	pBodyQueVars->colormap = ent->colormap;
	pBodyQueVars->movetype = MOVETYPE_TOSS; // TODO: was ent->movetype
	//pBodyQueVars->velocity = ent->velocity; // TODO
	pBodyQueVars->velocity[0] = ent->velocity[0];
	pBodyQueVars->velocity[1] = ent->velocity[1];
	pBodyQueVars->velocity[2] = ent->velocity[2];
	pBodyQueVars->flags = 0;
	
	pBodyQueVars->deadflag = ent->deadflag;
	
	pBodyQueVars->renderfx = kRenderFxDeadPlayer; // TODO
	//pBodyQueVars->renderamt = GetEntityIndex(GetEntityFromVars(ent)); // TODO
	
	pBodyQueVars->effects = ent->effects | EF_NOINTERP;
	
	pBodyQueVars->sequence = ent->sequence;
	pBodyQueVars->animtime = ent->animtime;
	
	gpEngine->pfnSetOrigin(pBodyQueVars, ent->origin);
	gpEngine->pfnSetSize(pBodyQueVars, ent->mins, ent->maxs);
	
	bodyque_head = pBodyQueVars->owner;
};
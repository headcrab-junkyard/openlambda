/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-2001 Id Software, Inc.
 * Copyright (C) 2018, 2020-2022 BlackPhrase
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
/// @brief generic monster

#pragma once

#include "BaseToggle.hpp"

class CBaseCharacter : public CBaseToggle
{
public: // CBaseEntity public overrides
	bool HandleKeyValue(const std::string &asKey, const std::string &asValue) override;
	
	/*virtual*/ int TakeHealth(float afValue, int anDmgTypeBits);
	/*virtual*/ int TakeDamage();
	
	void Use(CBaseEntity *apActivator, CBaseEntity *apCaller, UseType aeUseType, float afValue);
public: // The rest of the public methods
	/*virtual*/ int GetBloodColor() const {return mnBloodColor;}
	
	Task_t *GetTask() const;

	enum class Sex : int
	{
		Unknown = -1,
		Male = 0,
		Female
	};
	
	Sex GetSex() const {return meSex;}
private:
	Sex meSex{Sex::Unknown};
private:
	typedef struct
	{
		void	(*aifunc)(edict_t *self, float dist);
		float	dist;
		void	(*thinkfunc)(edict_t *self);
	} mframe_t;

	typedef struct
	{
		int			firstframe;
		int			lastframe;
		mframe_t	*frame;
		void		(*endfunc)(edict_t *self);
	} mmove_t;

	typedef struct
	{
		mmove_t		*currentmove;
		int			aiflags;
		int			nextframe;
		float		scale;

		void		(*stand)(edict_t *self);
		void		(*idle)(edict_t *self);
		void		(*search)(edict_t *self);
		void		(*walk)(edict_t *self);
		void		(*run)(edict_t *self);
		void		(*dodge)(edict_t *self, edict_t *other, float eta);
		void		(*attack)(edict_t *self);
		void		(*melee)(edict_t *self);
		void		(*sight)(edict_t *self, edict_t *other);
		qboolean	(*checkattack)(edict_t *self);

		float		pausetime;
		float		attack_finished;

		vec3_t		saved_goal;
		float		search_time;
		float		trail_time;
		vec3_t		last_sighting;
		int			attack_state;
		int			lefty;
		float		idle_time;
		int			linkcount;

		int			power_armor_type;
		int			power_armor_power;
	} monsterinfo_t;
	
	monsterinfo_t Info{};
	
	//
	// doors
	//
	float aflag;
	float dmg; // damage done by door when hit
};
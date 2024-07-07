/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2024 BlackPhrase
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

#pragma once

#include "LegacyEntityComponent.hpp"

class CLegacyDamageable : public CLegacyEntityComponent
{
public:
	enum class DmgType : int
	{
		Generic = 0,
	};
	
	enum class GibType : int
	{
		Normal = 0, ///< Gib if entity was overkilled
		Never, ///< Never gib, no matter how much dmg is done (used for freezing and such)
		Always ///< Always gib (Houndeye Shock, Barnacle Bite and such)
	};
public:
	CLegacyDamageable(entvars_t *apVars) : CLegacyEntityComponent(apVars)
	{
		mfMaxHealth = 100.0f; // TODO
		mfHealth = mfMaxHealth;
	};
	
	void SetHealth(float afValue)
	{
		if(self)
			self->health = afValue;
		
		mfHealth = afValue;
	};
	
	void AddHealth(float afValue)
	{
		if(self)
			self->health += afValue;
		
		mfHealth += afValue;
	};
	
	float GetHealth() const
	{
		if(self)
			mfHealth = self->health;
		
		return mfHealth;
	};
	
	void SetMaxHealth(float afValue){self->max_health = afValue;}
	float GetMaxHealth() const {return self->max_health;}
	
	/// Called when the entity receives damage
	virtual int TakeDamage(CBaseEntity *apInflictor, CBaseEntity *apAttacker, float afDamage, int anDmgBitSum); // TODO: was T_Damage
	
	/// Called when the entity receives health
	virtual float TakeHealth(float afValue, float afIgnore, int anDmgBitSum); // TODO: wtf is afIgnore and where did it come from?
	
	/// Called when the entity gets killed/destroyed
	virtual void Killed(CBaseEntity *apAttacker, CBaseEntity *apLastInflictor, GibType aeGibType = GibType::Normal); // TODO: wtf if apLastInflictor?
private:
	float mfHealth{0.0f};
	float mfMaxHealth{0.0f};
};
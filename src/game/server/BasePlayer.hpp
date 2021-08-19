/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2019-2021 BlackPhrase
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

#include "BaseMonster.hpp" // TODO: #include "BaseCharacter.hpp"?


class CGame;
class CBaseItem;

class CBasePlayer : public CBaseMonster
{
public:
	CBasePlayer();
	
	void Spawn() override;
	
	int TakeDamage(const CBaseEntity &aInflictor, const CBaseEntity &aAttacker, float afDamage, int anDmgTypeBitSum) override;
	
	void Killed(const CBaseEntity &aAttacker, const CBaseEntity &aLastInflictor, int anGib) override;
	
	void PreThink();
	void PostThink();
	
	void Jump(){}
	void Duck();
	
	void HandleImpulseCommands();
	
	void UpdateClientData(){}
	
	bool ShouldFadeOnDeath() const {return false;}
	
	const idVec3 &GetGunPosition() const {return {};} // TODO: return by ref?
protected:
	virtual void RoundRespawn(){} // CS Only
private:
	void DeathSound();
	void DeathThink();
	
	void WaterMove();
	
	void ItemPreFrame();
	void ItemPostFrame();
	
	bool AddItem(const CBaseItem &aItem);
	void GiveNamedItem(const char *asName);
	
	bool RemoveItem(const CBaseItem &aItem);
	void RemoveAllItems(int anFlags);
	
	void SelectItem(const char *asName);
	void SelectNextItem(int anID);
	void SelectPrevItem(int anID);
	void SelectLastItem();
	
	void SetAnimation(eAnimType aeAnimType);
	
	void HandleCheatImpulse(int anImpulse);
	
	enum class ActivityType : int
	{
		Leap
	};
	
	ActivityType GetIdealActivity() const;
	
	bool IsAlive() const {return GetHealth() > 0;}
private:
	CGame *mpGame{nullptr};
	
	CBaseItem *mpActiveItem{nullptr};
};
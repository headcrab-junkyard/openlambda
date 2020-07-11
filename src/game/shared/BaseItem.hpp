/*
 * This file is part of OGS Engine
 * Copyright (C) 2018-2020 BlackPhrase
 *
 * OGS Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OGS Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OGS Engine. If not, see <http://www.gnu.org/licenses/>.
 */

/// @file

#pragma once

#include "BaseEntity.hpp"

class CBasePlayer;
class CBaseWeapon;

class CBaseItem : public CBaseEntity
{
public:
	//CBaseItem();
	virtual ~CBaseItem();
	
	virtual void Think();
	
	virtual void PreFrame(){}
	virtual void PostFrame(){}
	
	virtual void Drop(){}
	virtual void Kill(){}
	
	//virtual void GiveToPlayer(CBasePlayer *apPlayer){}
	virtual bool AddToPlayer(CBasePlayer *apPlayer){return false;}
	virtual void AttachToPlayer(CBasePlayer *apPlayer){}
	
	virtual bool AddDuplicate(){return false;} // TODO: purpose?
	
	virtual bool Deploy(){return false;}
	virtual bool Holster(){return false;}
	
	virtual bool CanDeploy() const {return false;}
	virtual bool CanHolster() const {return false;}
	virtual bool CanDrop() const {return false;} // CS Only
	
	virtual void UpdateItemInfo(){}
	virtual void UpdateClientData(CBasePlayer *apPlayer){}
	
	virtual void ItemPreFrame(){}
	virtual void ItemPostFrame(){}
	virtual void ItemHolsterFrame(){}
	virtual void ItemBusyFrame(){}
	
	virtual float GetMaxSpeed() const {return 1.0f;} // CS Only
	
	int PrimaryAmmoIndex() const {return 0;}
	int SecondaryAmmoIndex() const {return 0;}
	
	int ItemSlot() const {return 0;}
	
	CBaseWeapon *GetWeaponPtr() const {return nullptr;}
protected:
	float mfNextPrimaryAttackTime{0.0f};
	float mfNextSecondaryAttackTime{0.0f};
	
	bool mbHolstered{false};
	bool mbReloading{false};
};
/*
 * This file is part of OGS Engine
 * Copyright (C) 2018, 2020 BlackPhrase
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

#include "BaseItem.hpp"

class CBaseWeapon : public CBaseItem
{
public:
	virtual void PrimaryAttack(){}
	virtual void SecondaryAttack(){}
	
	virtual void Reload(){}
	
	virtual void SendWeaponAnim(int anAnim, bool abSkipLocal, int anBody){}
	
	virtual bool AddWeapon(){return true;}
	
	//virtual int ExtractAmmo(){return 0;} // TODO
	//virtual int ExtractClipAmmo(){return 0;} // TODO
	
	virtual bool PlayEmptySound(){}
	virtual void ResetEmptySound(){}
	
	virtual void WeaponIdle(){}
	
	virtual void RetireWeapon(){}
	
	virtual bool ShouldWeaponIdle() const {return true;}
	
	virtual bool UseDecrement() const {return false;}
	
	virtual bool IsUsable() const {return true;}
};
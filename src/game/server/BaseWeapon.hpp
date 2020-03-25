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
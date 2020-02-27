/// @file

#pragma once

#include "BaseEntity.hpp"

class CBasePlayer;
class CBaseWeapon;

class CBaseItem : public CBaseEntity
{
public:
	virtual void PreFrame(){}
	virtual void PostFrame(){}
	
	virtual void Drop(){}
	virtual void Kill(){}
	
	virtual bool AddToPlayer(CBasePlayer *apPlayer){return false;}
	virtual void AttachToPlayer(CBasePlayer *apPlayer){}
	
	virtual bool AddDuplicate(){return false;} // TODO: purpose?
	
	virtual bool Deploy(){return false;}
	virtual bool Holster(){return false;}
	
	virtual bool CanDeploy() const {return false;}
	virtual bool CanHolster() const {return false;}
	
	virtual void UpdateItemInfo(){}
	virtual void UpdateClientData(CBasePlayer *apPlayer);
	
	int PrimaryAmmoIndex() const;
	int SecondaryAmmoIndex() const;
	
	int ItemSlot() const;
	
	CBaseWeapon *GetWeaponPtr() const;
};
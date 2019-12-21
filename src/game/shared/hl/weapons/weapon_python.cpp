/// @file

#include "BaseWeapon.hpp"

class CWeaponPython : public CBaseWeapon
{
public:
	void Spawn() override;
	
	void PrimaryAttack() override;
	void SecondaryAttack() override;
};

LINK_ENTITY_TO_CLASS(weapon_python, CWeaponPython)
LINK_ENTITY_TO_CLASS(weapon_357, CWeaponPython)

void CWeaponPython::Spawn()
{
	SetClassName("weapon_357"); // TODO: gpEngine->pfnMakeString(classname)
	
	//
	// TODO: Precache();
	//
	gpEngine->pfnPrecacheModel("models/v_357.mdl");
	gpEngine->pfnPrecacheModel("models/p_357.mdl");
	gpEngine->pfnPrecacheModel("models/w_357.mdl");
	//
	
	mnID = WEAPON_PYTHON;
	
	SetModel("models/w_357.mdl");
};

void CWeaponPython::PrimaryAttack()
{
	if(mnClip <= 0)
		return;
	
	--mnClip;
	
	mpOwner->SendAnim(PLAYER_ATTACK1);
	
	mpOwner->FireBullets(1, mpOwner->GetGunPosition());
	
	mfNextPrimaryAttack = 0.75;
};

void CWeaponPython::SecondaryAttack()
{
	mfNextSecondaryAttack = 0.5;
};
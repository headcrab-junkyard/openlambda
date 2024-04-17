/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2019-2021, 2023-2024 BlackPhrase
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

//#include "exports.h"
//#include "engine.h"
#include <BaseWeapon.hpp>

enum eMP5
{
	MP5_LONGIDLE = 0,
	MP5_IDLE1,
	MP5_LAUNCH,
	MP5_RELOAD,
	MP5_DEPLOY,
	MP5_FIRE1,
	MP5_FIRE2,
	MP5_FIRE3
};

class CWeaponSMG : public CBaseWeapon
{
public:
	void Spawn() override;
	//void Precache();
	
	bool Deploy() override;
	
	void WeaponIdle(); // TODO: Idle()?
	
	void PrimaryAttack() override;
	void SecondaryAttack() override;
	
	void Reload() override;
	
	int AddToCharacter(CBaseCharacter *apCharacter); // TODO: was AddToPlayer, make return bool?
	
	int GetSecondaryAmmoIndex() const;
	
	int GetItemInfo(ItemInfo *apInfo) const; //(ItemInfo &aInfo);
	
	int GetItemSlot() const {return 3;}
public:
	int mnShell{0}; // TODO: -1?
private:
	unsigned short mnMP5Fire{0};
	unsigned short mnMP5AltFire{0};
};

LINK_ENTITY_TO_CLASS(weapon_9mmAR, CWeaponSMG);
LINK_ENTITY_TO_CLASS(weapon_mp5, CWeaponSMG);

void CWeaponSMG::Spawn()
{
	// Hack to allow for old names
	SetClassName("weapon_9mmAR"); // TODO: gpEngine->pfnMakeString(classname)
	
	//
	// TODO: Precache();
	//
	mnShell = gpEngine->pfnPrecacheModel("models/shell.mdl"); // Brass shell (TE_MODEL)
	
	gpEngine->pfnPrecacheModel("models/v_9mmAR.mdl");
	gpEngine->pfnPrecacheModel("models/p_9mmAR.mdl");
	gpEngine->pfnPrecacheModel("models/w_9mmAR.mdl");
	
	gpEngine->pfnPrecacheModel("models/grenade.mdl");
	
	gpEngine->pfnPrecacheModel("models/w_9mmARclip.mdl");
	
	gpEngine->pfnPrecacheSound("items/9mmclip1.wav");
	
	gpEngine->pfnPrecacheSound("items/clipinsert1.wav");
	gpEngine->pfnPrecacheSound("items/cliprelease1.wav");
	
	gpEngine->pfnPrecacheSound("weapons/hks1.wav");
	gpEngine->pfnPrecacheSound("weapons/hks2.wav");
	gpEngine->pfnPrecacheSound("weapons/hks3.wav");
	
	gpEngine->pfnPrecacheSound("weapons/glauncher.wav");
	gpEngine->pfnPrecacheSound("weapons/glauncher2.wav");
	
	gpEngine->pfnPrecacheSound("weapons/357_cock1.wav");
	
	mnMP5Fire = gpEngine->pfnPrecacheEvent(1, "events/mp5.sc");
	mnMP5AltFire = gpEngine->pfnPrecacheEvent(1, "events/mp52.sc");
	//
	
	mnID = WEAPON_SMG; // TODO: WEAPON_MP5
	
	SetModel("models/w_9mmAR.mdl");
	
	mnDefaultAmmo = MP5_DEFAULT_GIVE;
	
	FallInit(); // Get ready to fall down
};

bool CWeaponSMG::Deploy()
{
	return DefaultDeploy("models/v_9mmAR.mdl", "models/p_9mmAR.mdl", MP5_DEPLOY, "mp5");
};

void CWeaponSMG::WeaponIdle()
{
	ResetEmptySound();
	
	mpOwner->GetAutoAimVector(AUTOAIM_5DEGREES);
	
	if(mfTimeWeaponIdle > UTIL_WeaponTimeBase())
		return;
	
	int nAnim{MP5_IDLE1};
	
	switch(gpEngine->pfnRandomLong(0, 1))
	{
	case 0:
		nAnim = MP5_LONGIDLE;
		break;
	};
	
	SendWeaponAnim(nAnim);
	
	mfTimeWeaponIdle = UTIL_SharedRandomFloat(mpOwner->random_seed, 10, 15); // How long till we do this again
};

void CWeaponSMG::PrimaryAttack()
{
	// Don't fire underwater
	if(mpOwner->GetWaterLevel() == 3) // TODO: make a flag for that?
	{
		PlayEmptySound();
		mfNextPrimaryAttack = 0.15;
		return;
	};
	
	//if(mpOwner->m_rgAmmo[mnSecondaryAmmoType] == 0)
	//{
		//PlayEmptySound();
		//return;
	//};
	
	if(mnClip <= 0)
	{
		PlayEmptySound();
		mfNextPrimaryAttack = 0.15;
		return;
	};
	
	mpOwner->mnWeaponVolume = NORMAL_GUN_VOLUME;
	mpOwner->mnWeaponFlash = BRIGHT_GUN_FLASH;
	
	mpOwner->self->effects |= EF_MUZZLEFLASH;
	
	--mnClip;
	//--mpOwner->m_rgAmmo[mnPrimaryAmmoType];
	
	// Play the "shoot" animation
	mpOwner->SetAnim(PLAYER_ATTACK1);
	
	idVec3 vSrc = mpOwner->GetGunPosition();
	idVec3 vAiming = mpOwner->GetAutoAimVector(AUTOAIM_5DEGREES);
	idVec3 vDir;
	
#ifdef CLIENT_DLL
	if(!bIsMultiplayer())
#else
	if(!mpGame->GetRules()->IsMultiplayer())
#endif
	{
		// Optimized multiplayer. Widened to make it easier to hit a moving player
		vDir = mpOwner->FireBulletsPlayer(1, vSrc, vAiming, VECTOR_CONE_6DEGREES, 8192, BULLET_PLAYER_MP5, 2, 0, mpOwner->self, mpOwner->random_seed);
	}
	else
	{
		// Single player spread
		vDir = mpOwner->FireBulletsPlayer(1, vSrc, vAiming, VECTOR_CONE_3DEGREES, 8192, BULLET_PLAYER_MP5, 2, 0, mpOwner->self, mpOwner->random_seed);
	};
	
	int nFlags{0};
	
#ifdef OPENLAMBDA_CLIENT_WEAPONS
	nFlags = FEV_NOTHOST;
#endif
	
	gpEngine->pfnPlaybackEventFull(nFlags, mpOwner->ToEdict(), mnMP5Fire, 0.0, (float*)&g_vecZero, (float*)&g_vecZero, vDir.x, vDir.y, 0, 0, 0, 0);
	
	// HEV suit - indicate out of ammo condition
	if(!mnClip && mpOwner->m_rgAmmo[mnPrimaryAmmoType] <= 0)
		mpOwner->SetSuitUpdate("!HEV_AMO0", false, 0);
	
	mfNextPrimaryAttack = GetNextAttackDelay(0.1);
	
	if(mfNextPrimaryAttack < UTIL_WeaponTimeBase())
		mfNextPrimaryAttack = UTIL_WeaponTimeBase() + 0.1;
	
	mfTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(mpOwner->random_seed, 10, 15);
};

void CWeaponSMG::SecondaryAttack()
{
	// Don't fire underwater
	if(mpOwner->GetWaterLevel() == 3) // TODO: make a flag for that?
	{
		PlayEmptySound();
		mfNextPrimaryAttack = 0.15;
		return;
	};
	
	if(mpOwner->m_rgAmmo[mnSecondaryAmmoType] == 0)
	{
		PlayEmptySound();
		return;
	};
	
	mpOwner->mnWeaponVolume = NORMAL_GUN_VOLUME;
	mpOwner->mnWeaponFlash = BRIGHT_GUN_FLASH;
	
	mpOwner->mnExtraSoundTypes = bits_SOUND_DANGER;
	mpOwner->mfStopExtraSoundTime = UTIL_WeaponTimeBase() + 0.2;
	
	--mpOwner->m_rgAmmo[mnSecondaryAmmoType];
	
	// Play the "shoot" animation
	mpOwner->SetAnim(PLAYER_ATTACK1);
	
	gpEngine->pfnMakeVectors(mpOwner->self->v_angle + mpOwner->self->punchangle);
	
	// We don't add in player velocity anymore
	CGrenade::ShootContact(mpOwner->self, mpOwner->GetOrigin() + mpOwner->self->view_ofs + gpGlobals->v_forward * 16, gpGlobals->v_forward * 800);
	
	int nFlags{0};
	
#ifdef OPENLAMBDA_CLIENT_WEAPONS
	nFlags = FEV_NOTHOST;
#endif
	
	gpEngine->pfnPlaybackEvent(nFlags, mpOwner->ToEdict(), mnMP5AltFire);
	
	mfNextPrimaryAttack = GetNextAttackDelay(1);
	mfNextSecondaryAttack = UTIL_WeaponTimeBase() + 1;
	
	mfTimeWeaponIdle = UTIL_WeaponTimeBase() + 5; // Idle pretty soon after shooting
	
	// HEV suit - indicate out of ammo condition
	if(!mpOwner->m_rgAmmo[mnSecondaryAmmoType])
		mpOwner->SetSuitUpdate("!HEV_AMO0", false, 0);
};

void CWeaponSMG::Reload()
{
	if(mpOwner->GetAmmoCount("9mm") <= 0) // TODO: was mpOwner->ammo_9mm
		return;
	
	DefaultReload(MP5_MAX_CLIP, MP5_RELOAD, 1.5);
};

int CWeaponSMG::AddToCharacter(CBaseCharacter *apCharacter)
{
	if(CBaseWeapon::AddToCharacter(apCharacter))
	{
		gpEngine->pfnMessageBegin(MSG_ONE, gMsgWeapPickup, nullptr, apCharacter->self);
		gpEngine->pfnWriteByte(mnID);
		gpEngine->pfnMessageEnd();
		return true;
	};
	
	return false;
};

int CWeaponSMG::GetSecondaryAmmoIndex() const
{
	return mnSecondaryAmmoType;
};

int CWeaponSMG::GetItemInfo(ItemInfo *apInfo) const
{
	apInfo->pszName = STRING(self->classname);
	
	apInfo->pszAmmo1 = "9mm";
	apInfo->iMaxAmmo1 = _9MM_MAX_CARRY;
	
	apInfo->pszAmmo2 = "ARgrenades";
	apInfo->iMaxAmmo2 = M203_GRENADE_MAX_CARRY;
	
	apInfo->iMaxClip = MP5_MAX_CLIP;
	
	apInfo->iSlot = 2;
	apInfo->iPosition = 0;
	apInfo->iFlags = 0;
	
	apInfo->iId = mnID = WEAPON_MP5;
	apInfo->iWeight = MP5_WEIGHT;
	
	return 1;
};
/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2019-2021, 2024 BlackPhrase
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

enum eShotgun
{
	SHOTGUN_IDLE = 0,
	SHOTGUN_FIRE,
	SHOTGUN_FIRE2,
	SHOTGUN_RELOAD,
	SHOTGUN_PUMP,
	SHOTGUN_RELOAD_START,
	SHOTGUN_DRAW,
	SHOTGUN_HOLSTER,
	SHOTGUN_IDLE4,
	SHOTGUN_IDLE_DEEP
};

/// Special deathmatch shotgun spreads
constexpr auto VECTOR_CONE_DM_SHOTGUN{idVec3(0.08716, 0.04362, 0.00)}; ///< 10 by 5 degrees
constexpr auto VECTOR_CONE_DM_DOUBLESHOTGUN{idVec3(0.17365, 0.04362, 0.00)}; ///< 20 by 5 degrees

class CWeaponShotgun : public CBaseWeapon
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
	
	int GetItemInfo(ItemInfo *apInfo) const; //(ItemInfo &aInfo);
	
	int GetItemSlot() const {return 3;}
private:
	unsigned short mnSingleFireEvent{0};
	unsigned short mnDoubleFireEvent{0};
	
	int mnShell{-1}; // TODO
};

LINK_ENTITY_TO_CLASS(weapon_shotgun, CWeaponShotgun);

void CWeaponShotgun::Spawn()
{
	SetClassName("weapon_9mmhandgun"); // TODO: gpEngine->pfnMakeString(classname)
	
	//
	// TODO: Precache();
	//
	gpEngine->pfnPrecacheModel("models/v_shotgun.mdl");
	gpEngine->pfnPrecacheModel("models/p_shotgun.mdl");
	gpEngine->pfnPrecacheModel("models/w_shotgun.mdl");
	
	mnShell = gpEngine->pfnPrecacheModel("models/shotgunshell.mdl");
	
	gpEngine->pfnPrecacheSound("items/9mmclip1.wav");
	
	gpEngine->pfnPrecacheSound("weapons/dbarrel1.wav");
	gpEngine->pfnPrecacheSound("weapons/sbarrel1.wav");
	
	gpEngine->pfnPrecacheSound("weapons/reload1.wav");
	gpEngine->pfnPrecacheSound("weapons/reload3.wav");
	
	gpEngine->pfnPrecacheSound("weapons/357_cock1.wav"); // Gun empty sound
	gpEngine->pfnPrecacheSound("weapons/scock1.wav"); // Cock gun
	
	mnSingleFireEvent = gpEngine->pfnPrecacheEvent(1, "events/shotgun1.sc");
	mnDoubleFireEvent = gpEngine->pfnPrecacheEvent(1, "events/shotgun2.sc");
	//
	
	mnID = WEAPON_SHOTGUN;
	
	SetModel("models/w_shotgun.mdl");
	
	mnDefaultAmmo = SHOTGUN_DEFAULT_GIVE;
	
	FallInit(); // Get ready to fall
};

bool CWeaponShotgun::Deploy()
{
	return DefaultDeploy("models/v_shotgun.mdl", "models/p_shotgun.mdl", SHOTGUN_DRAW, "shotgun");
};

void CWeaponShotgun::WeaponIdle()
{
	ResetEmptySound();
	
	mpOwner->GetAutoAimVector(AUTOAIM_5DEGREES);
	
	if(mfPumpTime && mfPumpTime < gpGlobals->time)
	{
		// Play pumping sound
		mpOwner->EmitSoundDyn(CHAN_ITEM, "weapons/scock1.wav", 1, ATTN_NORM, 0, 95 + gpEngine->pfnRandomLong(0, 0x1f));
		mfPumpTime = 0;
	};
	
	if(mfTimeWeaponIdle < UTIL_WeaponTimeBase())
	{
		if(mnClip == 0 && mnInSpecialReload == 0 && mpOwner->m_rgAmmo[mnPrimaryAmmoType])
			Reload();
		else if(mnInSpecialReload != 0)
		{
			if(mnClip != 8 && mpOwner->m_rgAmmo[mnPrimaryAmmoType])
				Reload();
			else
			{
				// Reload debounce has timed out
				SendWeaponAnim(SHOTGUN_PUMP);
				
				// Play cocking sound
				mpOwner->EmitSoundDyn(CHAN_ITEM, "weapons/scock1.wav", 1, ATTN_NORM, 0, 95 + gpEngine->pfnRandomLong(0, 0x1f));
				mnInSpecialReload = 0;
				mfTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.5;
			};
		}
		else
		{
			int nAnim{SHOTGUN_IDLE4};
			float fRand{UTIL_SharedRandomFloat(mpOwner->random_seed, 0, 1)};
			
			mfTimeWeaponIdle = UTIL_WeaponTimeBase() + (20.0/9.0);
			
			if(fRand <= 0.8)
			{
				nAnim = SHOTGUN_IDLE_DEEP;
				mfTimeWeaponIdle = UTIL_WeaponTimeBase() + (60.0/12.0); // * RANDOM_LONG(2, 5);
			}
			else if(fRand <= 0.95)
				nAnim = SHOTGUN_IDLE;
			
			SendWeaponAnim(nAnim);
		};
	};
};

void CWeaponShotgun::PrimaryAttack()
{
	// Don't fire underwater
	if(mpOwner->GetWaterLevel() == 3)
	{
		PlayEmptySound();
		mfNextPrimaryAttack = GetNextAttackDelay(0.15);
		return;
	};
	
	if(mnClip <= 0)
	{
		Reload();
		if(mnClip == 0)
			PlayEmptySound();
		return;
	};
	
	mpOwner->mnWeaponVolume = LOUD_GUN_VOLUME;
	mpOwner->mnWeaponFlash = NORMAL_GUN_FLASH;
	
	--mnClip;
	
	mpOwner->SetAnim(PLAYER_ATTACK1);
	
	mpOwner->AddEffects(EF_MUZZLEFLASH);
	
	idVec3 vSrc = mpOwner->GetGunPosition();
	idVec3 vAiming = mpOwner->GetAutoAimVector(AUTOAIM_5DEGREES);
	idVec3 vDir;
	
#ifdef OPENLAMBDA_CLIENT_DLL
	if(bIsMultiplayer())
#else
	if(mpGame->GetRules()->IsMultiplayer())
#endif
	{
		vDir = mpOwner->FireBulletsPlayer(4, vSrc, vAiming, VECTOR_CONE_DM_SHOTGUN, 2048, BULLET_PLAYER_BUCKSHOT, 0, 0, mpOwner->self, mpOwner->random_seed);
	}
	else
	{
		// Regular old, untouched spread
		vDir = mpOwner->FireBulletsPlayer(6, vSrc, vAiming, VECTOR_CONE_10DEGREES, 2048, BULLET_PLAYER_BUCKSHOT, 0, 0, mpOwner->self, mpOwner->random_seed);
	};
	
	int nFlags{0};
	
#ifdef OPENLAMBDA_CLIENT_WEAPONS
	nFlags = FEV_NOTHOST;
#endif
	
	gpEngine->pfnPlaybackEventFull(nFlags, mpOwner->ToEdict(), mnSingleFireEvent, 0.0, (float*)&g_vecZero, (float*)&g_vecZero, vDir.x, vDir.y, 0, 0, 0, 0);
	
	// HEV suit - indicate out of ammo condition
	if(!mnClip && mpOwner->m_rgAmmo[mnPrimaryAmmoType] <= 0)
		mpOwner->SetSuitUpdate("!HEV_AMO0", false, 0);
	
	if(mnClip != 0)
		mfPumpTime = gpGlobals->time + 0.5;
	
	mfNextPrimaryAttack = GetNextAttackDelay(0.75);
	mfNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.75;
	
	if(mnClip != 0)
		mfTimeWeaponIdle = UTIL_WeaponTimeBase() + 5.0;
	else
		mfTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.75;
	
	mnInSpecialReload = 0;
};

void CWeaponShotgun::SecondaryAttack()
{
	// Don't fire underwater
	if(mpOwner->GetWaterLevel() == 3)
	{
		PlayEmptySound();
		mfNextPrimaryAttack = GetNextAttackDelay(0.15);
		return;
	};
	
	if(mnClip <= 1)
	{
		Reload();
		PlayEmptySound();
		return;
	};
	
	mpOwner->mnWeaponVolume = LOUD_GUN_VOLUME;
	mpOwner->mnWeaponFlash = NORMAL_GUN_FLASH;
	
	mnClip -= 2;
	
	int nFlags{0};
	
#ifdef OPENLAMBDA_CLIENT_WEAPONS
	nFlags = FEV_NOTHOST;
#endif
	
	mpOwner->AddEffects(EF_MUZZLEFLASH);
	
	// Player "shoot" animation
	mpOwner->SetAnim(PLAYER_ATTACK1);
	
	idVec3 vSrc{mpOwner->GetGunPosition()};
	idVec3 vAiming{mpOwner->GetAutoAimVector(AUTOAIM_5DEGREES)};
	
	idVec3 vDir{};
	
#ifdef OPENLAMBDA_CLIENT_DLL
	if(bIsMultiplayer())
#else
	if(mpGame->GetRules()->IsMultiplayer())
#endif
	{
		// Tuned for deathmatch
		vDir = mpOwner->FireBulletsPlayer(8, vSrc, vAiming, VECTOR_CONE_DM_DOUBLESHOTGUN, 2048, BULLET_PLAYER_BUCKSHOT, 0, 0, mpOwner->self, mpOwner->random_seed);
	}
	else
	{
		// Untouched default single player
		vDir = mpOwner->FireBulletsPlayer(12, vSrc, vAiming, VECTOR_CONE_10DEGREES, 2048, BULLET_PLAYER_BUCKSHOT, 0, 0, mpOwner->self, mpOwner->random_seed);
	};
	
	gpEngine->pfnPlaybackEventFull(nFlags, mpOwner->ToEdict(), mnDoubleFireEvent, 0.0, (float*)&g_vecZero, (float*)&g_vecZero, vDir.x, vDir.y, 0, 0, 0, 0);
	
	// HEV suit - indicate out of ammo condition
	if(!mnClip && mpOwner->m_rgAmmo[mnPrimaryAmmoType] <= 0)
		mpOwner->SetSuitUpdate("!HEV_AMO0", false, 0);
	
	if(mnClip != 0)
		mfPumpTime = gpGlobals->time + 0.95;
	
	mfNextPrimaryAttack = GetNextAttackDelay(1.5);
	mfNextSecondaryAttack = UTIL_WeaponTimeBase() + 1.5;
	
	mfTimeWeaponIdle = 1.5;
	
	if(mnClip != 0)
		mfTimeWeaponIdle = UTIL_WeaponTimeBase() + 6.0;
	
	mnInSpecialReload = 0;
};

void CWeaponShotgun::Reload()
{
	if(mpOwner->m_rgAmmo[mnPrimaryAmmoType] <= 0 || mnClip == SHOTGUN_MAX_CLIP)
		return;
	
	// Don't reload until recoil is done
	if(mfNextPrimaryAttack > UTIL_WeaponTimeBase())
		return;
	
	// Check to see if we're ready to reload
	if(mnInSpecialReload == 0)
	{
		SendWeaponAnim(SHOTGUN_RELOAD_START);
		
		mnInSpecialReload = 1;
		
		mpOwner->mfNextAttack = UTIL_WeaponTimeBase() + 0.6;
		mfTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.6;
		
		mfNextPrimaryAttack = GetNextAttackDelay(1.0);
		mfNextSecondaryAttack = UTIL_WeaponTimeBase() + 1.0;
		
		return;
	}
	else if(mnInSpecialReload == 1)
	{
		if(mfTimeWeaponIdle > UTIL_WeaponTimeBase())
			return;
		
		// Was waiting for gun to move to the side
		mnInSpecialReload = 2;
		
		if(gpEngine->pfnRandomLong(0, 1))
			mpOwner->EmitSoundDyn(CHAN_ITEM, "weapons/reload1.wav", 1, ATTN_NORM, 0, 85 + gpEngine->pfnRandomLong(0, 0x1f));
		else
			mpOwner->EmitSoundDyn(CHAN_ITEM, "weapons/reload3.wav", 1, ATTN_NORM, 0, 85 + gpEngine->pfnRandomLong(0, 0x1f));
		
		SendWeaponAnim(SHOTGUN_RELOAD);
		
		mfNextReload = UTIL_WeaponTimeBase() + 0.5;
		mfTimeWeaponIdle = UTIL_WeaponTimeBase() + 0.5;
	}
	else
	{
		// Add them to the clip
		++mnClip;
		mpOwner->GetInventory()->AddAmmo(mnPrimaryAmmoType, -1); // ->m_rgAmmo[mnPrimaryAmmoType]
		mnInSpecialReload = 1;
	};
};

int CWeaponShotgun::AddToCharacter(CBaseCharacter *apCharacter)
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

int CWeaponShotgun::GetItemInfo(ItemInfo *apInfo) const
{
	apInfo->pszName = STRING(self->classname);
	
	apInfo->pszAmmo1 = "buckshot";
	apInfo->iMaxAmmo1 = BUCKSHOT_MAX_CARRY;
	
	apInfo->pszAmmo2 = nullptr;
	apInfo->iMaxAmmo2 = -1;
	
	apInfo->iMaxClip = SHOTGUN_MAX_CLIP;
	
	apInfo->iSlot = 2;
	apInfo->iPosition = 1;
	apInfo->iFlags = 0;
	
	apInfo->iId = mnID = WEAPON_SHOTGUN;
	apInfo->iWeight = SHOTGUN_WEIGHT;
	
	return 1;
};
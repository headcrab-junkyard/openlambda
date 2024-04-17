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

enum eRPG
{
	RPG_IDLE = 0,
	RPG_FIDGET,
	RPG_RELOAD, // To reload
	RPG_FIRE2, // To empty
	RPG_HOLSTER1, // Loaded holster
	RPG_DRAW1, // Loaded draw
	RPG_HOLSTER2, // Unloaded holster
	RPG_DRAW_UL, // Unloaded draw
	RPG_IDLE_UL, // Unloaded idle
	RPG_FIDGET_UL // Unloaded fidget
};

class CWeaponRPG : public CBaseWeapon
{
public:
	void Spawn() override;
	//void Precache();
	
	bool Deploy() override;
	
	bool CanHolster() const override;
	void Holster() override;
	
	void WeaponIdle(); // TODO: Idle()?
	
	void PrimaryAttack() override;
	void SecondaryAttack() override;
	
	void Reload() override;
	
	int AddToCharacter(CBaseCharacter *apCharacter); // TODO: was AddToPlayer, make return bool?
	
	int GetItemInfo(ItemInfo *apInfo) const; //(ItemInfo &aInfo);
private:
	void UpdateSpot();
};

LINK_ENTITY_TO_CLASS(weapon_rpg, CWeaponRPG);

void CWeaponRPG::Spawn()
{
	SetClassName("weapon_rpg"); // TODO: gpEngine->pfnMakeString(classname)
	
	//
	// TODO: Precache();
	//
	gpEngine->pfnPrecacheModel("models/v_rpg.mdl");
	gpEngine->pfnPrecacheModel("models/p_rpg.mdl");
	gpEngine->pfnPrecacheModel("models/w_rpg.mdl");
	
	gpEngine->pfnPrecacheSound("items/9mmclip1.wav");
	
	gpEngine->pfnPrecacheSound("weapons/rocketfire1.wav");
	gpEngine->pfnPrecacheSound("weapons/glauncher.wav"); // Alternative fire sound
	
	UTIL_PrecacheOther("laser_spot");
	UTIL_PrecacheOther("rpg_rocket");
	
	mnRPGFire = gpEngine->pfnPrecacheEvent(1, "events/rpg.sc");
	//
	
	mnID = WEAPON_RPG;
	
	SetModel("models/w_rpg.mdl");
	
	mbSpotActive = true;
	
	mnDefaultAmmo = RPG_DEFAULT_GIVE;
	
#ifdef OPENLAMBDA_CLIENT_DLL
	if(bIsMultiplayer())
#else
	if(mpGame->GetRules()->IsMultiplayer())
#endif
	{
		// More default ammo in multiplayer
		mnDefaultAmmo = RPG_DEFAULT_GIVE * 2;
	};
	
	FallInit(); // Get ready to fall down
};

bool CWeaponRPG::Deploy()
{
	if(mnClip == 0)
		return DefaultDeploy("models/v_rpg.mdl", "models/p_rpg.mdl", RPG_DRAW_UL, "rpg");
	
	return DefaultDeploy("models/v_rpg.mdl", "models/p_rpg.mdl", RPG_DRAW1, "rpg");
};

bool CWeaponRPG::CanHolster() const
{
	// Can't put away guiding a missile
	if(mbSpotActive && mnActiveRockets)
		return false;
	
	return true;
};

void CWeaponRPG::Holster(int abSkipLocal)
{
	mbInReload = false; // Cancel any reload in progress
	
	mpOwner->mfNextAttack = UTIL_WeaponTimeBase() + 0.5;
	
	SendWeaponAnim(RPG_HOLSTER1);
	
#ifndef OPENLAMBDA_CLIENT_DLL
	if(mpSpot)
	{
		mpSpot->Killed(nullptr, GIB_NEVER);
		mpSpot = nullptr;
	};
#endif
};

void CWeaponRPG::WeaponIdle()
{
	UpdateSpot();
	
	ResetEmptySound();
	
	if(mfTimeWeaponIdle > UTIL_WeaponTimeBase())
		return;
	
	mfTimeWeaponIdle = UTIL_WeaponTimeBase() + 1;
	
	if(mpOwner->m_rgAmmo[mnPrimaryAmmoType])
	{
		int nAnim{};
		float fRand{UTIL_SharedRandomFloat(mpOwner->random_seed, 0, 1)};
		if(fRand <= 0.75 || mbSpotActive)
		{
			if(mnClip == 0)
				nAnim = RPG_IDLE_UL;
			else
				nAnim = RPG_IDLE;
			
			mfTimeWeaponIdle = UTIL_WeaponTimeBase() + 90.0 / 15.0;
		}
		else
		{
			if(mnClip == 0)
				nAnim = RPG_FIDGET_UL;
			else
				nAnim = RPG_FIDGET;
			
			mfTimeWeaponIdle = UTIL_WeaponTimeBase() + 3.0;
		};
		
		SendWeaponAnim(nAnim);
	};
};

void CWeaponRPG::PrimaryAttack()
{
	UpdateSpot();
	
	if(mnClip <= 0)
	{
		PlayEmptySound();
		return;
	};
	
	mpOwner->mnWeaponVolume = LOUD_GUN_VOLUME;
	mpOwner->mnWeaponFlash = BRIGHT_GUN_FLASH;
	
#ifndef OPENLAMBDA_CLIENT_DLL
	// Player "shoot" animation
	mpOwner->SetAnim(PLAYER_ATTACK1);
	
	gpEngine->pfnMakeVectors(mpOwner->self->v_angle);
	idVec3 vSrc{mpOwner->GetGunPosition() + gpGlobals->v_forward * 16 + gpGlobals->v_right * 8 + gpGlobals->v_up * -8};
	
	CRPGRocket *pRocket{CRPGRocket::CreateRPGRocket(vSrc, mpOwner->self->v_angle, mpOwner, this};
	
	gpEngine->pfnMakeVectors(mpOwner->self->v_angle); // CRPGRocket::Create stomps on globals, so remake
	pRocket->SetVelocity(pRocket->GetVelocity() + gpGlobals->v_forward * DotProduct(mpOwner->GetVelocity(), gpGlobals->v_forward));
#endif
	
	int nFlags{0};
	
#ifdef OPENLAMBDA_CLIENT_WEAPONS
	nFlags = FEV_NOTHOST;
#endif
	
	gpEngine->pfnPlaybackEvent(nFlags, mpOwner->ToEdict(), mnRPGFire);
	
	--mnClip;
	
	mfNextPrimaryAttack = GetNextAttackDelay(1.5);
	mfTimeWeaponIdle = UTIL_WeaponTimeBase() + 1.5;
};

void CWeaponRPG::SecondaryAttack()
{
	mbSpotActive = !mbSpotActive;
	
#ifndef OPENLAMBDA_CLIENT_DLL
	if(!mbSpotActive && mpSpot)
	{
		mpSpot->Killed(nullptr, GIB_NORMAL);
		mpSpot = nullptr;
	};
#endif
	
	mfNextSecondaryAttack = UTIL_WeaponTimeBase() + 0.2;
};

void CWeaponRPG::Reload()
{
	int nResult;
	
	// Don't bother with any of this if don't need to reload
	if(mnClip == 1)
		return;
	
	if(mpOwner->ammo_rockets <= 0)
		return;
	
	// Because the RPG waits to autoreload when no missiles are active while the
	// LTD is on, the weapons code is constantly calling into this function, but is
	// often denied because
	// a) missiles are in flight, but the LTD is on
	// or
	// b) owner is totally out of ammo and has nothing to switch to, and should be
	// allowed to shine the designator around
	
	
	// Set the next attack time into the future so that WeaponIdle will get called
	// more often than reload, allowing the RPG LTD to be updated
	mfNextPrimaryAttack = GetNextAttackDelay(0.5);
	
	// No reloading when there are active missiles tracking the designator
	// Ward off future autoreload attempts by setting the next attack time into the future for a bit
	if(mnActiveRockets && mbSpotActive)
		return;
	
#ifndef OPENLAMBDA_CLIENT_DLL
	if(mpSpot && mbSpotActive)
	{
		mpSpot->Suspend(2.1);
		mfNextSecondaryAttack = UTIL_WeaponTimeBase() + 2.1;
	};
#endif
	
	if(mnClip == 0)
		nResult = DefaultReload(RPG_MAX_CLIP, RPG_RELOAD, 2);
	
	if(nResult)
		mfTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(mpOwner->random_seed, 10, 15);
};

int CWeaponRPG::AddToCharacter(CBaseCharacter *apCharacter)
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

int CWeaponRPG::GetItemInfo(ItemInfo *apInfo) const
{
	apInfo->pszName = STRING(self->classname);
	
	apInfo->pszAmmo1 = "rockets";
	apInfo->iMaxAmmo1 = ROCKET_MAX_CARRY;
	
	apInfo->pszAmmo2 = nullptr;
	apInfo->iMaxAmmo2 = -1;
	
	apInfo->iMaxClip = RPG_MAX_CLIP;
	
	apInfo->iSlot = 3;
	apInfo->iPosition = 0;
	apInfo->iFlags = 0;
	
	apInfo->iId = mnID = WEAPON_RPG;
	apInfo->iWeight = RPG_WEIGHT;
	
	return 1;
};

void CWeaponRPG::UpdateSpot()
{
#ifndef OPENLAMBDA_CLIENT_DLL
	if(mbSpotActive)
	{
		if(!mpSpot)
			mpSpot = CLaserSpot::CreateSpot();
		
		gpEngine->pfnMakeVectors(mpOwner->self->v_angle);
		
		idVec3 vSrc{mpOwner->GetGunPosition()};
		idVec3 vAiming{gpGlobals->v_forward};
		
		TraceResult tr{};
		mpGame->GetWorld()->TraceLine(vSrc, vSrc + vAiming * 8192, dont_ignore_monsters, ENT(mpOwner->self), &tr);
		
		mpSpot->SetOrigin(tr.vecEndPos);
	};
#endif
};
/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018, 2021, 2024 BlackPhrase
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

#include <BaseWeapon.hpp>

enum eDeagle
{
	DEAGLE_IDLE1 = 0,
	DEAGLE_IDLE2,
	DEAGLE_IDLE3,
	DEAGLE_SHOOT,
	DEAGLE_SHOOT_EMPTY,
	DEAGLE_RELOAD,
	DEAGLE_RELOAD_NOT_EMPTY,
	DEAGLE_DRAW,
	DEAGLE_HOLSTER,
	DEAGLE_ADD_SILENCER
};

class CWeaponDeagle final : public CBaseWeapon
{
public:
	CWeaponDeagle();
	~CWeaponDeagle();
	
	//void OnSpawn() override;
	void Spawn() override;
	//void Precache();
	
	bool Deploy() override;
	
	void WeaponIdle(); // TODO: Idle?
	
	void PrimaryAttack() override;
	
	void Reload() override;
private:
	void Fire(float afSpread, float afCycleTime, bool abUseAutoAim);
private:
	int mnShell{0};
	
	unsigned short mnGlockFireEvent;
	
	bool mbBurstMode{false};
};

LINK_ENTITY_TO_CLASS(weapon_deagle, CWeaponDeagle);

CWeaponDeagle::CWeaponDeagle() = default;
CWeaponDeagle::~CWeaponDeagle() = default;

void CWeaponDeagle::Spawn()
{
	// Hack to allow for old names
	SetClassName("weapon_deagle"); // TODO: gpEngine->pfnMakeString(classname)
	
	//
	// TODO: Precache();
	//
	gpEngine->pfnPrecacheModel("models/v_deagle.mdl");
	gpEngine->pfnPrecacheModel("models/p_deagle.mdl");
	gpEngine->pfnPrecacheModel("models/w_deagle.mdl");
	
	mnShell = gpEngine->pfnPrecacheModel("models/shell.mdl"); // Brass shell
	
	gpEngine->pfnPrecacheSound("items/9mmclip1.wav");
	gpEngine->pfnPrecacheSound("items/9mmclip2.wav");
	
	gpEngine->pfnPrecacheSound("weapons/pl_gun1.wav"); // Silenced handgun
	gpEngine->pfnPrecacheSound("weapons/pl_gun2.wav"); // Silenced handgun
	gpEngine->pfnPrecacheSound("weapons/pl_gun3.wav"); // Handgun
	
	gpEngine->pfnPrecacheSound("weapons/deagle-1.wav");
	gpEngine->pfnPrecacheSound("weapons/deagle-2.wav");
	
	mnGlockFireEvent = gpEngine->pfnPrecacheEvent(1, "events/deagle1.sc");
	//
	
	mnID = WEAPON_DEAGLE;
	
	SetModel("models/w_deagle.mdl");
	
	mnDefaultAmmo = DEAGLE_DEFAULT_GIVE;
	
	// Get ready to fall down
	FallInit();
};

bool CWeaponDeagle::Deploy()
{
	//self->body = 1;
	return DefaultDeploy("models/v_deagle.mdl", "models/p_deagle.mdl", DEAGLE_DRAW, "onehanded", /*UseDecrement() ? 1 : */ 0);
};

void CWeaponDeagle::WeaponIdle()
{
	ResetEmptySound();
	
	mpOwner->GetAutoAimVector(AUTOAIM_10DEGREES);
	
	if(mfTimeWeaponIdle > UTIL_WeaponTimeBase())
		return;
	
	// Only idle if the slid isn't back
	if(mnClip != 0)
	{
		int nAnim{DEAGLE_IDLE2};
		float fRand{UTIL_SharedRandomFloat(mpOwner->random_seed, 0.0, 1.0)};
		
		mfTimeWeaponIdle = UTIL_WeaponTimeBase() + 40.0 / 16.0;
		
		if(fRand <= 0.3)
		{
			nAnim = DEAGLE_IDLE3;
			mfTimeWeaponIdle = UTIL_WeaponTimeBase() + 49.0 / 16.0;
		}
		else if(fRand <= 0.6)
		{
			nAnim = DEAGLE_IDLE1;
			mfTimeWeaponIdle = UTIL_WeaponTimeBase() + 60.0 / 16.0;
		};
		
		SendWeaponAnim(nAnim, 1);
	};
};

void CWeaponDeagle::PrimaryAttack()
{
	Fire(0.01, 0.3, true);
};

void CWeaponDeagle::Reload()
{
	if(mpOwner->ammo_9mm <= 0)
		return;
	
	bool bResult{false};
	
	if(mnClip == 0)
		bResult = DefaultReload(7, DEAGLE_RELOAD, 1.5);
	else
		bResult = DefaultReload(7, DEAGLE_RELOAD_NOT_EMPTY, 1.5);
	
	if(bResult)
		//mfTimeWeaponIdle = gpGlobals->time + RANDOM_FLOAT(10, 15);
		mfTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(mpOwner->random_seed, 10, 15);
};

void CWeaponDeagle::Fire(float afSpread, float afCycleTime, bool abUseAutoAim)
{
	if(mnClip <= 0)
	{
		if(mbFireOnEmpty)
		{
			PlayEmptySound();
			mfNextPrimaryAttack = GetNextAttackDelay(0.2);
		};
		
		return;
	};
	
	--mnClip;
	
	mpOwner->AddEffects(EF_MUZZLEFLASH);
	
	int nFlags{0};
	
#ifdef OPENLAMBDA_CLIENT_WEAPONS
	nFlags = FEV_NOTHOST;
#endif
	
	// Player "shoot" animation
	mpOwner->SetAnim(PLAYER_ATTACK1);
	
	if(self->body == 1)
	{
		// Silenced
		mpOwner->mnWeaponVolume = QUIET_GUN_VOLUME;
		mpOwner->mnWeaponFlash = DIM_GUN_FLASH;
	}
	else
	{
		// Non-silenced
		mpOwner->mnWeaponVolume = NORMAL_GUN_VOLUME;
		mpOwner->mnWeaponFlash = NORMAL_GUN_FLASH;
	};
	
	idVec3 vSrc{mpOwner->GetGunPosition()};
	idVec3 vAiming{gpGlobals->v_forward};
	
	if(abUseAutoAim)
		vAiming = mpOwner->GetAutoAimVector(AUTOAIM_10DEGREES);
	
	idVec3 vDir{mpOwner->FireBulletsPlayer(1, vSrc, vAiming, idVec3(afSpread, afSpread, afSpread), 8192, BULLET_PLAYER_9MM, 0, 0, mpOwner->self, mpOwner->random_seed)};
	
	gpEngine->pfnPlaybackEventFull(nFlags, mpOwner->ToEdict(), abUseAutoAim ? mnFireGlock1 : mnFireGlock2, 0.0, (float*)&g_vecZero, (float*)&g_vecZero, vDir.x, vDir.y, 0, 0, mnClip == 0 ? 1 : 0, 0);
	
	mfNextPrimaryAttack = mfNextSecondaryAttack = GetNextAttackDelay(afCycleTime);
	
	// HEV suit - indicate out of ammo condition
	if(!mnClip && mpOwner->m_rgAmmo[mnPrimaryAmmoType] <= 0)
		mpOwner->SetSuitUpdate("!HEV_AMO0", false, 0);
	
	mfTimeWeaponIdle = UTIL_WeaponTimeBase() + UTIL_SharedRandomFloat(mpOwner->random_seed, 10, 15);
};
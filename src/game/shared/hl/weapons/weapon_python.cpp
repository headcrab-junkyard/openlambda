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

#include <BaseWeapon.hpp>

enum ePython
{
	PYTHON_IDLE1 = 0,
	PYTHON_FIDGET,
	PYTHON_FIRE1,
	PYTHON_RELOAD,
	PYTHON_HOLSTER,
	PYTHON_DRAW,
	PYTHON_IDLE2,
	PYTHON_IDLE3
};

class CWeaponPython : public CBaseWeapon
{
public:
	void Spawn() override;
	//void Precache();
	
	bool Deploy() override;
	void Holster() override;
	
	void WeaponIdle(); // TODO: Idle()?
	
	void PrimaryAttack() override;
	void SecondaryAttack() override;
	
	void Reload() override;
	
	int AddToCharacter(CBaseCharacter *apCharacter); // TODO: was AddToPlayer, make return bool?
	
	int GetItemInfo(ItemInfo *apInfo) const;
private:
	unsigned short mnPythonFireEvent{0};
};

LINK_ENTITY_TO_CLASS(weapon_python, CWeaponPython);
LINK_ENTITY_TO_CLASS(weapon_357, CWeaponPython);

void CWeaponPython::Spawn()
{
	// Hack to allow for old names
	SetClassName("weapon_357"); // TODO: gpEngine->pfnMakeString(classname)
	
	//
	// TODO: Precache();
	//
	gpEngine->pfnPrecacheModel("models/v_357.mdl");
	gpEngine->pfnPrecacheModel("models/p_357.mdl");
	gpEngine->pfnPrecacheModel("models/w_357.mdl");
	
	gpEngine->pfnPrecacheModel("models/w_357ammobox.mdl");
	
	gpEngine->pfnPrecacheSound("items/9mmclip1.wav");
	
	gpEngine->pfnPrecacheSound("weapons/357_reload1.wav");
	gpEngine->pfnPrecacheSound("weapons/357_cock1.wav");
	gpEngine->pfnPrecacheSound("weapons/357_shot1.wav");
	gpEngine->pfnPrecacheSound("weapons/357_shot2.wav");
	
	mnPythonFireEvent = gpEngine->pfnPrecacheEvent(1, "events/python.sc");
	//
	
	mnID = WEAPON_PYTHON;
	
	SetModel("models/w_357.mdl");
	
	mnDefaultAmmo = PYTHON_DEFAULT_GIVE;
	
	// Get ready to fall down
	FallInit();
};

bool CWeaponPython::Deploy()
{
	self->body = 0;
	
#ifdef OPENLAMBDA_CLIENT_DLL
	if(bIsMultiplayer())
#else
	if(mpGame->GetRules()->IsMultiplayer())
#endif
	{
		// Enable laser sight geometry in MP
		self->body = 1;
	};
	
	return DefaultDeploy("models/v_357.mdl", "models/p_357.mdl", PYTHON_DRAW, "python", UseDecrement(), self->body);
};

void CWeaponPython::Holster(int abSkipLocal)
{
	// Cancel any reload in progess
	mbInReload = false;
	
	if(mbInZoom)
		SecondaryAttack();
	
	mpOwner->mfNextAttack = UTIL_WeaponTimeBase() + 1.0;
	mfTimeWeaponIdle = UTIL_SharedRandomFloat(mpOwner->random_seed, 10, 15);
	SendWeaponAnim(PYTHON_HOLSTER);
};

void CWeaponPython::WeaponIdle()
{
	ResetEmptySound();
	
	mpOwner->GetAutoAimVector(AUTOAIM_10DEGREES);
	
	if(mfTimeWeaponIdle > UTIL_WeaponTimeBase())
		return;
	
	int nAnim{PYTHON_FIDGET};
	float fRand{UTIL_SharedRandomFloat(mpOwner->random_seed, 0, 1)};
	
	mfTimeWeaponIdle = 170.0 / 30.0;
	
	if(fRand <= 0.5)
	{
		nAnim = PYTHON_IDLE1;
		mfTimeWeaponIdle = 70.0 / 30.0;
	}
	else if(fRand <= 0.7)
	{
		nAnim = PYTHON_IDLE2;
		mfTimeWeaponIdle = 60.0 / 30.0;
	}
	else if(fRand <= 0.9)
	{
		nAnim = PYTHON_IDLE3;
		mfTimeWeaponIdle = 88.0 / 30.0;
	};
	
	bool bUseScope{false};
	
#ifdef OPENLAMBDA_CLIENT_DLL
	bUseScope = bIsMultiplayer();
#else
	bUseScope = mpGame->GetRules()->IsMultiplayer();
#endif
	
	SendWeaponAnim(nAnim, UseDecrement() ? 1 : 0, bUseScope);
};

void CWeaponPython::PrimaryAttack()
{
	// Don't fire underwater
	if(mpOwner->GetWaterLevel() == 3)
	{
		PlayEmptySound();
		mfNextPrimaryAttack = 0.15;
		return;
	};
	
	if(mnClip <= 0)
	{
		if(!mbFireOnEmpty)
			Reload();
		else
		{
			mpOwner->EmitSound(CHAN_WEAPON, "weapons/357_cock1.wav", 0.8, ATTN_NORN);
			mfNextPrimaryAttack = 0.15;
		};
		
		return;
	};
	
	mpOwner->mnWeaponVolume = LOUD_GUN_VOLUME;
	mpOwner->mnWeaponFlash = BRIGHT_GUN_FLASH;
	
	--mnClip;
	
	mpOwner->AddEffects(EF_MUZZLEFLASH);
	
	// Player "shoot" animation
	mpOwner->SendAnim(PLAYER_ATTACK1);
	
	gpEngine->pfnMakeVectors(mpOwner->self->v_angle + mpOwner->self->punchangle);
	
	idVec3 vSrc{mpOwner->GetGunPosition()};
	idVec3 vAiming{mpOwner->GetAutoAimVector(AUTOAIM_10DEGREES)};
	
	idVec3 vDir{};
	vDir = mpOwner->FireBulletsPlayer(1, vSrc, vAiming, VECTOR_CONE_10DEGREES, 8192, BULLET_PLAYER_357, mpOwner->self, mpOwner->random_seed);
	
	int nFlags{0};
	
#ifdef OPENLAMBDA_CLIENT_WEAPONS
	nFlags = FEV_NOTHOST;
#endif
	
	gpEngine->pfnPlaybackEventFull(nFlags, mpOwner->ToEdict(), mnPythonFireEvent, 0.0, (float*)&g_vecZero, (float*)&g_vecZero, vDir.x, vDir.y, 0, 0, 0, 0);
	
	// HEV suit - indicate out of ammo condition
	if(!mnClip && mpOwner->m_rgAmmo[mnPrimaryAmmoType] <= 0)
		mpOwner->SetSuitUpdate("!HEV_AMO0", false, 0);
	
	mfNextPrimaryAttack = 0.75;
	mfTimeWeaponIdle = UTIL_SharedRandomFloat(mpOwner->random_seed, 10, 15);
};

void CWeaponPython::SecondaryAttack()
{
#ifdef OPENLAMBDA_CLIENT_DLL
	if(!bIsMultiplayer())
#else
	if(!mpGame->GetRules()->IsMultiplayer())
#endif
	{
		return;
	};
	
	if(mpOwner->GetFOV() != 0) // mpOwner->self->fov
	{
		mbInZoom = false;
		mpOwner->SetFOV(0); // mpOwner->mnFOV = 0; // 0 means reset to default FOV
		mpOwner->self->fov = 0;
	}
	else if(mpOwner->GetFOV() != 40)
	{
		mbInZoom = true;
		mpOwner->SetFOV(40); // mpOwner->mnFOV = 40;
		mpOwner->self->fov = 40;
	};
	
	mfNextSecondaryAttack = 0.5;
};

void CWeaponPython::Reload()
{
	if(mpOwner->ammo_357 <= 0)
		return;
	
	if(mpOwner->self->fov != 0)
	{
		mbInZoom = false;
		mpOwner->self->fov = mpOwner->mnFOV = 0; // 0 means reset to default
	};
	
	bool bUseScope{false};
	
#ifdef OPENLAMBDA_CLIENT_DLL
	bUseScope = bIsMultiplayer();
#else
	bUseScope = mpGame->GetRules()->IsMultiplayer;
#endif
	
	DefaultReload(6, PYTHON_RELOAD, 2.0, bUseScope);
};

int CWeaponPython::AddToCharacter(CBaseCharacter *apCharacter)
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

int CWeaponPython::GetItemInfo(ItemInfo *apInfo) const
{
	apInfo->pszName = STRING(self->classname);
	
	apInfo->pszAmmo1 = "357";
	apInfo->iMaxAmmo1 = _357_MAX_CARRY;
	
	apInfo->pszAmmo2 = nullptr;
	apInfo->iMaxAmmo2 = -1;
	
	apInfo->iMaxClip = PYTHON_MAX_CLIP;
	
	apInfo->iSlot = 1;
	apInfo->iPosition = 1;
	apInfo->iFlags = 0;
	
	apInfo->iId = mnID = WEAPON_PYTHON;
	apInfo->iWeight = PYTHON_WEIGHT;
	
	return 1;
};
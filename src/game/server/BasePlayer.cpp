/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-2001 Id Software, Inc.
 * Copyright (C) 2018-2023 BlackPhrase
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
/// @brief player functions/definitions

#include "BasePlayer.hpp"
#include "BaseGame.hpp"
#include "Util.hpp"
#include "IGameRules.hpp"
#include "BaseItem.hpp"

#include <in_buttons.h>

//=============================================================================

unsigned long modelindex_player{0};

void respawn(entvars_t *self);

/*
============
SelectSpawnPoint

Returns the entity to spawn at
============
*/
// TODO: CGameWorld::SelectSpawnPoint?
// TODO: CGameRules::SelectSpawnPoint?
CBaseEntity *SelectSpawnPoint()
{
	int numspots{0}, totalspots{0};
	CBaseEntity *world{ToBaseEntity(gpWorld->GetEntity(0))};

	// testinfo_player_start is only found in regioned levels
	CBaseEntity *spot{gpWorld->FindEntityByString(world, "classname", "testplayerstart")};
	if(spot)
		return spot;
		
	// choose a info_player_deathmatch point

	// ok, find all spots that don't have players nearby

	CBaseEntity *spots{world};
	spot = gpWorld->FindEntityByString(world, "classname", "info_player_deathmatch");       
	while(spot)
	{
		totalspots++;

		CBaseEntity *thing{nullptr};
		thing = gpWorld->FindEntityInSphere(thing, spot->GetOrigin(), 84);
		int pcount{0};               
		while(thing)
		{
			if(thing->GetClassName() == "player")
				pcount++;                      
			thing = gpWorld->FindEntityInSphere(thing, spot->GetOrigin(), 84);
		};
		if(pcount == 0)
		{
			spot->SetGoal(spots);
			spots = spot;
			numspots++;
		};

		// Get the next spot in the chain
		spot = gpWorld->FindEntityByString(spot, "classname", "info_player_deathmatch");                
	};
	
	totalspots--;
	if(!numspots)
	{
		// ack, they are all full, just pick one at random
//		bprint (PRINT_HIGH, "Ackk! All spots are full. Selecting random spawn spot\n");
		totalspots = rint((random() * totalspots));
		spot = gpWorld->FindEntityByString(world, "classname", "info_player_deathmatch");       
		while(totalspots > 0)
		{
			totalspots--;
			spot = gpWorld->FindEntityByString(spot, "classname", "info_player_deathmatch");
		};
		return spot;
	};
	
	// TODO: info_player_start support
	
	// We now have the number of spots available on the map in numspots

	// Generate a random number between 1 and numspots

	numspots--;
	
	numspots = rint((random() * numspots));

	spot = spots;
	while(numspots > 0)
	{
		spot = spot->GetGoalEntity();
		numspots--;
	};
	
	if(!spot)
		return world;
	
	return spot;
};

CBasePlayer::CBasePlayer() : mpGame(gpGame){}

void CBasePlayer::Spawn()
{
	// Clear entity values
	SetClassName("player");
	SetMaxHealth(100);
	SetHealth(GetMaxHealth());
	SetArmorValue(0); // TODO: recheck
	//SetDeadFlag(CBaseEntity::DeadFlag::No); // TODO
	SetDamageable(CBaseEntity::Damageable::Aim);
	SetSolidity(CBaseEntity::Solidity::SlideBox);
	SetMoveType(CBaseEntity::MoveType::Walk);
	//self->show_hostile = 0; // TODO
	SetFlags(FL_CLIENT);
	self->air_finished = gpGlobals->time + 12; // TODO: level.time in q2
	self->dmg = 2; // initial water damage
	
	self->dmg_take = 0;
	self->dmg_save = 0;
	
	SetFriction(1.0);
	SetGravity(1.0);
	
/*
	self->super_damage_finished = 0;
	self->radsuit_finished = 0;
	self->invisible_finished = 0;
	self->invincible_finished = 0;
*/
	SetEffects(0); // TODO: ClearEffects?
	//self->invincible_time = 0; // TODO
	
	//DecodeLevelParms(); // TODO
	
	//W_SetCurrentAmmo(); // TODO

	//self->attack_finished = gpGlobals->time; // TODO
	
	//self->th_pain = CBasePlayer::Pain; // TODO
	//self->th_die = CBasePlayer::Die; // TODO
	
	SetDeadFlag(DEAD_NO);

	// pausetime is set by teleporters to keep the player from moving a while
	//self->pausetime = 0; // TODO
	
	// oh, this is a hack!
	//SetModel("models/eyes.mdl");
	//modelindex_eyes = self->modelindex; // TODO

	SetModel("models/player.mdl");
	modelindex_player = self->modelindex;
	
	SetSequence(LookupActivity(ACT_IDLE));

	SetSize(VEC_HULL_MIN, VEC_HULL_MAX);
	
	mpGame->GetRules()->OnPlayerSpawn(this);
	
	// TODO: Probably should be part of the game rules
	
	CBaseEntity *pSpawnSpot{SelectSpawnPoint()};
	
	SetOrigin(idVec3::Origin);
	idVec3 vAdd{0.0f};
	vAdd[2] = 1.0f;
	SetOrigin(GetOrigin() + vAdd);
	SetOrigin(pSpawnSpot->GetOrigin() + idVec3(0, 0, 1));
	SetAngles(pSpawnSpot->GetAngles());
	self->fixangle = 1; // turn this way immediately
	
	idVec3 view_ofs{0.0f};
	view_ofs[2] = 22;
	VectorCopy(view_ofs, self->view_ofs);

	// Mod - Xian (May.20.97)
	// Bug where player would have velocity from their last kill

	SetVelocity(idVec3::Origin);

	//player_stand1();
	
	//gpEngine->pfnMakeVectors(GetAngles());
	//spawn_tfog(GetOrigin() + gpGlobals->v_forward * 20);

	//spawn_tdeath(GetOrigin(), self);

	// Set Rocket Jump Modifiers
	//if(stof(gpEngine->pfnInfokey(world, "rj")) != 0)           
		//rj = stof(gpEngine->pfnInfokey(world, "rj"));

/*
	if (deathmatch == 4)
	{
		client->v.ammo_shells = 0;
		if (stof(infokey(world, "axe")) == 0)
		{
			client->v.ammo_nails = 255;
			client->v.ammo_shells = 255;
			client->v.ammo_rockets = 255;
			client->v.ammo_cells = 255;
			client->v.items = client->v.items | IT_NAILGUN;
			client->v.items = client->v.items | IT_SUPER_NAILGUN;
			client->v.items = client->v.items | IT_SUPER_SHOTGUN;
			client->v.items = client->v.items | IT_ROCKET_LAUNCHER;
//		client->v.items = client->v.items | IT_GRENADE_LAUNCHER;
			client->v.items = client->v.items | IT_LIGHTNING;
		};
		client->v.items = client->v.items - (client->v.items & (IT_ARMOR1 | IT_ARMOR2 | IT_ARMOR3)) + IT_ARMOR3;
		client->v.armorvalue = 200;
		client->v.armortype = 0.8;
		client->v.health = 250;
		client->v.items = client->v.items | IT_INVULNERABILITY;
		client->v.invincible_time = 1;
		client->v.invincible_finished = time + 3;
	};

	if (deathmatch == 5)
	{
		client->v.ammo_nails = 80;
		client->v.ammo_shells = 30;
		client->v.ammo_rockets = 10;
		client->v.ammo_cells = 30;
		client->v.items = client->v.items | IT_NAILGUN;
		client->v.items = client->v.items | IT_SUPER_NAILGUN;
		client->v.items = client->v.items | IT_SUPER_SHOTGUN;
		client->v.items = client->v.items | IT_ROCKET_LAUNCHER;
		client->v.items = client->v.items | IT_GRENADE_LAUNCHER;
		client->v.items = client->v.items | IT_LIGHTNING;
		client->v.items = client->v.items - (client->v.items & (IT_ARMOR1 | IT_ARMOR2 | IT_ARMOR3)) + IT_ARMOR3;
		client->v.armorvalue = 200;
		client->v.armortype = 0.8;
		client->v.health = 200;
		client->v.items = client->v.items | IT_INVULNERABILITY;
		client->v.invincible_time = 1;
		client->v.invincible_finished = time + 3;
	};
*/
};

int CBasePlayer::Save(CGameSave &aGameSave)
{
	if(!CBaseCharacter::Save(aGameSave))
		return 0;
	
	return aGameSave.WriteFields("PLAYER", this, mSaveData, ARRAYSIZE(mSaveData));
};

int CBasePlayer::Restore(const CGameSave &aGameSave)
{
	return 0;
};

int CBasePlayer::TakeDamage(CBaseEntity *apInflictor, CBaseEntity *apAttacker, float afDamage, int anDmgTypeBitSum)
{
	// Already ded :P
	if(!IsAlive())
		return 0;
	
	if(!mpGame->GetRules()->PlayerCanTakeDamage(this, aAttacker)) // TODO: this also can be moved inside the game rules' PlayerTakeDamage method
		return 0;
	
	float fDmgTaken{0.0f};
	
	fDmgTaken = mpGame->GetRules()->PlayerTakeDamage(this, apInflictor, apAttacker, afDamage, anDmgTypeBitSum);
	
	return fDmgTaken;
};

void CBasePlayer::RemoveAllItems(int anFlags)
{
};

void CBasePlayer::Killed(CBaseEntity *apAttacker, CBaseEntity *apLastInflictor, int anGib)
{
	// Holster the current weapon immediately to allow it to cleanup
	if(mpActiveItem)
		mpActiveItem->Holster();
	
	mpGame->GetRules()->OnPlayerKilled(this, apAttacker, apLastInflictor);
	
	// Turret
	if(mpTank)
	{
		mpTank->Use(this, this, CBaseEntity::UseType::Off, 0);
		mpTank = nullptr;
	};
	
	// TODO: sound ent
	// This client isn't going to be thinking for a while, so reset the sound until they respawn
	auto pSound{CSoundEnt::SoundPointerForIndex(CSoundEnt::ClientSoundIndex(ToEdict()))};
	if(pSound)
		pSound->Reset();
	
	SetAnimation(PLAYER_DIE);
	
	mnRespawnFrames = 0;
	
	// Don't use eyes
	SetModelIndex(gnModelIndexPlayer);
	
	SetDeadFlag(DEAD_DYING);
	SetMoveType(CBaseEntity::MoveType::Toss);
	
	RemoveFlags(FL_ONGROUND);
	
	if(GetVelocity().z < 10)
		AddVelocity(idVec3(0, 0, gpEngine->pfnRandomFloat(0, 300)));
	
	// TODO: broadcast the player killed event here to let HEV suit emit its sounds, for example
	
	// Clear out the suit message cache so we don't keep chattering
	//SetSuitUpdate(nullptr, false, 0); // TODO
	
	mnClientHealth = 0;
	
	// Send "health" update message
	gpEngine->pfnMessageBegin(MSG_ONE, gMsgHealth, nullptr, self);
	gpEngine->pfnWriteByte(mnClientHealth);
	gpEngine->pfnMessageEnd();
	
	// Tell Ammo HUD that the player is dead
	gpEngine->pfnMessageBegin(MSG_ONE, gMsgCurWeapon, nullptr, self);
	gpEngine->pfnWriteByte(0);
	gpEngine->pfnWriteByte(0xFF);
	gpEngine->pfnWriteByte(0xFF);
	gpEngine->pfnMessageEnd();
	
	// Reset FOV
	self->fov = mnFOV = mnClientFOV = 0;
	
	gpEngine->pfnMessageBegin(MSG_ONE, gMsgSetFOV, nullptr, self);
	gpEngine->pfnWriteByte(mnClientFOV);
	gpEngine->pfnMessageEnd();
	
	// UNDONE: Put this is, but add FFADE_PERMANENT and make fade time 8.8 instead of 4.12
	//UTIL_ScreenFade(ToEdict(), idVec3(128, 0, 0), 6, 15, 255, FFADE_OUT | FFADE_MODULATE);
	
	if((GetHealth() < -40 && anGib != GIB_NEVER) || anGib == GIB_ALWAYS)
	{
		SetSolidity(CBaseEntity::Solidity::None);
		GibMonster(); // This clears self->model
		AddEffects(EF_NODRAW);
		return;
	};
	
	DeathSound();
	
	GetAngles().x = 0;
	GetAngles().z = 0;
	
	SetThinkCallback(CBasePlayer::DeathThink);
	SetNextThink(gpGlobals->time + 0.1);
};

void CBasePlayer::SetAnimation(eAnimType aeAnimType)
{
	float fSpeed{GetVelocity.GetLength2D()};
	
	if(HasFlags(FL_FROZEN))
	{
		fSpeed = 0.0f;
		aeAnimType = PLAYER_IDLE;
	};
	
	switch(aeAnimType)
	{
	case PLAYER_JUMP:
		meIdealActivity = ACT_HOP;
		break;
	case PLAYER_SUPERJUMP:
		meIdealActivity = ACT_LEAP;
		break;
	case PLAYER_DIE:
		meIdealActivity = GetDeathActivity();
		break;
	case PLAYER_ATTACK1:
		switch(meActivity)
		{
		case ACT_HOVER:
		case ACT_SWIM:
		case ACT_HOP:
		case ACT_LEAP:
		case ACT_DIESIMPLE:
			meIdealActivity = meActivity;
			break;
		default:
			meIdealActivity = ACT_RANGE_ATTACK1;
			break;
		};
		break;
	case PLAYER_IDLE:
	case PLAYER_WALK:
		break;
	};
	
	switch(meIdealActivity)
	{
	case ACT_HOVER:
	case ACT_LEAP:
	case ACT_SWIM:
	case ACT_HOP:
	case ACT_DIESIMPLE:
	default:
		if(meActivity == meIdealActivity)
			return;
		
		meActivity = meIdealActivity;
		
		eAnimDesired = LookupActivity(meActivity);
		
		// Already using the desired animation?
		if(GetSequence() == eAnimDesired)
			return;
		
		self->gaitsequence = 0;
		SetSequence(eAnimDesired);
		SetFrame(0);
		ResetSequenceInfo();
		return;
	case ACT_RANGE_ATTACK1:
		break;
	case ACT_WALK:
		eAnimDesired = GetSequence();
		
		if(meActivity != ACT_RANGE_ATTACK1 || mfSequenceFinished)
		{
			if(HasFlags(FL_DUCKING))
				strcpy(sAnim, "crouch_aim_")
			else
				strcpy(sAnim, "ref_aim_");
			strcat(sAnim, msAnimExtension);
			eAnimDesired = LookupActivity(sAnim);
			if(eAnimDesired == -1)
				eAnimDesired = 0;
			meActivity = ACT_WALK;
		};
		
		break;
	};
	
	if(HasFlags(FL_DUCKING))
	{
		if(fSpeed == 0.0f)
			self->gaitsequence = LookupActivity(ACT_CROUCHIDLE);
		else
			self->gaitsequence = LookupActivity(ACT_CROUCH);
	}
	else if(fSpeed > 220.0f)
		self->gaitsequence = LookupActivity(ACT_RUN);
	else if(fSpeed > 0.0f)
		self->gaitsequence = LookupActivity(ACT_WALK);
	else
		//self->gaitsequence = LookupActivity(ACT_WALK);
		self->gaitsequence = LookupSequence("deep_idle");
	
	// Already using the desired animation?
	if(GetSequence() == eAnimDesired)
		return;
	
	//ALERT();
	
	// Reset to first frame of desired animation
	SetSequence(eAnimDesired); //self->sequence
	SetFrame(0); //self->frame // TODO: SetAnimFrame?
	ResetSequenceInfo();
};

void CBasePlayer::PreThink()
{
	int nButtonsChanged = m_afButtonLast ^ self->button;
	
	m_afButtonPressed = nButtonsChanged & self->button;
	m_afButtonReleased = nButtonsChanged & ~self->button;
	
	mpGame->GetRules()->OnPlayerThink(this);
	
	if(mpGame->IsOver())
		return; // Intermission or finale
	
	gpEngine->pfnMakeVectors(self->v_angle); // TODO: Is this still used?
	
	ItemPreFrame();
	WaterMove();
	
	if(mpGame()->GetRules() && mpGame->GetRules()->IsFlashlightAllowed())
		mnHideHUD &= ~HIDEHUD_FLASHLIGHT;
	else
		mnHideHUD |= HIDEHUD_FLASHLIGHT;
	
	// Checks if new client data (for HUD and view control) needs to be sent to the client
	UpdateClientData();
	
	CheckTimeBasedDamage();
	
	CheckSuitUpdate(); // TODO: move to HLPlayer
	
	// Observer button handling
	if(IsObserver())
	{
		Observer_HandleButtons();
		Observer_CheckTarget();
		Observer_CheckProperties();
		self->impulse = 0;
		return;
	};
	
	if(GetDeadFlag() >= DEAD_DYING)
	{
		DeathThink();
		return;
	};
	
	// So the correct flags get sent to client ASAP
	if(m_afPhysicsFlags & PFLAG_ONTRAIN)
		AddFlags(FL_ONTRAIN);
	else
		RemoveFlags(FL_ONTRAIN);
	
	// Train speed control
	if(m_afPhysicsFlags & PFLAG_ONTRAIN)
	{
		auto pTrain{CBaseEntity::Instance(GetGroundEntity())};
		float fVel;
		
		if(!pTrain)
		{
			if()
				pTrain =;
			
			if()
			{
				return;
			};
		}
		else if()
		{
			return;
		};
		
		SetVelocity(idVec3::Origin);
		fVel = 0.0f;
		
		if(m_afButtonPressed & IN_FORWARD)
		{
			fVel = 1.0f;
			pTrain->Use(this, this, CBaseEntity::UseType::Set, fVel);
		}
		else if(m_afButtonPressed & IN_BACK)
		{
			fVel = -1.0f;
			pTrain->Use(this, this, CBaseEntity::UseType::Set, fVel);
		};
		
		if(fVel)
		{
			mnTrain = GetTrainSpeed(pTrain->GetSpeed(), pTrain->GetImpulse());
			mnTrain |= TRAIN_ACTIVE | TRAIN_NEW;
		};
	}
	else if(mnTrain & TRAIN_ACTIVE)
		mnTrain = TRAIN_NEW; // Turn off train
	
	// If on a ladder, jump off it, else jump
	if(self->button & IN_JUMP)
		Jump();
	
	// If trying to duck, already ducked or in in the process of ducking
	if(self->button & IN_DUCK || HasFlags(FL_DUCKING) || m_afPhysicsFlags & PFLAG_DUCKING)
		Duck();
	
	if(!HasFlags(FL_ONGROUND))
		mfFallVelocity = -GetVelocity().z;
	
	//StudioFrameAdvance();
	
	// Clear out ladder pointer
	mhEnemy = nullptr;
	
	if(m_afPhysicsFlags & PFLAG_ONBARNACLE)
		SetVelocity(idVec3::Origin);
};

void CBasePlayer::PostThink()
{
	// If NOT in intermission or finale and the player is ALIVE then do the following things
	if(!mpGame->IsOver() && IsAlive())
	{
		// Handle tank controlling
		if(mpTank)
		{
			// If they've moved too far from the gun or selected a weapon, unuse the gun
			if(mpTank->OnControls(self) && !self->weaponmodel)
				mpTank->Use(this, this, CBaseEntity::UseType::Set, 2); // Try fire the gun
			else
			{
				// They've moved off the platform
				mpTank->Use(this, this, CBaseEntity::UseType::Off, 0);
				mpTank = nullptr;
			};
		};
		
		// Do weapon stuff
		ItemPostFrame();
		
		// Check to see if player landed hard enough to make a sound
		
		if()
		{
		};
		
		if(HasFlags(FL_ONGROUND))
		{
			if(mfFallVelocity > 64.0f && !mpGame->GetRules()->IsMultiplayer())
			{
				CSoundEnt::InsertSound(bits_SOUND_PLAYER, GetOrigin(), mfFallVelocity, 0.2f);
				//ALERT(at_console, "fall %f\n", mfFallVelocity);
			};
			mfFallVelocity = 0.0f;
		};
		
		// Select the proper animation for the player character
		if(IsAlive())
		{
			if(!GetVelocity().x && !GetVelocity().y)
				SetAnimation(PLAYER_IDLE);
			else if((GetVelocity().x || GetVelocity().y) && HasFlags(FL_ONGROUND))
				SetAnimation(PLAYER_WALK);
			else if(GetWaterLevel() > 1)
				SetAnimation(PLAYER_WALK);
		};
		
		StudioFrameAdvance(); // TODO: mpAnimator->Update()?
		CheckPowerups(self);
		
		UpdatePlayerSound();
	};
	
#ifdef CLIENT_WEAPONS
	// Decay timers on weapons
	// Go through all of the weapons and make a list of the ones to pack
	for(int i = 0; i < MAX_ITEM_TYPES; ++i)
	{
		if(m_rgpPlayerItems[i])
		{
			auto pPlayerItem{m_rgpPlayerItems[i]};
			
			while(pPlayerItem)
			{
				auto pGun{reinterpret_cast<CBaseWeapon*>(pPlayerItem->GetWeaponPtr())};
				
				if(pGun && pGun->UseDecrement())
				{
					pGun->mfNextPrimaryAttack = std::max();
					pGun->mfNextSecondaryAttack = std::max();
					
					if()
						pGun->mfTimeWeaponIdle = std::max();
					
					if()
						pGun->self->fuser1 = std::max();
				};
				
				pPlayerItem = pPlayerItem->mpNext;
			};
		};
	};
	
	mfNextAttack -= gpGlobals->frametime;
	
	if(mfNextAttack < -0.001)
		mfNextAttack = -0.001;
	
	if(mfNextAmmoBurn != 1000)
	{
		mfNextAmmoBurn -= gpGlobals->frametime;
		
		if(mfNextAmmoBurn < -0.001)
			mfNextAmmoBurn = -0.001;
	};
	
	if(mfAmmoStartCharge != 1000)
	{
		mfAmmoStartCharge -= gpGlobals->frametime;
		
		if(mfAmmoStartCharge < -0.001)
			mfAmmoStartCharge = -0.001;
	};
#endif // CLIENT_WEAPONS
	
	// Track button info so we can detect "pressed" and "released" buttons next frame
	m_afButtonLast = self->button;
};

void CBasePlayer::Duck()
{
	if((self->button & IN_DUCK) && GetIdealActivity() != ActivityType::Leap)
		SetAnimation(PLAYER_WALK);
};

void CBasePlayer::UpdateClientData()
{
	// TODO
};

void CBasePlayer::SelectNextItem(int anItemID)
{
};

void CBasePlayer::SelectPrevItem(int anItemID)
{
	// NOTE: nothing
};

void CBasePlayer::SelectItem(const char *asItem)
{
};

void CBasePlayer::SelectLastItem()
{
};

void CBasePlayer::GiveNamedItem(const char *asName)
{
};

void CBasePlayer::FlashlightToggle()
{
	if(FlashlightIsOn())
		FlashlightTurnOff();
	else
		FlashlightTurnOn();
};

void CBasePlayer::HandleImpulseCommands()
{
	PlayerUse();
	
	switch(self->impulse)
	{
	case 99: // Spray paint
		// TODO
		break;
	case 100: // "Temporary" flashlight "for level designers"
		FlashlightToggle();
		break;
	case 201:
		// TODO
		break;
	default:
		HandleCheatImpulse(self->impulse);
		break;
	};
	
	self->impulse = 0;
};

void CBasePlayer::HandleCheatImpulse(int anImpulse)
{
	switch(anImpulse)
	{
	};
};

bool CBasePlayer::AddItem(const CBaseItem &aItem)
{
	return false;
};

bool CBasePlayer::RemoveItem(const CBaseItem &aItem)
{
	return false;
};

void CBasePlayer::ItemPreFrame()
{
	if(!CanAttack()) // Checks for next attack time
		return;
	
	if(mpActiveItem)
		mpActiveItem->ItemPreFrame();
};

void CBasePlayer::ItemPostFrame()
{
	if(IsUsingTurret())
		return;
	
	if(!CanAttack()) // Checks for next attack time
		return;
	
	HandleImpulseCommands();
	
	if(mpActiveItem)
		mpActiveItem->ItemPostFrame();
};

void CBasePlayer::SetViewEnt(CBaseEntity *apEntity)
{
	// TODO: Next API support
	gpEngine->pfnSetView(ToEdict(), apEntity->ToEdict());
};

void CBasePlayer::FadeVolume(int anFadePercent, int anFadeOutSecs, int anHoldTime, int anFadeInSecs)
{
	// TODO: Next API support
	gpEngine->pfnFadeClientVolume(ToEdict(), anFadePercent, anFadeOutSecs, anHoldTime, anFadeInSecs);
};

void CBasePlayer::SetMaxSpeed(float afSpeed)
{
	// TODO: Next API support
	gpEngine->pfnSetClientMaxspeed(ToEdict(), afSpeed);
};

void CBasePlayer::Pain()
{
	auto nRandomSound{gpEngine->pfnRandomLong(0, 2)};
	
	EmitSound(CHAN_VOICE, va("player/pl_pain%d.wav", 5 + nRandomSound), 1, ATTN_NORM);
};

void CBasePlayer::DeathSound()
{
	auto sSoundPrefix{""}; // TODO: make internal?
	
	if(GetSex() == CBaseCharacter::Sex::Female)
		sSoundPrefix = "f_";
	
	auto nRandomSound{gpEngine->pfnRandomLong(1, 3)};
	
	EmitSound(CHAN_VOICE, va("player/%sdie%d.wav", sSoundPrefix, nRandomSound), 1, ATTN_NORM);
	// TODO: EmitSound(CHAN_VOICE, va("%s/%sdie%d.wav", GetClassName(), sSoundPrefix, nRandomSound), 1, ATTN_NORM); ?
};

int CBasePlayer::TakeHealth(float afValue, int anDmgTypeBitSum)
{
	return CBaseCharacter::TakeHealth(afValue, anDmgTypeBitSum); // TODO: CBaseMonster?
};

const idVec3 &CBasePlayer::GetGunPosition() const
{
	idVec3 vOrigin{GetOrigin() + GetViewOfs()};
	return vOrigin;
};

void CBasePlayer::TraceAttack(CBaseEntity *apAttacker, float afDamage, const idVec3 &avDir, TraceResult *apTraceResult, int anDmgTypeBitSum)
{
	if(!IsDamageable())
		return;
	
	mLastHitGroup = apTraceResult->iHitgroup;
	
	switch(apTraceResult->iHitgroup)
	{
	case HITGROUP_HEAD:
		afDamage *= gSkillData.plrHead;
		break;
	case HITGROUP_CHEST:
		afDamage *= gSkillData.plrChest;
		break;
	case HITGROUP_STOMACH:
		afDamage *= gSkillData.plrStomach;
		break;
	case HITGROUP_LEFTARM:
	case HITGROUP_RIGHTARM:
		afDamage *= gSkillData.plrArm;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		afDamage *= gSkillData.plrLeg;
		break;
	case HITGROUP_GENERIC:
	default:
		break;
	};
	
	SpawnBlood(apTraceResult->vecEndPos, GetBloodType(), afDamage); // A little surface blood
	TraceBleed(afDamage, avDir, apTraceResult, anDmgTypeBitSum);
	AddMultiDamage(apAttacker, this, afDamage, anDmgTypeBitSum);
};

//int CBasePlayer::TakeDamage()

void CBasePlayer::PackDeadPlayerItems()
{
};

//void CBasePlayer::RemoveAllItems(int anFlags)

//void CBasePlayer::Killed()

//void CBasePlayer::SetAnimation()

void CBasePlayer::TabulateAmmo()
{
};

void CBasePlayer::WaterMove()
{
	// TODO
};

// true is the player is attached to a ladder
bool CBasePlayer::IsOnLadder() const
{
	return GetMoveType() == CBaseEntity::MoveType::Fly; // TODO: Ladder in src
};

void CBasePlayer::DeathThink() // TODO: PlayerDeathThink?
{
	StopAnimation();
	
	respawn(self);
	SetNextThink(-1);
};

void CBasePlayer::StartDeathCam()
{
};

void CBasePlayer::StartObserver(const idVec3 &avPosition, const idVec3 &avViewAngle)
{
	// Clear any client-side entities attached to this player
	gpEngine->pfnMessageBegin(MSG_PAS, SVC_TEMPENTITY, GetOrigin());
	gpEngine->pfnWriteByte(TE_KILLPLAYERATTACHMENTS);
	gpEngine->pfnWriteByte((BYTE)gpEngine->pfnGetEntIndex(ToEdict()));
	gpEngine->pfnMessageEnd();
	
	// Holster the weapon immediately to allow it to cleanup
	if(mpActiveItem)
		mpActiveItem->Holster();
	
	if(mpTank)
	{
		mpTank->Use(this, this, UseType::Off, 0);
		mpTank = nullptr;
	};
	
	// Clear out the suit message cache so we don't keep chattering
	SetSuitUpdate(nullptr, false, 0);
	
	// Tell Ammo HUD that the player is dead
	gpEngine->pfnMessageBegin(MSG_ONE, gMsgCurWeapon, nullptr, self);
	gpEngine->pfnWriteByte(0);
	gpEngine->pfnWriteByte(0xFF);
	gpEngine->pfnWriteByte(0xFF);
	gpEngine->pfnMessageEnd();
	
	// Reset FOV
	mnFOV = mnClientFOV = 0;
	self->fov = mnFOV;
	
	gpEngine->pfnMessageBegin(MSG_ONE, gMsgSetFOV, nullptr, self);
	gpEngine->pfnWriteByte(0);
	gpEngine->pfnMessageEnd();
	
	// Setup flags
	mnHideHUD = HIDEHUD_HEALTH | HIDEHUD_WEAPONS;
	m_afPhysicsFlags |= PFLAG_OBSERVER;
	
	SetEffects(EF_NODRAW);
	self->view_ofs = g_vecZero;
	
	SetAngles(avViewAngle);
	self->v_angle = avViewAngle;
	
	self->fixangle = true;
	
	SetSolidity(CBaseEntity::Solidity::None);
	SetDamageable(CBaseEntity::Damageable::Not);
	SetMoveType(CBaseEntity::MoveType::None);
	
	RemovePhysicsFlags(PFLAG_DUCKING); //m_afPhysicsFlags
	RemoveFlags(FL_DUCKING); // TODO: there won't be that flag set as the flags were reset to NODRAW above
	
	SetDeadFlag(DEAD_RESPAWNABLE);
	SetHealth(1);
	
	// Clear out the status bar
	m_fInitHUD = true;
	
	SetTeam(0); //self->team = 0
	
	gpEngine->pfnMessageBegin(MSG_ALL, gMsgTeamInfo);
	gpEngine->pfnWriteByte(gpEngine->pfnGetEntIndex(ToEdict()));
	gpEngine->pfnWriteString("");
	gpEngine->pfnMessageEnd();
	
	// Remove all the player's stuff
	RemoveAllItems(false);
	
	// Move them to the new position
	SetOrigin(avPosition);
	
	// Find a player to watch
	mfNextObserverInput = 0;
	Observer_SetMode(mnObserverLastMode);
};

void CBasePlayer::PlayerUse()
{
	if(IsObserver())
		return;
	
	// Was use pressed or released?
	if()
		return;
	
	// Hit "Use" on a train?
	if(m_afButtonPressed & IN_USE)
	{
		if(mpTank)
		{
			// Stop controlling the tank
			mpTank->Use(this, this, UseType::Off, 0);
			mpTank = nullptr;
			return;
		}
		else
		{
		};
	};
	
	CBaseEntity *pObject{nullptr};
	CBaseEntity *pClosest{nullptr};
	
	idVec3 vLOS;
	float fMaxDot{VIEW_FIELD_NARROW};
	float fDot{0.0f};
	
	gpEngine->pfnMakeVectors(self->v_angle); // So we know which way we are facing
	
	while()
	{
	};
	
	pObject = pClosest;
	
	// Found an object
	if(pObject)
	{
		int nCaps{pObject->GetObjectCaps()};
		
		if()
			EmitSound();
		
		if()
		{
			if()
				m_afPhysicsFlags |= PFLAG_USING;
			
			pObject->Use(this, this, UseType::Set, 1);
		};
	}
	else
		if(m_afButtonPressed & IN_USE)
			EmitSound(CHAN_ITEM, "common/wpn_denyselect.wav", 0.4, ATTN_NORM);
};

void CBasePlayer::Jump()
{
	idVec3 vWallCheckDir; // Direction we're tracing a line to find a wall when walljumping
	idVec3 vAdjustedVelocity;
	idVec3 vSpot;
	TraceResult TraceRslt;
	
	if(HasFlags(FL_WATERJUMP))
		return;
	
	if(GetWaterLevel() >= 2)
		return;
	
	// Jump velocity is sqrt(height * gravity * 2)
	
	// If this isn't the first frame pressing the jump button, break out
	if(!(m_afButtonPressed & IN_JUMP))
		return; // Don't pogo stick
	
	auto pGroundEnt{GetGroundEntity()};
	
	// Is player on ground?
	if(!HasFlags(FL_ONGROUND) || !pGroundEnt())
		return;
	
	// Many features in this function use v_forward, so makevectors now
	gpEngine->pfnMakeVectors(GetAngles());
	
	//RemoveFlags(FL_ONGROUND); // Don't stairwalk
	
	SetAnimation(PLAYER_JUMP);
	
	if(!m_fLongJump && self->button & IN_DUCK && self->flDuckTime > 0 && GetVelocity().GetLength() > 50)
		SetAnimation(PLAYER_SUPERJUMP);
	
	// If you're standing on a conveyor, add it's velocity to yours (for momentum)
	if(pGroundEnt && pGroundEnt->HasFlags(FL_CONVEYOR))
		AddVelocity(GetBaseVelocity()); // self->basevelocity
};

//void CBasePlayer::Duck()

int CBasePlayer::GetClass() const
{
};

void CBasePlayer::AddPoints(int anScore, bool abAllowNegativeScore)
{
	// Positive score always adds
	if(anScore < 0)
	{
		if(!abAllowNegativeScore)
		{
			// Can't go more negative
			if(GetFrags() < 0)
				return;
			
			// Will this go negative?
			if(-anScore > GetFrags())
				anScore = -GetFrags(); // Sum will be 0
		};
	};
	
	AddFrags(anScore);
	
	gpEngine->pfnMessageBegin(MSG_ALL, gMsgScoreInfo);
	gpEngine->pfnWriteByte(gpEngine->pfnGetEntIndex(ToEdict()));
	gpEngine->pfnWriteShort(GetFrags());
	gpEngine->pfnWriteShort(mnDeaths);
	gpEngine->pfnWriteShort(0);
	gpEngine->pfnWriteShort(mpGame->GetRules()->GetTeamIndex(msTeamName) + 1);
	gpEngine->pfnMessageEnd();
};

void CBasePlayer::AddPointsToTeam(int anScore, bool abAllowNegativeScore)
{
	int nIndex{};
	
	for(int i = 1; i <= gpGlobals->maxClients; ++i)
	{
		auto pPlayer{};
		
		if()
		{
		};
	};
};

void CBasePlayer::InitStatusBar()
{
	mfStatusBarDisappearDelay = 0.0f;
	msStatusBarString1[0] = msStatusBarString0[0] = 0;
};

void CBasePlayer::UpdateStatusBar()
{
};

//void CBasePlayer::PreThink()

void CBasePlayer::CheckTimeBasedDamage()
{
};

void CBasePlayer::UpdatePlayerSound()
{
};

//void CBasePlayer::PostThink()

//void CBasePlayer::Spawn()

void CBasePlayer::Precache()
{
};

int CBasePlayer::Save()
{
};

void CBasePlayer::RenewItems()
{
	// Nothing
};

int CBasePlayer::Restore()

//void CBasePlayer::SelectNextItem()

//void CBasePlayer::SelectItem()

//void CBasePlayer::SelectLastItem()

// TODO: HasAnyWeapons?
bool CBasePlayer::HasWeapons() const
{
	for(int i = 0; i < MAX_ITEM_TYPES; ++i)
		if(m_rgpPlayerItems[i])
			return true;
	
	return false;
};

const char *CBasePlayer::GetTeamID() const
{
	// Not fully connected yet
	if(!self)
		return "";
	
	// Return my team name
	return msTeamName;
};

//void CBasePlayer::GiveNamedItem

bool CBasePlayer::FlashlightIsOn()
{
	return HasEffects(EF_DIMLIGHT);
};

void CBasePlayer::FlashlightTurnOn()
{
	if(!mpGame->GetRules()->IsFlashlightAllowed())
		return;
	
	//if(self->weapons & (1 << WEAPON_SUIT))
	if(HasWeapon(WEAPON_SUIT)) // TODO
	{
		EmitSoundDyn(CHAN_WEAPON, SOUND_FLASHLIGHT_ON, 1.0, ATTN_NORM, 0, PITCH_NORM);
		
		AddEffects(EF_DIMLIGHT);
		
		gpEngine->pfnMessageBegin(MSG_ONE, gMsgFlashlight, nullptr, self);
		gpEngine->pfnWriteByte(1);
		gpEngine->pfnWriteByte(mnFlashBattery);
		gpEngine->pfnMessageEnd();
		
		mfFlashLightTime = gpGlobals->time + FLASH_DRAIN_TIME;
	};
};

void CBasePlayer::FlashlightTurnOff()
{
	EmitSoundDyn(CHAN_WEAPON, SOUND_FLASHLIGHT_OFF, 1.0, ATTN_NORM, 0, PITCH_NORM);
	
	RemoveEffects(EF_DIMLIGHT);
	
	gpEngine->pfnMessageBegin(MSG_ONE, gMsgFlashlight, nullptr, self);
	gpEngine->pfnWriteByte(0);
	gpEngine->pfnWriteByte(mnFlashBattery);
	gpEngine->pfnMessageEnd();
	
	mfFlashLightTime = gpGlobals->time + FLASH_CHARGE_TIME;
};

void CBasePlayer::ForceClientDllUpdate()
{
};

//void CBasePlayer::HandleImpulseCommands()

//void CBasePlayer::HandleCheatImpulse()

//int CBasePlayer::AddItem()

//int CBasePlayer::RemoveItem()

int CBasePlayer::GiveAmmo(int anCount, const char *asName, int anMax)
{
	// No ammo
	if(!asName)
		return -1;
	
	// Game rules say I can't have any more of this ammo type
	if(!mpGame->GetRules()->CanHaveAmmo(this, asName, anMax))
		return -1;
	
	int i = GetAmmoIndex(asName);
	
	if(i < 0 || i >= MAX_AMMO_SLOTS)
		return -1;
	
	int nAdd{std::min(anCount, anMax - m_rgAmmo[i])};
	
	if(nAdd < 1)
		return i;
	
	m_rgAmmo[i] += nAdd;
	
	// Make sure the ammo messages have been linked first
	if(gMsgAmmoPickup)
	{
		// Send the message that ammo has been picked up
		gpEngine->pfnMessageBegin(MSG_ONE, gMsgAmmoPickup, nullptr, self);
		gpEngine->pfnWriteByte(GetAmmoIndex(asName)); // Ammo ID
		gpEngine->pfnWriteByte(nAdd); // Amount
		gpEngine->pfnMessageEnd();
	};
	
	TabulateAmmo();
	return i;
};

//void CBasePlayer::ItemPreFrame()
//void CBasePlayer::ItemPostFrame()

int CBasePlayer::GetAmmoInventory(int anAmmoSlot) const
{
	if(anAmmoSlot == -1)
		return -1;
	
	return m_rgAmmo[anAmmoSlot];
};

// TODO: GetAmmoSlot? GetAmmoSlotIndex?
int CBasePlayer::GetAmmoIndex(const char *asAmmo) const
{
	if(!asAmmo || !*asAmmo)
		return -1;
	
	for(int i = 1; i < MAX_AMMO_SLOTS; ++i)
	{
	};
	
	return -1;
};

void CBasePlayer::SendAmmoUpdate()
{
};

//void CBasePlayer::UpdateClientData

bool CBasePlayer::OnBecomeProne() // TODO: HasBecomeProne?
{
	return true;
};

int CBasePlayer::GetIlum() const
{
};

void CBasePlayer::EnableControl(bool abState)
{
	if(abState)
		RemoveFlags(FL_FROZEN);
	else
		AddFlags(FL_FROZEN);
};

const idVec3 &CBasePlayer::GetAutoAimVector(float afDelta) const
{
};

const idVec3 &CBasePlayer::GetAutoAimDeflection()
{
};

void CBasePlayer::ResetAutoAim()
{
	if(mvAutoAim.x != 0 || mvAutoAim.y != 0)
	{
		mvAutoAim = idVec3(0);
		SetCrosshairAngle(0, 0);
	};
	
	mbOnTarget = false;
};

void CBasePlayer::SetCustomDecalFrames(int anFrames)
{
	mnCustomSprayFrames = -1;
	
	if(anFrames > 0 && anFrames < 8)
		mnCustomSprayFrames = anFrames;
};

int CBasePlayer::GetCustomDecalFrames() const
{
	return mnCustomSprayFrames;
};

void CBasePlayer::DropItem(const char *asItemName)
{
};

bool CBasePlayer::HasItem(CBaseItem *apItem) const
{
	auto pItem{m_rgpPlayerItems[apItem->iItemSlot()]};
	
	while(pItem)
	{
		if(!strcmp(apItem->GetClassName(), pItem->GetClassName()))
			return true;
		
		pItem = pItem->m_pNext;
	};
	
	return false;
};

bool CBasePlayer::HasNamedItem(const char *asItemName) const
{
	CBaseItem *pItem{nullptr};
	
	for(int i = 0; i < MAX_ITEM_TYPES; ++i)
	{
		pItem = m_rgpPlayerItems[i]; // TODO: GetItem(i)?
		
		while(pItem)
		{
			if(!strcmp(asItemName, pItem->GetClassName()))
				return true;
			
			pItem = pItem->m_pNext;
		};
	};
	
	return false;
};

bool CBasePlayer::SwitchWeapon(CBaseItem *apWeapon)
{
	if(!apWeapon->CanDeploy())
		return false;
	
	ResetAutoAim();
	
	if(mpActiveItem)
	{
		mpActiveItem->Holster();
		// TODO: wait for anim completion?
	};
	
	mpActiveItem = apWeapon;
	apWeapon->Deploy();
	
	return true;
};
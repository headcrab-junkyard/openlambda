/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2021 BlackPhrase
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

#include "BasePlayer.hpp"

void CBasePlayer::Precache(){}

void CBasePlayer::Jump(){}
void CBasePlayer::Duck(){}

void CBasePlayer::PreThink(){}
void CBasePlayer::PostThink(){}

void CBasePlayer::ItemPreFrame(){}
void CBasePlayer::ItemPostFrame(){}

int CBasePlayer::Save(CSave &aSave){return 0;}
int CBasePlayer::Restore(CRestore &aRestore){return 0;}

void CBasePlayer::DeathSound(){}

void CBasePlayer::WaterMove(){}

void CBasePlayer::SetAnimation(PlayerAnim aeAnim){}

void CBasePlayer::PlayerDeathThink(){}

void CBasePlayer::StartDeathCam(){}

void CBasePlayer::PlayerUse(){}

bool CBasePlayer::IsOnLadder() const {return false;}

void CBasePlayer::RemoveAllItems(bool abRemoveSuit){}

void CBasePlayer::UpdatePlayerSound(){}

void CBasePlayer::RenewItems(){}

int CBasePlayer::Classify() const {return 0;}

void CBasePlayer::HandleImpulseCommands(){}
void CBasePlayer::HandleCheatImpulse(int anImpulse){}

int CBasePlayer::AddPlayerItem(CBaseItem *apItem){return false;}
int CBasePlayer::RemovePlayerItem(CBaseItem *apItem){return false;}
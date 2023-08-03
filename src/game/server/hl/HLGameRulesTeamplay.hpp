/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2023 BlackPhrase
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
/// @brief multiplayer with teams game rules

#pragma once

#include "HLGameRulesMultiplayer.hpp"

// CHalfLifeTeamplay
// TODO: CTeamplayGameRules?
class CHLGameRulesTeamplay : public CHLGameRulesMultiplayer
{
public:
	CHLGameRulesTeamplay();
	virtual ~CHLGameRulesTeamplay() = default;
	
	virtual void Think() override;
	
	// This is the game name that player will see in the server browser
	virtual const char *GetGameDescription() const {return "HL Teamplay";}
	
	// Methods to verify the single/multiplayer status of the game
	virtual bool IsTeamplay() const override;
	
	virtual void OnClientUserInfoChanged(CBasePlayer *apPlayer, char *asInfoBuffer) override;
	
	// The client game module is ready for updating
	virtual void InitPlayerHUD(CBasePlayer *apPlayer) override;
	
	// The player needs to be informed about the current game mode
	virtual void UpdateGameMode(CBasePlayer *apPlayer) override; // TODO: UpdateGameModeForPlayer?
	
	// Player damage rules
	virtual bool IsPlayerAllowedToDamage(CBasePlayer *apPlayer, CBaseEntity *apOther) const override; // FPlayerCanTakeDamage(CBasePlayer *apPlayer, CBaseEntity *apAttacker); // was From?
	
	virtual bool ShouldAutoAim(CBasePlayer *apPlayer, CBaseEntity *apTarget) const override;
	
	virtual bool HandleClientCommand(CBasePlayer *apPlayer, const ICmdArgs &aCmd) override;
	
	// Player kills/scoring
	virtual int GetPointsForKill(CBasePlayer *apAttacker, CBasePlayer *apKilled) const override; // TODO: victim, killer?
	
	virtual void OnPlayerKilled(CBasePlayer *apVictim, CBaseEntity *apKiller, CBaseEntity *apInflictor) override;
	
	virtual void DeathNotice(CBasePlayer *apVictim, CBaseEntity *apKiller, CBaseEntity *apInflictor) override;
	
	// Teamplay stuff
	virtual const char *GetTeamID(CBaseEntity *apEntity) const override {return "";} // TODO: make CBaseEntity::GetTeamID?
	virtual int GetPlayerRelationship(CBasePlayer *apPlayer, CBaseEntity *apTarget) const override;
	
	virtual int GetTeamIndex(const char *asTeamName) const;
	virtual const char *GetIndexedTeamName(int anTeamID) const;
	virtual bool IsValidTeam(const char *asTeamName) const;
	
	const char *SetDefaultPlayerTeam(CBasePlayer *apPlayer) override;
	virtual void ChangePlayerTeam(CBasePlayer *apPlayer, const char *asTeamName, bool abKill, bool abGib) override;
private:
	void RecountTeams(bool abResendInfo = false);
	
	const char *GetTeamWithFewestPlayers() const;
private:
	constexpr auto MaxTeams{32};
	constexpr auto MaxTeamNameLength{16};
	constexpr auto TeamListLength{MaxTeams * MaxTeamNameLength};
	
	char msTeamList[TeamListLength]{};
	
	/// This means the server set only some teams as valid
	bool mbTeamLimit{false};
	
	bool mbDisableDeathPenalty{false};
	bool mbDisableDeathMessages{false};
};
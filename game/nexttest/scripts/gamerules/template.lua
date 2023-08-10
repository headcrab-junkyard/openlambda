-- Template Game Rules Script Example

-- Module table
local M = {}

function M.Think()
end

function M.IsMultiplayer()
	return false
end

function M.IsDeathmatch()
	return false
end

function M.IsTeamplay()
	return false
end

function M.IsCoOp()
	return false
end

function M.GetGameDescription()
	return "Template"
end

function M.IsAllowedToSpawn(entity)
	return true
end

function M.IsFlashlightAllowed()
	return true
end

function M.ShouldSwitchWeapon(player, weapon)
	return true
end

function M.GetNextBestWeapon(player, weapon)
	return true
end

function M.OnClientConnected(id, name, addr, rejectreason)
	return true
end

function M.OnClientDisconnect(id)
end

function M.InitPlayerHUD(player)
end

function M.UpdateGameMode(player)
end

function M.GetPlayerFallDamage(player)
	return 0
end

function M.CanPlayerTakeDamage(player, attacker)
	return true
end

function M.CanPlayerSpawn(player)
	return true
end

function M.OnPlayerSpawn(player)
end

function M.OnPlayerThink(player)
end

function M.CanPlayerRespawn(player)
	return true
end

function M.GetPlayerSpawnTime(player)
	return 0 -- TODO: Game.GetCurTime()
end

function M.GetPlayerSpawnSpot(player)
	return nil
end

function M.ShouldAutoAim(player, target)
	return true
end

function M.IsAutoTargetCrosshairAllowed()
	return true
end

function M.OnClientCommand(client, args)
	return false
end

function M.OnClientUserInfoChanged(client, oldinfo)
end

function M.GetPointsForKill(killer, victim)
	return 1
end

function M.OnPlayerKilled(player, killer, inflictor)
end

function M.DeathNotice(victim, killer, inflictor)
end

function M.CanPlayerHaveWeapon(player, weapon)
	return true
end

function M.OnPlayerGotWeapon(player, weapon)
end

function M.ShouldWeaponRespawn(weapon)
	return false
end

function M.GetWeaponRespawnTime(weapon)
	return 0 -- Game.GetCurTime()
end

function M.TryRespawnWeapon(weapon)
	return 0
end

function M.GetWeaponRespawnSpot(weapon)
	return weapon.GetOrigin()
end

function M.CanPlayerHaveItem(player, item)
	return true
end

function M.OnPlayerGotItem(player, item)
end

function M.ShouldItemRespawn(item)
	return false
end

function M.GetItemRespawnTime(item)
	return 0
end

function M.GetItemRespawnSpot(item)
	return item.GetOrigin()
end

function M.CanPlayerHaveAmmo(player, ammoname, maxcarry)
	return true
end

function M.OnPlayerGotAmmo(player, name, count)
end

function M.ShouldAmmoRespawn(ammo)
	return true
end

function M.GetAmmoRespawnTime(ammo)
	return 0
end

function M.GetAmmoRespawnSpot(ammo)
	return ammo.GetOrigin()
end

function M.GetHealthChargerRechargeTime()
	return 0
end

function M.GetHEVChargerRechargeTime()
	return 0
end

function M.HandleDeadPlayerWeapons(player)
	return 0
end

function M.HandleDeadPlayerAmmo(player)
	return 0
end

function M.GetTeamID(entity)
	return ""
end

function M.GetPlayerRelationship(player, target)
	return 0
end

function M.GetTeamIndex(teamname)
	return -1
end

function M.GetIndexedTeamName(teamindex)
	return ""
end

function M.IsValidTeam(teamname)
	return true
end

function M.ChangePlayerTeam(player, teamname, kill, gib)
end

function M.SetDefaultPlayerTeam(player)
	return ""
end

function M.PlayTextureSounds()
	return true
end

function M.PlayFootstepSounds(player, volume)
	return true
end

function M.AreMonstersAllowed()
	return true
end

function M.EndMultiplayerGame()
end

-- Return the module
return M
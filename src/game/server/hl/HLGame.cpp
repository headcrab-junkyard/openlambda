/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2020-2023 BlackPhrase
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

#include "HLGame.hpp"

CHLGame gGame; //(nullptr);
CGame *gpGame{&gGame};

EXPOSE_SINGLE_INTERFACE_GLOBALVAR(CHLGame, IGame, OGS_GAME_INTERFACE_VERSION, gGame);

// TODO: not all of these cvars should be defined here

/////////////////////////////////////////////////
// CVARs
/////////////////////////////////////////////////

CConVar displaysoundlist("displaysoundlist", "0"); // TODO: make base?


// Multiplayer server rules

// Don't spam console/log files/users with this changing
CConVar fragsleft("mp_fragsleft", "0", FCVAR_SERVER | FCVAR_UNLOGGED);

CConVar timeleft("mp_timeleft", "0", FCVAR_SERVER | FCVAR_UNLOGGED);

CConVar teamplay("mp_teamplay", "0", FCVAR_SERVER);

CConVar fraglimit("mp_fraglimit", "0", FCVAR_SERVER);
CConVar timelimit("mp_timelimit", "0", FCVAR_SERVER);

CConVar friendlyfire("mp_friendlyfire", "0", FCVAR_SERVER);

CConVar falldamage("mp_falldamage", "0", FCVAR_SERVER);

CConVar weaponstay("mp_weaponstay", "0", FCVAR_SERVER);

CConVar forcerespawn("mp_forcerespawn", "1", FCVAR_SERVER);

CConVar flashlight("mp_flashlight", "0", FCVAR_SERVER);

CConVar aimcrosshair("mp_autocrosshair", "1", FCVAR_SERVER);

CConVar decalfrequency("decalfrequency", "30", FCVAR_SERVER);

CConVar teamlist("mp_teamlist", "hgrunt;scientist", FCVAR_SERVER);
CConVar teamoverride("mp_teamoverride", "1");
CConVar defaultteam("mp_defaultteam", "0");

CConVar allowmonsters("mp_allowmonsters", "0", FCVAR_SERVER);

// 0 prevents player from being spectators
CConVar allow_spectators("allow_spectators", "0", FCVAR_SERVER);

CConVar mp_chattime("mp_chattime", "10", FCVAR_SERVER);

/////////////////////////////////////////////////
// MONSTERS/NPCs
/////////////////////////////////////////////////

// A(lien)grunt
CConVar sk_agrunt_health1("sk_agrunt_health1", "0");
CConVar sk_agrunt_health2("sk_agrunt_health2", "0");
CConVar sk_agrunt_health3("sk_agrunt_health3", "0");

CConVar sk_agrunt_dmg_punch1("sk_agrunt_dmg_punch1", "0");
CConVar sk_agrunt_dmg_punch2("sk_agrunt_dmg_punch2", "0");
CConVar sk_agrunt_dmg_punch3("sk_agrunt_dmg_punch3", "0");

// Apache
CConVar sk_apache_health1("sk_apache_health1", "0");
CConVar sk_apache_health2("sk_apache_health2", "0");
CConVar sk_apache_health3("sk_apache_health3", "0");

// Barney
CConVar sk_barney_health1("sk_barney_health1", "0");
CConVar sk_barney_health2("sk_barney_health2", "0");
CConVar sk_barney_health3("sk_barney_health3", "0");

// Bullsquid
CConVar sk_bullsquid_health1("sk_bullsquid_health1", "0");
CConVar sk_bullsquid_health2("sk_bullsquid_health2", "0");
CConVar sk_bullsquid_health3("sk_bullsquid_health3", "0");

CConVar sk_bullsquid_dmg_bite1("sk_bullsquid_dmg_bite1", "0");
CConVar sk_bullsquid_dmg_bite2("sk_bullsquid_dmg_bite2", "0");
CConVar sk_bullsquid_dmg_bite3("sk_bullsquid_dmg_bite3", "0");

CConVar sk_bullsquid_dmg_whip1("sk_bullsquid_dmg_whip1", "0");
CConVar sk_bullsquid_dmg_whip2("sk_bullsquid_dmg_whip2", "0");
CConVar sk_bullsquid_dmg_whip3("sk_bullsquid_dmg_whip3", "0");

CConVar sk_bullsquid_dmg_spit1("sk_bullsquid_dmg_spit1", "0");
CConVar sk_bullsquid_dmg_spit2("sk_bullsquid_dmg_spit2", "0");
CConVar sk_bullsquid_dmg_spit3("sk_bullsquid_dmg_spit3", "0");

// Big Momma
CConVar sk_bigmomma_health_factor1("sk_bigmomma_health_factor1", "1.0");
CConVar sk_bigmomma_health_factor2("sk_bigmomma_health_factor2", "1.0");
CConVar sk_bigmomma_health_factor3("sk_bigmomma_health_factor3", "1.0");

CConVar sk_bigmomma_dmg_slash1("sk_bigmomma_dmg_slash1", "50");
CConVar sk_bigmomma_dmg_slash2("sk_bigmomma_dmg_slash2", "50");
CConVar sk_bigmomma_dmg_slash3("sk_bigmomma_dmg_slash3", "50");

CConVar sk_bigmomma_dmg_blast1("sk_bigmomma_dmg_blast1", "100");
CConVar sk_bigmomma_dmg_blast2("sk_bigmomma_dmg_blast2", "100");
CConVar sk_bigmomma_dmg_blast3("sk_bigmomma_dmg_blast3", "100");

CConVar sk_bigmomma_radius_blast1("sk_bigmomma_radius_blast1", "250");
CConVar sk_bigmomma_radius_blast2("sk_bigmomma_radius_blast2", "250");
CConVar sk_bigmomma_radius_blast3("sk_bigmomma_radius_blast3", "250");

// Gargantua
CConVar sk_gargantua_health1("sk_gargantua_health1", "0");
CConVar sk_gargantua_health2("sk_gargantua_health2", "0");
CConVar sk_gargantua_health3("sk_gargantua_health3", "0");

CConVar sk_gargantua_dmg_slash1("sk_gargantua_dmg_slash1", "0");
CConVar sk_gargantua_dmg_slash2("sk_gargantua_dmg_slash2", "0");
CConVar sk_gargantua_dmg_slash3("sk_gargantua_dmg_slash3", "0");

CConVar sk_gargantua_dmg_fire1("sk_gargantua_dmg_fire1", "0");
CConVar sk_gargantua_dmg_fire2("sk_gargantua_dmg_fire2", "0");
CConVar sk_gargantua_dmg_fire3("sk_gargantua_dmg_fire3", "0");

CConVar sk_gargantua_dmg_stomp1("sk_gargantua_dmg_stomp1", "0");
CConVar sk_gargantua_dmg_stomp2("sk_gargantua_dmg_stomp2", "0");
CConVar sk_gargantua_dmg_stomp3("sk_gargantua_dmg_stomp3", "0");

// Hassassin
CConVar sk_hassassin_health1("sk_hassassin_health1", "0");
CConVar sk_hassassin_health2("sk_hassassin_health2", "0");
CConVar sk_hassassin_health3("sk_hassassin_health3", "0");

// Headcrab
CConVar sk_headcrab_health1("sk_headcrab_health1", "0");
CConVar sk_headcrab_health2("sk_headcrab_health2", "0");
CConVar sk_headcrab_health3("sk_headcrab_health3", "0");

CConVar sk_headcrab_dmg_bite1("sk_headcrab_dmg_bite1", "0");
CConVar sk_headcrab_dmg_bite2("sk_headcrab_dmg_bite2", "0");
CConVar sk_headcrab_dmg_bite3("sk_headcrab_dmg_bite3", "0");

// Hgrunt
CConVar sk_hgrunt_health1("sk_hgrunt_health1", "0");
CConVar sk_hgrunt_health2("sk_hgrunt_health2", "0");
CConVar sk_hgrunt_health3("sk_hgrunt_health3", "0");

CConVar sk_hgrunt_kick1("sk_hgrunt_kick1", "0");
CConVar sk_hgrunt_kick2("sk_hgrunt_kick2", "0");
CConVar sk_hgrunt_kick3("sk_hgrunt_kick3", "0");

CConVar sk_hgrunt_pellets1("sk_hgrunt_pellets1", "0");
CConVar sk_hgrunt_pellets2("sk_hgrunt_pellets2", "0");
CConVar sk_hgrunt_pellets3("sk_hgrunt_pellets3", "0");

CConVar sk_hgrunt_gspeed1("sk_hgrunt_gspeed1", "0");
CConVar sk_hgrunt_gspeed2("sk_hgrunt_gspeed2", "0");
CConVar sk_hgrunt_gspeed3("sk_hgrunt_gspeed3", "0");

// Houndeye
CConVar sk_houndeye_health1("sk_houndeye_health1", "0");
CConVar sk_houndeye_health2("sk_houndeye_health2", "0");
CConVar sk_houndeye_health3("sk_houndeye_health3", "0");

CConVar sk_houndeye_dmg_blast1("sk_houndeye_dmg_blast1", "0");
CConVar sk_houndeye_dmg_blast2("sk_houndeye_dmg_blast2", "0");
CConVar sk_houndeye_dmg_blast3("sk_houndeye_dmg_blast3", "0");

// ISlave
CConVar sk_islave_health1("sk_islave_health1", "0");
CConVar sk_islave_health2("sk_islave_health2", "0");
CConVar sk_islave_health3("sk_islave_health3", "0");

CConVar sk_islave_dmg_claw1("sk_islave_dmg_claw1", "0");
CConVar sk_islave_dmg_claw2("sk_islave_dmg_claw2", "0");
CConVar sk_islave_dmg_claw3("sk_islave_dmg_claw3", "0");

CConVar sk_islave_dmg_clawrake1("sk_islave_dmg_clawrake1", "0");
CConVar sk_islave_dmg_clawrake2("sk_islave_dmg_clawrake2", "0");
CConVar sk_islave_dmg_clawrake3("sk_islave_dmg_clawrake3", "0");

CConVar sk_islave_dmg_zap1("sk_islave_dmg_zap1", "0");
CConVar sk_islave_dmg_zap2("sk_islave_dmg_zap2", "0");
CConVar sk_islave_dmg_zap3("sk_islave_dmg_zap3", "0");

// Ichtyosaur
CConVar sk_ichthyosaur_health1("sk_ichthyosaur_health1", "0");
CConVar sk_ichthyosaur_health2("sk_ichthyosaur_health2", "0");
CConVar sk_ichthyosaur_health3("sk_ichthyosaur_health3", "0");

CConVar sk_ichthyosaur_shake1("sk_ichthyosaur_shake1", "0");
CConVar sk_ichthyosaur_shake2("sk_ichthyosaur_shake2", "0");
CConVar sk_ichthyosaur_shake3("sk_ichthyosaur_shake3", "0");

// Leech
CConVar sk_leech_health1("sk_leech_health1", "0");
CConVar sk_leech_health2("sk_leech_health2", "0");
CConVar sk_leech_health3("sk_leech_health3", "0");

CConVar sk_leech_dmg_bite1("sk_leech_dmg_bite1", "0");
CConVar sk_leech_dmg_bite2("sk_leech_dmg_bite2", "0");
CConVar sk_leech_dmg_bite3("sk_leech_dmg_bite3", "0");

// Controller
CConVar sk_controller_health1("sk_controller_health1", "0");
CConVar sk_controller_health2("sk_controller_health2", "0");
CConVar sk_controller_health3("sk_controller_health3", "0");

CConVar sk_controller_dmgzap1("sk_controller_dmgzap1", "0");
CConVar sk_controller_dmgzap2("sk_controller_dmgzap2", "0");
CConVar sk_controller_dmgzap3("sk_controller_dmgzap3", "0");

CConVar sk_controller_speedball1("sk_controller_speedball1", "0");
CConVar sk_controller_speedball2("sk_controller_speedball2", "0");
CConVar sk_controller_speedball3("sk_controller_speedball3", "0");

CConVar sk_controller_dmgball1("sk_controller_dmgball1", "0");
CConVar sk_controller_dmgball2("sk_controller_dmgball2", "0");
CConVar sk_controller_dmgball3("sk_controller_dmgball3", "0");

// Nihilanth
CConVar sk_nihilanth_health1("sk_nihilanth_health1", "0");
CConVar sk_nihilanth_health2("sk_nihilanth_health2", "0");
CConVar sk_nihilanth_health3("sk_nihilanth_health3", "0");

CConVar sk_nihilanth_zap1("sk_nihilanth_zap1", "0");
CConVar sk_nihilanth_zap2("sk_nihilanth_zap2", "0");
CConVar sk_nihilanth_zap3("sk_nihilanth_zap3", "0");

// Scientist
CConVar sk_scientist_health1("sk_scientist_health1", "0");
CConVar sk_scientist_health2("sk_scientist_health2", "0");
CConVar sk_scientist_health3("sk_scientist_health3", "0");

// Snark
CConVar sk_snark_health1("sk_snark_health1", "0");
CConVar sk_snark_health2("sk_snark_health2", "0");
CConVar sk_snark_health3("sk_snark_health3", "0");

CConVar sk_snark_dmg_bite1("sk_snark_dmg_bite1", "0");
CConVar sk_snark_dmg_bite2("sk_snark_dmg_bite2", "0");
CConVar sk_snark_dmg_bite3("sk_snark_dmg_bite3", "0");

CConVar sk_snark_dmg_pop1("sk_snark_dmg_pop1", "0");
CConVar sk_snark_dmg_pop2("sk_snark_dmg_pop2", "0");
CConVar sk_snark_dmg_pop3("sk_snark_dmg_pop3", "0");

// Zombie
CConVar sk_zombie_health1("sk_zombie_health1", "0");
CConVar sk_zombie_health2("sk_zombie_health2", "0");
CConVar sk_zombie_health3("sk_zombie_health3", "0");

CConVar sk_zombie_dmg_one_slash1("sk_zombie_dmg_one_slash1", "0");
CConVar sk_zombie_dmg_one_slash2("sk_zombie_dmg_one_slash2", "0");
CConVar sk_zombie_dmg_one_slash3("sk_zombie_dmg_one_slash3", "0");

CConVar sk_zombie_dmg_both_slash1("sk_zombie_dmg_both_slash1", "0");
CConVar sk_zombie_dmg_both_slash2("sk_zombie_dmg_both_slash2", "0");
CConVar sk_zombie_dmg_both_slash3("sk_zombie_dmg_both_slash3", "0");

// Turret
CConVar sk_turret_health1("sk_turret_health1", "0");
CConVar sk_turret_health2("sk_turret_health2", "0");
CConVar sk_turret_health3("sk_turret_health3", "0");

// Mini Turret
CConVar sk_miniturret_health1("sk_miniturret_health1", "0");
CConVar sk_miniturret_health2("sk_miniturret_health2", "0");
CConVar sk_miniturret_health3("sk_miniturret_health3", "0");

// Sentry Turret
CConVar sk_sentry_health1("sk_sentry_health1", "0");
CConVar sk_sentry_health2("sk_sentry_health2", "0");
CConVar sk_sentry_health3("sk_sentry_health3", "0");

/////////////////////////////////////////////////
// (PLAYER) WEAPONS
/////////////////////////////////////////////////

// Crowbar whack
CConVar sk_plr_crowbar1("sk_plr_crowbar1", "0");
CConVar sk_plr_crowbar2("sk_plr_crowbar2", "0");
CConVar sk_plr_crowbar3("sk_plr_crowbar3", "0");

// Glock/9mm Pistol Round
CConVar sk_plr_9mm_bullet1("sk_plr_9mm_bullet1", "0");
CConVar sk_plr_9mm_bullet2("sk_plr_9mm_bullet2", "0");
CConVar sk_plr_9mm_bullet3("sk_plr_9mm_bullet3", "0");

// 357 Round
CConVar sk_plr_357_bullet1("sk_plr_357_bullet1", "0");
CConVar sk_plr_357_bullet2("sk_plr_357_bullet2", "0");
CConVar sk_plr_357_bullet3("sk_plr_357_bullet3", "0");

// MP5 Round
CConVar sk_plr_9mmAR_bullet1("sk_plr_9mmAR_bullet1", "0");
CConVar sk_plr_9mmAR_bullet2("sk_plr_9mmAR_bullet2", "0");
CConVar sk_plr_9mmAR_bullet3("sk_plr_9mmAR_bullet3", "0");

// M203 Grenade
CConVar sk_plr_9mmAR_grenade1("sk_plr_9mmAR_grenade1", "0");
CConVar sk_plr_9mmAR_grenade2("sk_plr_9mmAR_grenade2", "0");
CConVar sk_plr_9mmAR_grenade3("sk_plr_9mmAR_grenade3", "0");

// Shotgun buckshot
CConVar sk_plr_buckshot1("sk_plr_buckshot1", "0");
CConVar sk_plr_buckshot2("sk_plr_buckshot2", "0");
CConVar sk_plr_buckshot3("sk_plr_buckshot3", "0");

// Crossbow
CConVar sk_plr_xbow_bolt_client1("sk_plr_xbow_bolt_client1", "0");
CConVar sk_plr_xbow_bolt_client2("sk_plr_xbow_bolt_client2", "0");
CConVar sk_plr_xbow_bolt_client3("sk_plr_xbow_bolt_client3", "0");

CConVar sk_plr_xbow_bolt_monster1("sk_plr_xbow_bolt_monster1", "0");
CConVar sk_plr_xbow_bolt_monster2("sk_plr_xbow_bolt_monster2", "0");
CConVar sk_plr_xbow_bolt_monster3("sk_plr_xbow_bolt_monster3", "0");

// RPG
CConVar sk_plr_rpg1("sk_plr_rpg1", "0");
CConVar sk_plr_rpg2("sk_plr_rpg2", "0");
CConVar sk_plr_rpg3("sk_plr_rpg3", "0");

// Gauss (aka Zero Point Generator)
CConVar sk_plr_gauss1("sk_plr_gauss1", "0");
CConVar sk_plr_gauss2("sk_plr_gauss2", "0");
CConVar sk_plr_gauss3("sk_plr_gauss3", "0");

// Tau Cannon
CConVar sk_plr_egon_narrow1("sk_plr_egon_narrow1", "0");
CConVar sk_plr_egon_narrow2("sk_plr_egon_narrow2", "0");
CConVar sk_plr_egon_narrow3("sk_plr_egon_narrow3", "0");

CConVar sk_plr_egon_wide1("sk_plr_egon_wide1", "0");
CConVar sk_plr_egon_wide2("sk_plr_egon_wide2", "0");
CConVar sk_plr_egon_wide3("sk_plr_egon_wide3", "0");

// Hand Grenade
CConVar sk_plr_hand_grenade1("sk_plr_hand_grenade1", "0");
CConVar sk_plr_hand_grenade2("sk_plr_hand_grenade2", "0");
CConVar sk_plr_hand_grenade3("sk_plr_hand_grenade3", "0");

// Satchel Charge
CConVar sk_plr_satchel1("sk_plr_satchel1", "0");
CConVar sk_plr_satchel2("sk_plr_satchel2", "0");
CConVar sk_plr_satchel3("sk_plr_satchel3", "0");

// Tripmine
CConVar sk_plr_tripmine1("sk_plr_tripmine1", "0");
CConVar sk_plr_tripmine2("sk_plr_tripmine2", "0");
CConVar sk_plr_tripmine3("sk_plr_tripmine3", "0");

/////////////////////////////////////////////////
// (WORLD) WEAPONS
/////////////////////////////////////////////////

CConVar sk_9mm_bullet1("sk_9mm_bullet1", "0");
CConVar sk_9mm_bullet2("sk_9mm_bullet2", "0");
CConVar sk_9mm_bullet3("sk_9mm_bullet3", "0");

CConVar sk_9mmAR_bullet1("sk_9mmAR_bullet1", "0");
CConVar sk_9mmAR_bullet2("sk_9mmAR_bullet2", "0");
CConVar sk_9mmAR_bullet3("sk_9mmAR_bullet3", "0");

CConVar sk_12mm_bullet1("sk_12mm_bullet1", "0");
CConVar sk_12mm_bullet2("sk_12mm_bullet2", "0");
CConVar sk_12mm_bullet3("sk_12mm_bullet3", "0");

// Hornet
CConVar sk_hornet_dmg1("sk_hornet_dmg1", "0");
CConVar sk_hornet_dmg2("sk_hornet_dmg2", "0");
CConVar sk_hornet_dmg3("sk_hornet_dmg3", "0");

/////////////////////////////////////////////////
// HEALTH/CHARGE
/////////////////////////////////////////////////

CConVar sk_suitcharger1("sk_suitcharger1", "0");
CConVar sk_suitcharger2("sk_suitcharger2", "0");
CConVar sk_suitcharger3("sk_suitcharger3", "0");

CConVar sk_battery1("sk_battery1", "0");
CConVar sk_battery2("sk_battery2", "0");
CConVar sk_battery3("sk_battery3", "0");

CConVar sk_healthcharger1("sk_healthcharger1", "0");
CConVar sk_healthcharger2("sk_healthcharger2", "0");
CConVar sk_healthcharger3("sk_healthcharger3", "0");

CConVar sk_healthkit1("sk_healthkit1", "0");
CConVar sk_healthkit2("sk_healthkit2", "0");
CConVar sk_healthkit3("sk_healthkit3", "0");

CConVar sk_scientist_heal1("sk_scientist_heal1", "0");
CConVar sk_scientist_heal2("sk_scientist_heal2", "0");
CConVar sk_scientist_heal3("sk_scientist_heal3", "0");

/////////////////////////////////////////////////
// DMG ADJUSTER CVARs
/////////////////////////////////////////////////

// Monster damage adjusters
CConVar sk_monster_head1("sk_monster_head1", "2");
CConVar sk_monster_head2("sk_monster_head2", "2");
CConVar sk_monster_head3("sk_monster_head3", "2");

CConVar sk_monster_chest1("sk_monster_chest1", "1");
CConVar sk_monster_chest2("sk_monster_chest2", "1");
CConVar sk_monster_chest3("sk_monster_chest3", "1");

CConVar sk_monster_stomach1("sk_monster_stomach1", "1");
CConVar sk_monster_stomach2("sk_monster_stomach2", "1");
CConVar sk_monster_stomach3("sk_monster_stomach3", "1");

CConVar sk_monster_arm1("sk_monster_arm1", "1");
CConVar sk_monster_arm2("sk_monster_arm2", "1");
CConVar sk_monster_arm3("sk_monster_arm3", "1");

CConVar sk_monster_leg1("sk_monster_leg1", "1");
CConVar sk_monster_leg2("sk_monster_leg2", "1");
CConVar sk_monster_leg3("sk_monster_leg3", "1");

// Player damage adjusters
CConVar sk_player_head1("sk_player_head1", "2");
CConVar sk_player_head2("sk_player_head2", "2");
CConVar sk_player_head3("sk_player_head3", "2");

CConVar sk_player_chest1("sk_player_chest1", "1");
CConVar sk_player_chest2("sk_player_chest2", "1");
CConVar sk_player_chest3("sk_player_chest3", "1");

CConVar sk_player_stomach1("sk_player_stomach1", "1");
CConVar sk_player_stomach2("sk_player_stomach2", "1");
CConVar sk_player_stomach3("sk_player_stomach3", "1");

CConVar sk_player_arm1("sk_player_arm1", "1");
CConVar sk_player_arm2("sk_player_arm2", "1");
CConVar sk_player_arm3("sk_player_arm3", "1");

CConVar sk_player_leg1("sk_player_leg1", "1");
CConVar sk_player_leg2("sk_player_leg2", "1");
CConVar sk_player_leg3("sk_player_leg3", "1");

/////////////////////////////////////////////////

bool CHLGame::Init(CreateInterfaceFn afnEngineFactory)
{
	if(!CBaseGame::Init())
		return false;
	
	RegisterCvars(); // TODO: make base?
	
	gpEngine->pfnServerCommand("exec skill.cfg\n");
	return true;
};

void CHLGame::InstallRules()
{
};

void CHLGame::RegisterCvars()
{
	// TODO
};
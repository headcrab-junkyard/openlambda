/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2020-2022 BlackPhrase
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

/////////////////////////////////////////////////
// MONSTERS/NPCs
/////////////////////////////////////////////////

// Agrunt
cvar_t sk_agrunt_health1{"sk_agrunt_health1", "0"};
cvar_t sk_agrunt_health2{"sk_agrunt_health2", "0"};
cvar_t sk_agrunt_health3{"sk_agrunt_health3", "0"};

// Apache
cvar_t sk_apache_health1{"sk_apache_health1", "0"};
cvar_t sk_apache_health2{"sk_apache_health2", "0"};
cvar_t sk_apache_health3{"sk_apache_health3", "0"};

// Barney
cvar_t sk_barney_health1{"sk_barney_health1", "0"};
cvar_t sk_barney_health2{"sk_barney_health2", "0"};
cvar_t sk_barney_health3{"sk_barney_health3", "0"};

// Bullsquid
cvar_t sk_bullsquid_health1{"sk_bullsquid_health1", "0"};
cvar_t sk_bullsquid_health2{"sk_bullsquid_health2", "0"};
cvar_t sk_bullsquid_health3{"sk_bullsquid_health3", "0"};

// Big Momma

// Gargantua
cvar_t sk_gargantua_health1{"sk_gargantua_health1", "0"};
cvar_t sk_gargantua_health2{"sk_gargantua_health2", "0"};
cvar_t sk_gargantua_health3{"sk_gargantua_health3", "0"};

// Hassassin
cvar_t sk_hassassin_health1{"sk_hassassin_health1", "0"};
cvar_t sk_hassassin_health2{"sk_hassassin_health2", "0"};
cvar_t sk_hassassin_health3{"sk_hassassin_health3", "0"};

// Headcrab

// Hgrunt

// Houndeye

// ISlave

// Ichtyosaur

// Leech

// Controller

// Nihilanth

// Scientist

// Snark

// Zombie
cvar_t sk_zombie_health1{"sk_zombie_health1", "0"};
cvar_t sk_zombie_health2{"sk_zombie_health2", "0"};
cvar_t sk_zombie_health3{"sk_zombie_health3", "0"};

// Turret

// Mini Turret

// Sentry Turret

/////////////////////////////////////////////////
// WEAPONS
/////////////////////////////////////////////////

// Crowbar

// Glock

// 357

// MP5

// M203

// Shotgun

// Crossbow

// RPG

// Gauss

// Tau Cannon

// Hand Grenade

// Satchel

// Tripmine

bool CHLGame::Init(CreateInterfaceFn afnEngineFactory)
{
	if(!CBaseGame::Init())
		return false;
	
	RegisterCvars();
	return true;
};

void CHLGame::InstallRules()
{
};

void CHLGame::RegisterCvars()
{
	// TODO
};
/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2020-2021 BlackPhrase
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
cvar_t sk_agrunt_health1{};
cvar_t sk_agrunt_health2{};
cvar_t sk_agrunt_health3{};

// Barney
cvar_t sk_barney_health1{};
cvar_t sk_barney_health2{};
cvar_t sk_barney_health3{};

// Bullsquid
cvar_t sk_bullsquid_health1{};
cvar_t sk_bullsquid_health2{};
cvar_t sk_bullsquid_health3{};

// Big Momma

// Gargantua

// Hassassin

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

void CHLGame::Init()
{
	RegisterCvars();
};

void CHLGame::InstallRules()
{
};

void CHLGame::RegisterCvars()
{
	// TODO
};
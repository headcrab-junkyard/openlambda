/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2022 BlackPhrase
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

#include "CSClientGame.hpp"

EXPOSE_SINGLE_INTERFACE(CCSClientGame, IClientGame, OGS_CLIENTGAME_INTERFACE_VERSION);

void EV_FireGlock1(event_args_t *apArgs);

void EV_CreateExplosion(event_args_t *apArgs);
void EV_CreateSmoke(event_args_t *apArgs);

void EV_DecalReset(event_args_t *apArgs);

void EV_Vehicle(event_args_t *apArgs);

void EV_Knife(event_args_t *apArgs);

void EV_Glock18(event_args_t *apArgs);
void EV_FireUSP(event_args_t *apArgs);
void EV_FireP228(event_args_t *apArgs);
void EV_FireEliteLeft(event_args_t *apArgs);
void EV_FireEliteRight(event_args_t *apArgs);
void EV_FireFiveSeven(event_args_t *apArgs);
void EV_FireDeagle(event_args_t *apArgs);

void EV_FireM3(event_args_t *apArgs);
void EV_FireXM1014(event_args_t *apArgs);

void EV_FireTMP(event_args_t *apArgs);
void EV_FireUMP45(event_args_t *apArgs);
void EV_FireMac10(event_args_t *apArgs);
void EV_FireMP5(event_args_t *apArgs);
void EV_FireP90(event_args_t *apArgs);

void EV_FireFamas(event_args_t *apArgs);
void EV_FireAK47(event_args_t *apArgs);
void EV_FireAUG(event_args_t *apArgs);
void EV_FireGalil(event_args_t *apArgs);
void EV_FireM4A1(event_args_t *apArgs);

void EV_FireG3SG1(event_args_t *apArgs);
void EV_FireSG550(event_args_t *apArgs);
void EV_FireSG552(event_args_t *apArgs);
void EV_FireScout(event_args_t *apArgs);
void EV_FireAWP(event_args_t *apArgs);

void EV_FireM249(event_args_t *apArgs);

void CCSClientGame::HookEvents()
{
	gpEngine->pfnHookEvent("events/createexplo.sc", EV_CreateExplosion);
	gpEngine->pfnHookEvent("events/createsmoke.sc", EV_CreateSmoke);
	
	gpEngine->pfnHookEvent("events/decal_reset.sc", EV_DecalReset);
	
	gpEngine->pfnHookEvent("events/vehicle.sc", EV_Vehicle);
	
	gpEngine->pfnHookEvent("events/knife.sc", EV_Knife);
	
	gpEngine->pfnHookEvent("events/glock18.sc", EV_Glock18);
	gpEngine->pfnHookEvent("events/usp.sc", EV_FireUSP);
	gpEngine->pfnHookEvent("events/p228.sc", EV_FireP228);
	gpEngine->pfnHookEvent("events/elite_left.sc", EV_FireEliteLeft);
	gpEngine->pfnHookEvent("events/elite_right.sc", EV_FireEliteRight);
	gpEngine->pfnHookEvent("events/fiveseven.sc", EV_FireFiveSeven);
	gpEngine->pfnHookEvent("events/deagle.sc", EV_FireDeagle);
	
	gpEngine->pfnHookEvent("events/m3.sc", EV_FireM3);
	gpEngine->pfnHookEvent("events/xm1014.sc", EV_FireXM1014);
	
	gpEngine->pfnHookEvent("events/tmp.sc", EV_FireTMP);
	gpEngine->pfnHookEvent("events/ump45.sc", EV_FireUMP45);
	gpEngine->pfnHookEvent("events/mac10.sc", EV_FireMac10);
	gpEngine->pfnHookEvent("events/mp5n.sc", EV_FireMP5);
	gpEngine->pfnHookEvent("events/p90.sc", EV_FireP90);
	
	gpEngine->pfnHookEvent("events/famas.sc", EV_FireFamas);
	gpEngine->pfnHookEvent("events/ak47.sc", EV_FireAK47);
	gpEngine->pfnHookEvent("events/aug.sc", EV_FireAUG);
	gpEngine->pfnHookEvent("events/galil.sc", EV_FireGalil);
	gpEngine->pfnHookEvent("events/m4a1.sc", EV_FireM4A1);
	
	gpEngine->pfnHookEvent("events/g3sg1.sc", EV_FireG3SG1);
	gpEngine->pfnHookEvent("events/sg550.sc", EV_FireSG550);
	gpEngine->pfnHookEvent("events/sg552.sc", EV_FireSG552);
	gpEngine->pfnHookEvent("events/scout.sc", EV_FireScout);
	gpEngine->pfnHookEvent("events/awp.sc", EV_FireAWP);
	
	gpEngine->pfnHookEvent("events/m249.sc", EV_FireM249);
};
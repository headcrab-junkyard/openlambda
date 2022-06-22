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

#include "HLClientGame.hpp"

EXPOSE_SINGLE_INTERFACE(CHLClientGame, IClientGame, OGS_CLIENTGAME_INTERFACE_VERSION);

void EV_FireGlock1(event_args_t *apArgs);
void EV_FireGlock2(event_args_t *apArgs);

void EV_FireShotGunSingle(event_args_t *apArgs);
void EV_FireShotGunDouble(event_args_t *apArgs);

void EV_FireMP5(event_args_t *apArgs);
void EV_FireMP52(event_args_t *apArgs);

void EV_FirePython(event_args_t *apArgs);

void EV_FireGauss(event_args_t *apArgs);
void EV_SpinGauss(event_args_t *apArgs);

void EV_TrainPitchAdjust(event_args_t *apArgs);

void EV_Crowbar(event_args_t *apArgs);

void EV_FireCrossbow(event_args_t *apArgs);
void EV_FireCrossbow2(event_args_t *apArgs);

void EV_FireRPG(event_args_t *apArgs);

void EV_EgonFire(event_args_t *apArgs);
void EV_EgonStop(event_args_t *apArgs);

void EV_HornetGunFire(event_args_t *apArgs);

void EV_TripmineFire(event_args_t *apArgs);

void EV_SnarkFire(event_args_t *apArgs);

void CHLClientGame::HookEvents()
{
	gpEngine->pfnHookEvent("events/glock1.sc", EV_FireGlock1);
	gpEngine->pfnHookEvent("events/glock2.sc", EV_FireGlock2);
	
	gpEngine->pfnHookEvent("events/shotgun1.sc", EV_FireShotGunSingle);
	gpEngine->pfnHookEvent("events/shotgun2.sc", EV_FireShotGunDouble);
	
	gpEngine->pfnHookEvent("events/mp5.sc", EV_FireMP5);
	gpEngine->pfnHookEvent("events/mp52.sc", EV_FireMP52);
	
	gpEngine->pfnHookEvent("events/python.sc", EV_FirePython);
	
	gpEngine->pfnHookEvent("events/gauss.sc", EV_FireGauss);
	gpEngine->pfnHookEvent("events/gaussspin.sc", EV_SpinGauss);
	
	gpEngine->pfnHookEvent("events/train.sc", EV_TrainPitchAdjust);
	
	gpEngine->pfnHookEvent("events/crowbar.sc", EV_Crowbar);
	
	gpEngine->pfnHookEvent("events/crossbow1.sc", EV_FireCrossbow);
	gpEngine->pfnHookEvent("events/crossbow2.sc", EV_FireCrossbow2);
	
	gpEngine->pfnHookEvent("events/rpg.sc", EV_FireRPG);
	
	gpEngine->pfnHookEvent("events/egon_fire.sc", EV_EgonFire);
	gpEngine->pfnHookEvent("events/egon_stop.sc", EV_EgonStop);
	
	gpEngine->pfnHookEvent("events/firehornet.sc", EV_HornetGunFire);
	
	gpEngine->pfnHookEvent("events/tripfire.sc", EV_TripmineFire);
	
	gpEngine->pfnHookEvent("events/snarkfire.sc", EV_SnarkFire);
};
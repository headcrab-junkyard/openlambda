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

#include "HUD.hpp"
#include "engine.h"

void __CmdFunc_Slot1(){} // TODO
void __CmdFunc_Slot2(){} // TODO
void __CmdFunc_Slot3(){} // TODO
void __CmdFunc_Slot4(){} // TODO
void __CmdFunc_Slot5(){} // TODO
void __CmdFunc_Slot6(){} // TODO
void __CmdFunc_Slot7(){} // TODO
void __CmdFunc_Slot8(){} // TODO
void __CmdFunc_Slot9(){} // TODO
void __CmdFunc_Slot10(){} // TODO

int CHUDAmmo::Init()
{
	gpEngine->pfnAddCommand("slot1", __CmdFunc_Slot1);
	gpEngine->pfnAddCommand("slot2", __CmdFunc_Slot2);
	gpEngine->pfnAddCommand("slot3", __CmdFunc_Slot3);
	gpEngine->pfnAddCommand("slot4", __CmdFunc_Slot4);
	gpEngine->pfnAddCommand("slot5", __CmdFunc_Slot5);
	gpEngine->pfnAddCommand("slot6", __CmdFunc_Slot6);
	gpEngine->pfnAddCommand("slot7", __CmdFunc_Slot7);
	gpEngine->pfnAddCommand("slot8", __CmdFunc_Slot8);
	gpEngine->pfnAddCommand("slot9", __CmdFunc_Slot9);
	gpEngine->pfnAddCommand("slot10", __CmdFunc_Slot10);
	
	Reset();
	return 1;
};

int CHUDAmmo::VidInit()
{
	return 1;
};

void CHUDAmmo::Reset()
{
};

void CHUDAmmo::SlotInput(int anSlot)
{
};
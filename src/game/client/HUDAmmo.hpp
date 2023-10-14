/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2021, 2023 BlackPhrase
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

#pragma once

#include "HUDElement.hpp"

class CHUDAmmo : public CHUDElement
{
public:
	int Init() override;
	int VidInit() override;
	
	void Reset() override;
	
	int Draw(float afTime) override;
	
	void Think() override;
public:
	int DrawWeaponList(float afTime);
	
	void SlotInput(int anSlot);
	
	void UserCmd_Slot1(){SlotInput(0);}
	void UserCmd_Slot2(){SlotInput(1);}
	void UserCmd_Slot3(){SlotInput(2);}
	void UserCmd_Slot4(){SlotInput(3);}
	void UserCmd_Slot5(){SlotInput(4);}
	void UserCmd_Slot6(){SlotInput(5);}
	void UserCmd_Slot7(){SlotInput(6);}
	void UserCmd_Slot8(){SlotInput(7);}
	void UserCmd_Slot9(){SlotInput(8);}
	void UserCmd_Slot10(){SlotInput(9);}
private:
	RGBA mColor{};
	
	WEAPON *mpWeapon{nullptr};
	
	float mfFade{0.0f};
	
	int mnBucket0{-1};
	int mnSelection{-1};
};
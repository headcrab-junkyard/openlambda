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

#pragma once

constexpr auto MAX_WEAPON_NAME{128};

constexpr auto WEAPON_FLAGS_SELECTIONEMPTY{1};

constexpr auto WEAPON_IS_ONTARGET{0x40};

class CHUDTexture;

struct SWeaponInfo
{
	// SHARED
	
	char msName[MAX_WEAPON_NAME];
	
	int mnAmmoType;
	int mnAmmoType2;
	
	int mnMaxAmmo1;
	int mnMaxAmmo2;
	
	int mnSlot; ///< Inventory slot
	int mnSlotPos; ///< Position in the inventory slot
	
	int mnFlags; ///< Misc weapon flags
	
	int mnID;
	int mnClip;
	
	int mnCount; ///< # of item in plist
	
	// SERVER-ONLY
	
	// Nothing
	
	// CLIENT-ONLY
	
	CHUDTexture *mpIconActive{nullptr};
	CHUDTexture *mpIconInactive{nullptr};
	
	CHUDTexture *mpIconAmmo{nullptr};
	CHUDTexture *mpIconAmmo2{nullptr};
	
	CHUDTexture *mpIconCrosshair{nullptr};
	CHUDTexture *mpIconAutoAim{nullptr};
	
	CHUDTexture *mpIconZoomedCrosshair{nullptr};
	CHUDTexture *mpIconZoomedAutoAim{nullptr};
	
	//bool mbShowUsageHint{false};
};
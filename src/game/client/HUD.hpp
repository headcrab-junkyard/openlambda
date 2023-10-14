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
/// @brief main HUD manager, handles the message, calculation and drawing the HUD

#pragma once

using cvar_t = struct cvar_s;

class CHUDElement;
class CHUDTexture;

// TODO: CGameHUD?
class CHUD
{
public:
	CHUD() = default;
	~CHUD();
	
	//void Reset();
	
	/// Called when the HUD is created at the module loading stage
	void Init();
	
	/// Called when the video mode is changed
	void VidInit();
	
	//void Update();
	void Think();
	
	int Redraw(float afTime, int anIntermission);
	
	int UpdateClientData(client_data_t *apData, float afTime);
	
	//void SetActive(bool abActive);
	//bool IsActive() const;
	
	/// HUD element registration
	void AddElement(CHUDElement *apElement);
	
	float GetSensitivity() const {return mfMouseSensitivity;} // TODO: GetMouseSensitivity?
	//float GetFOVSensitivityAdjust() const;
	
	/*SCREENINFO *GetScreenInfo() const;*/ // TODO
private:
	//void InitFonts();
	
	//void SetupNewTexture(CHUDTexture *apTexture);
private:
	cvar_t *mpCvarDraw{nullptr};
	cvar_t *default_fov{nullptr};
	
	float mfMouseSensitivity{0.0f};
	
	//bool mbTexturesLoaded{false};
};

//extern CHUD gHUD; // jiHAD!
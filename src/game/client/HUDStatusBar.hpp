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

#include "HUDElement.hpp"

class CHUDStatusBar : public CHUDElement
{
public:
	int Init();
	int VidInit();
	
	void Reset();
	
	int Draw(float afTime);
	
	void ParseStatusString(int anLineNum);
	
	int MsgFunc_StatusText(const char *asName, int anSize, void *apBuf);
	int MsgFunc_StatusValue(const char *asName, int anSize, void *apBuf);
protected:
	constexpr auto MAX_STATUSTEXT_LENGTH{128};
	constexpr auto MAX_STATUSBAR_VALUES{8};
	constexpr auto MAX_STATUSBAR_LINES{3};
	
	char msStatusText[MAX_STATUSBAR_LINES][MAX_STATUSTEXT_LENGTH]{}; ///< A text string describing how the status bar is to be drawn
	char msStatusBar[MAX_STATUSBAR_LINES][MAX_STATUSTEXT_LENGTH]{}; ///< The constructed bar that is drawn
	
	float *mvNameColors[MAX_STATUSBAR_LINES]{nullptr}; ///< An array of colors (one color for each line)
	
	int mnStatusValues[MAX_STATUSBAR_VALUES]{}; ///< An array of values for use in the status bar
	
	bool mbReparseString{false}; ///< Set to true whenever the msStatusBar needs to be recalculated
};
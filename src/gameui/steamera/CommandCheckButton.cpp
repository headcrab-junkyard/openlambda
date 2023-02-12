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

#include "CommandCheckButton.hpp"
//#include "EngineInterface.hpp"

// NOTE: memdbgon must be the last include file in a .cpp file!
//#include <tier0/memdbgon.h>

//using namespace vgui;

CCommandCheckButton::CCommandCheckButton(vgui::Panel *apParent, const char *asPanelName, const char *asText, const char *asDownCmd, const char *asUpCmd) : BaseClass(apParent, asPanelName, asText)
{
	if(asDownCmd)
		msDownCmd = strdup(asDownCmd);
	
	if(asUpCmd)
		msUpCmd = strdup(asUpCmd);
};

CCommandCheckButton::~CCommandCheckButton()
{
	free(msDownCmd);
	free(msUpCmd);
};

void CCommandCheckButton::SetSelected(bool abState)
{
	BaseClass::SetSelected(abState);
	
	if(IsSelected() && msDownCmd)
	{
		gpEngine->ClientCmd_Unrestricted(msDownCmd);
		gpEngine->ClientCmd_Unrestricted("\n");
	}
	else if(!IsSelected() && msUpCmd)
	{
		gpEngine->ClientCmd_Unrestricted(msUpCmd);
		gpEngine->ClientCmd_Unrestricted("\n");
	};
};
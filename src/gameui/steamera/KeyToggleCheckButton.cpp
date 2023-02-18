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

#include "KeyToggleCheckButton.hpp"
//#include "EngineInterface.hpp"

//#include <vgui/IVGui.h>

//#include <IGameUIFuncs.h>

// NOTE: memdbgon must be the last include file in a .cpp file!
//#include <tier0/memdbgon.h>

//using namespace vgui;

CKeyToggleCheckButton::CKeyToggleCheckButton(vgui::Panel *apParent, const char *asPanelName, const char *asText, const char *asKeyName, const char *asCmdName) : BaseClass(apParent, asPanelName, asText)
{
	if(asKeyName)
		msKeyName = strdup(asKeyName);
	
	if(asCmdName)
		msCmdName = strdup(asCmdName);
	
	if(msKeyName)
		Reset();
	
	//mbNoCommand = false;
};

CKeyToggleCheckButton::~CKeyToggleCheckButton()
{
	free(msKeyName);
	free(msCmdName);
};

/*
void CKeyToggleCheckButton::SetSelected(bool abState)
{
	BaseClass::SetSelected(abState);
	
	if(!msCmdName || !msCmdName[0])
		return;
	
	if(mbNoCommand)
		return;
	
	char sCmd[256]{};
	Q_snprintf(sCmd, sizeof(sCmd), "%c%s\n", IsSelected() ? '+' : '-', msCmdName);
	
	gpEngine->pfnClientCmd(sCmd);
};
*/

void CKeyToggleCheckButton::Paint()
{
	BaseClass::Paint();
	
	if(!msKeyName)
		return;
	
	// FIXME: lookup key state
	bool bIsDown;
	if(gpGameUIFuncs->IsKeyDown(msKeyName, bIsDown))
	{
		// If someone changed the value using the console
		if(mbStartValue != bIsDown)
		{
			SetSelected(bIsDown);
			mbStartValue = bIsDown;
		};
	};
};

void CKeyToggleCheckButton::Reset()
{
	gpGameUIFuncs->IsKeyDown(msKeyName, mbStartValue);
	
	if(IsSelected() != mbStartValue)
		SetSelected(mbStartValue);
};

void CKeyToggleCheckButton::ApplyChanges()
{
	if(!msCmdName || !msCmdName[0])
		return;
	
	char sCmd[256]{};
	Q_snprintf(sCmd, sizeof(sCmd), "%c%s\n", IsSelected() ? '+' : '-', msCmdName);
	
	gpEngine->ClientCmd_Unrestricted(sCmd);
};

bool CKeyToggleCheckButton::HasBeenModified() const
{
	return IsSelected() != mbStartValue;
};
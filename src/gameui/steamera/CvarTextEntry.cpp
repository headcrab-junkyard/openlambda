/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2020, 2023-2024 BlackPhrase
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

#include "CvarTextEntry.hpp"

//#include "EngineInterface.h"

//#include <vgui/IVGui.h>
//#include <IGameUIFuncs.h>
//#include <tier1/KeyValues.h>
//#include <tier1/convar.h>

// NOTE: memdbgon must be the last include file in a source file!
//#include <tier0/memdbgon.h>

//using namespace vgui;

static constexpr auto MAX_CVAR_TEXT = 64;

CCvarTextEntry::CCvarTextEntry(vgui2::Panel *apParent, const char *asPanelName, const char *asCvarName) : TextEntry(apParent, asPanelName)
{
	msCvarName = asCvarName ? strdup(asCvarName) : nullptr;
	msStartValue[0] = 0;
	
	if(msCvarName)
		Reset();
	
	AddActionSignalTarget(this);
};

CCvarTextEntry::~CCvarTextEntry()
{
	if(msCvarName)
		free(msCvarName);
};

void CCvarTextEntry::OnTextChanged()
{
	if(!msCvarName)
		return;
	
	if(HasBeenModified())
		PostActionSignal(new KeyValues("ControlModified"));
};

void CCvarTextEntry::Reset()
{
	//char *sValue{gpEngine->pfnGetCvarString(msCvarName)};
	ConVarRef VarRef(msCvarName);
	if(!VarRef.IsValid())
		return;
	
	auto sValue{VarRef.GetString()};
	if(sValue && sValue[0])
	{
		SetText(sValue);
		Q_strncpy(msStartValue, sValue, sizeof(msStartValue));
	};
};

void CCvarTextEntry::ApplyChanges(bool abImmediate)
{
	if(!msCvarName)
		return;
	
	char sText[MAX_CVAR_TEXT]{};
	GetText(sText, MAX_CVAR_TEXT);
	
	if(!sText[0])
		return;
	
	if(abImmediate)
	{
		// Set immediately - don't wait for the next frame
		ConVarRef NewCvar(msCvarName);
		NewCvar.SetValue(sText);
	}
	else
	{
		char sCommand[256]{};
		sprintf(sCommand, "%s \"%s\"\n", msCvarName, sText);
		gpEngine->ClientCmd_Unrestricted(sCommand);
	};
	
	Q_strncpy(msStartValue, sText, sizeof(msStartValue));
};

void CCvarTextEntry::ApplySchemeSettings(vgui2::IScheme *apScheme)
{
	BaseClass::ApplySchemeSettings(apScheme);
	
	if(GetMaximumCharCount() < 0 || GetMaximumCharCount() > MAX_CVAR_TEXT)
		SetMaximumCharCount(MAX_CVAR_TEXT - 1);
};

bool CCvarTextEntry::HasBeenModified()
{
	char sText[MAX_CVAR_TEXT]{};
	GetText(sText, MAX_CVAR_TEXT);
	
	return stricmp(sText, msStartValue) != 0 ? true : false;
};
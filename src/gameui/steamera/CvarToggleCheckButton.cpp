/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2020, 2023 BlackPhrase
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

#include "CvarToggleCheckButton.hpp"
//#include "EngineInterface.hpp"

//#include <vgui/IVGui.h>

//#include <tier1/KeyValues.h>
//#include <tier1/convar.h>

//#include <IGameUIFuncs.h>

// NOTE: memdbgon must be the last include file in a .cpp file!
#include <tier0/memdbgon.h>

//using namespace vgui;

vgui::Panel *CvarToggleCheckButtonFactory()
{
	return new CCvarToggleCheckButton(nullptr, nullptr, "CvarToggleCheckButton", nullptr);
};

DECLARE_BUILD_FACTORY_CUSTOM(CCvarToggleCheckButton, CvarToggleCheckButtonFactory);

CCvarToggleCheckButton::CCvarToggleCheckButton(vgui2::Panel *apParent, const char *asPanelName, const char *asText, const char *asCvarName)
	: BaseClass(apParent, asPanelName, asText)
{
	if(asCvarName)
	{
		msCvarName = strdup(asCvarName);
		Reset();
	};
	
	AddActionSignalTarget(this);
};

CCvarToggleCheckButton::~CCvarToggleCheckButton()
{
	if(msCvarName)
		free(msCvarName);
};

void CCvarToggleCheckButton::SetSelected(bool abState)
{
	BaseClass::SetSelected(abState);
	
	if(!msCvarName || !msCvarName[0])
		return;

/*
	// Look up current value
	bool bValue{abState};
	
	gpEngine->Cvar_SetValue(msCvarName, bValue ? 1.0f : 0.0f);
*/
};

void CCvarToggleCheckButton::Paint()
{
	if(!msCvarName || !msCvarName[0])
	{
		BaseClass::Paint();
		return;
	};
	
	// Look up current value
	
	ConVarRef var(msCvarName);
	if(!var.IsValid())
		return;
	
	//bool bValue{gpEngine->pfnGetCvarFloat(msCvarName) > 0.0f};
	bool bValue{var.GetBool()};
	
	//if(bValue != IsSelected())
	if(bValue != mbStartValue)
	{
		SetSelected(bValue);
		mbStartValue = bValue;
	};
	
	BaseClass::Paint();
};

void CCvarToggleCheckButton::Reset()
{
	//mbStartValue = gpEngine->pfnGetCvarFloat(msCvarName) > 0.0f;
	
	if(!msCvarName || !msCvarName[0])
		return;
	
	ConVarRef var(msCvarName);
	if(!var.IsValid())
		return;
	
	mbStartValue = var.GetBool();
	SetSelected(mbStartValue);
};

void CCvarToggleCheckButton::ApplyChanges()
{
	if(!msCvarName || !msCvarName[0])
		return;
	
	mbStartValue = IsSelected();
	
	//gpEngine->Cvar_SetValue(msCvarName, mbStartValue ? 1.0f : 0.0f);
	ConVarRef var(msCvarName);
	var.SetValue(mbStartValue);
};

void CCvarToggleCheckButton::ApplySettings(KeyValues *apResourceData)
{
	BaseClass::ApplySettings(apResourceData);
	
	auto sCvarName{apResourceData->GetString("cvar_name", "")};
	auto sCvarValue{apResourceData->GetString("cvar_value", "")};
	
	if(!Q_stricmp(sCvarName, ""))
		return; // Doesn't have cvar set up res file, must have been constructed with it
	
	if(msCvarName)
		free(msCvarName); // Got a "", not a nullptr from the create-control call
	
	msCvarName = nullptr;
	
	if(sCvarName)
		msCvarName = strdup(sCvarName);
	
	mbStartValue = false;
	
	if(!Q_stricmp(sCvarValue, "1"))
		mbStartValue = true;
	
	auto pVar{cvar->FindVar(msCvarName)};
	if(pVar)
	{
		SetSelected(false);
		
		if(var->GetBool())
			SetSelected(true);
	};
};

bool CCvarToggleCheckButton::HasBeenModified()
{
	return IsSelected() != mbStartValue;
};

void CCvarToggleCheckButton::OnButtonChecked()
{
	if(HasBeenModified())
		PostActionSignal(new KeyValues("ControlModified"));
};
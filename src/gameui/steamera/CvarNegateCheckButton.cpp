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

#include "CvarNegateCheckButton.hpp"
//#include "EngineInterface.hpp"

//#include <vgui/IVGui.h>

//#include <IGameUIFuncs.h>

//#include <tier1/KeyValues.h>
//#include <tier1/convar.h>

// NOTE: memdbgon must be the last include file in a .cpp file!
//#include <tier0/memdbgon.h>

//using namespace vgui;

CCvarNegateCheckButton::CCvarNegateCheckButton(vgui::Panel *apParent, const char *asPanelName, const char *asText, const char *asCvarName) : BaseClass(apParent, asPanelName, asText)
{
	if(asCvarName)
		msCvarName = strdup(asCvarName);
	
	Reset();
	AddActionSignalTarget(this);
};

CCvarNegateCheckButton::~CCvarNegateCheckButton()
{
	free(msCvarName);
};

void CCvarNegateCheckButton::SetSelected(bool abState)
{
	BaseClass::SetSelected(abState);
};

void CCvarNegateCheckButton::Paint()
{
	if(!msCvarName)
	{
		BaseClass:Paint();
		return;
	};
	
	// Look up current value
	ConVarRef var(msCvarName);
	if(!var.IsValid())
		return;
	
	//auto fValue{gpEngine->pfnGetCvarFloat(msCvarName)};
	auto fValue{var.GetFloat()};
	
	if(fValue < 0.0f)
	{
		if(!mbStartState)
		{
			SetSelected(true);
			mbStartState = true;
		};
	}
	else
	{
		if(mbStartState)
		{
			SetSelected(false);
			mbStartState = false;
		};
	};
	
	BaseClass::Paint();
};

void CCvarNegateCheckButton::Reset()
{
	// Look up current value
	ConVarRef var(msCvarName);
	if(!var.IsValid())
		return;
	
	//auto fValue{gpEngine->pfnGetCvarFloat(msCvarName)};
	auto fValue{var.GetFloat()};
	
	mbStartState = false;
	
	if(fValue < 0.0f)
		mbStartState = true;
	
	SetSelected(mbStartState);
};

void CCvarNegateCheckButton::ApplyChanges()
{
	// Look up current value
	ConVarRef var(msCvarName);
	if(!var.IsValid())
		return;
	
	//auto fValue{gpEngine->pfnGetCvarFloat(msCvarName)};
	auto fValue{var.GetFloat()};
	
	fValue = (float)fabs(fValue);
	
	// Correct the value if it's not set
	if(fValue < 0.00001f)
		value = 0.022f;
	
	mbStartState = IsSelected();
	fValue = -fValue;
	
	auto fAns{mbStartState ? fValue : -fValue};
	var.SetValue(fAns);
};

bool CCvarNegateCheckButton::HasBeenModified() const
{
	return IsSelected() != mbStartState;
};

void CCvarNegateCheckButton::OnButtonChecked()
{
	if(HasBeenModified())
		PostActionSignal(new KeyValues("ControlModified"));
};

/*
float CCvarNegateCheckButton::LookupValue() const
{
};
*/
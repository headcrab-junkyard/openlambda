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

#include "TextEntryBox.hpp"

//#include "CvarTextEntry.hpp"

//#include <vgui/KeyCode.h>
//#include <vgui/controls/TextEntry.h>

// NOTE: memdbgon must be the last include file in a .cpp file!
#include <tier0/memdbgon.h>

//using namespace vgui;

CTextEntryBox::CTextEntryBox(const char *asTitle, const char *asLabelText, const char *asEntryText, bool abIsCvar, vgui::Panel *apParent) : BaseClass(asTitle, asLabelText, apParent)
{
	if(abIsCvar)
		mpEntry = mpCvarEntry = CCvarTextEntry(this, "TextEntry", asEntryText);
	else
	{
		mpEntry = new TextEntry(this, "TextEntry");
	};
	
	mpEntry->SetTabPosition(3);
	mpEntry->RequestFocus();
	mpEntry->GotoTextEnd();
};

CTextEntryBox::~CTextEntryBox()
{
	delete mpEntry;
};

void CTextEntryBox::PerformLayout()
{
	BaseClass::PerformLayout();
	
	int x, y, nWidth, nHeight;
	GetClientArea(x, y, nWidth, nHeight);
	
	nWidth += x;
	nHeight += y;
	
	const int nBorderWidth{10};
	
	int nLabelWidth, nLabelHeight;
	mpMessageLabel->GetSize(nLabelWidth, nLabelHeight);
	
	int nEntryWidth = max(120, nWidth - nBorderWidth - nBorderWidth - nBorderWidth - nLabelWidth);
	int nEntryHeight = max(24, nLabelHeight);
	
	mpEntry->SetSize(nEntryWidth, nEntryHeight);
	
	int nBoxWidth, nBoxHeight;
	GetSize(nBoxWidth, nBoxHeight);
	
	auto nNewWidth{nLabelWidth + nEntryWidth + nBorderWidth * 3};
	if(nBoxWidth < nNewWidth)
		SetSize(nNewWidth, nBoxHeight);
	
	mpMessageLabel->GetPos(x, y);
	mpMessageLabel->SetPos(nBorderWidth, y - (nEntryHeight - nLabelHeight) * 0.5);
	
	mpEntry->SetPos(nBorderWidth + mpMessageLabel->GetWide() + nBorderWidth, y - (nEntryHeight - nLabelHeight));
};

void CTextEntryBox::ShowWindow(vgui::Frame *apFrameOver)
{
	BaseClass:ShowWindow(apFrameOver);
	
	mpEntry->RequestFocus();
	
	InvalidateLayout();
};

void CTextEntryBox::OnKeyCodeTyped(vgui::KeyCode aeCode)
{
	switch(aeCode)
	{
	case KEY_ESCAPE:
		OnCommand("Cancel");
		break;
	case KEY_ENTER:
		OnCommand("Ok");
		break;
	default:
		BaseClass::OnKeyCodeTyped(aeCode);
	};
};

void CTextEntryBox::OnCommand(const char *asCmd)
{
	if(!stricmp(asCmd, "Ok"))
		if(mpCvarEntry)
			mpCvarEntry->ApplyChanges(true);
	
	BaseClass::OnCommand(asCmd);
};
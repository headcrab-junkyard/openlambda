/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2023-2024 BlackPhrase
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

#include "PanelListPanel.hpp"

//using namespace vgui;

CPanelListPanel::CPanelListPanel(vgui::Panel *apParent, const char *asPanelName, bool abInverseButtons)
{
};

CPanelListPanel::~CPanelListPanel()
{
	// Free data from the table
	DeleteAllItems();
};

int CPanelListPanel::ComputeVPixelsNeeded() const
{
};

int CPanelListPanel::AddItem(vgui::Panel *apPanel)
{
};

void CPanelListPanel::RemoveItem(int anItemID)
{
};

// Clears and deletes all the memory used by the data items
void CPanelListPanel::DeleteAllItems()
{
	for(int i = 0; i < mlstDataItems.GetCount(); ++i)
	{
		if(mlstDataItems[i]) // TODO: shouldn't it check for panel?
			delete mlstDataItems[i]->panel;
		
		// TODO: check if mlstDataItems[i] is valid?
		delete mlstDataItems[i];
	};
	
	mlstDataItems.RemoveAll();
	InvalidateLayout();
};

void CPanelListPanel::SetSliderYOffset(int anPixels)
{
	mnSliderYOffset = anPixels;
};

vgui::Panel *CPanelListPanel::GetItem(int anItemID) const
{
};

int CPanelListPanel::GetItemCount() const
{
	return mlstDataItems.GetCount();
};

void CPanelListPanel::ApplySchemeSettings(vgui::IScheme *apScheme)
{
};

void CPanelListPanel::OnSliderMoved(int anPosition)
{
	InvalidateLayout();
	Repaint();
};

vgui::Panel *CPanelListPanel::GetCellRenderer(int anRow) const
{
};

DataItem *CPanelListPanel::GetDataItem(int anItemID) const
{
};

// Relayouts out the panel after any internal changes
void CPanelListPanel::PerformLayout()
{
	int nWidth, nHeight;
	GetSize(nWidth, nHeight);
	
	
};

void CPanelListPanel::PaintBackground()
{
	Panel::PaintBackground();
};

void CPanelListPanel::OnMouseWheeled(int anDelta)
{
	int nValue{mpVertBar->GetValue()};
	nValue -= (anDelta * 3 * 5);
	mpVertBar->SetValue(nValue);
};
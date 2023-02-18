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
/// @brief A list of variable height child panels

#pragma once

#include <vgui/controls/Panel.h>
//#include <vgui/VGUI.h>

//class KeyValues;

class CPanelListPanel : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CPanelListPanel, vgui::Panel);
public:
	CPanelListPanel(vgui::Panel *apParent, const char *asPanelName, bool abInverseButtons = false);
	~CPanelListPanel();
public: // DATA & ROW HANDLING
	/// The list now owns the panel
	virtual int ComputeVPixelsNeeded() const;
	
	virtual int AddItem(vgui::Panel *apPanel);
	
	/// Removed an item from the table (changing the indices of all following items)
	virtual void RemoveItem(int anItemID);
	
	/// Clears and deletes all the memory used by the data items
	virtual void DeleteAllItems(); // TODO: RemoveAllItems?
	
	/// Career mode UI wants to nudge sub-controls around
	void SetSliderYOffset(int anPixels);
	
	/// @return pointer to data the row holds
	virtual vgui::Panel *GetItem(int anItemID) const;
	
	virtual int GetItemCount() const;
public: // PAINTING
	virtual void ApplySchemeSettings(vgui::IScheme *apScheme);
	
	MESSAGE_FUNC_INT(OnSliderMoved, "ScrollBarSliderMoved", position);
	
	virtual vgui::Panel *GetCellRenderer(int anRow) const;
	
	vgui::Panel *GetEmbedded() const {return mpEmbedded;}
public:
	struct DataItem
	{
		vgui::Panel *mpPanel{nullptr}; ///< Always store a panel pointer
	};
protected:
	DataItem *GetDataItem(int anItemID) const;
	
	virtual void PerformLayout();
	virtual void PaintBackground();
	virtual void OnMouseWheeled(int anDelta);
private:
	vgui::Dar<DataItem*> mlstDataItems; ///< List of the column headers
	
	vgui::ScrollBar *mpVertBar{nullptr};
	vgui::Panel *mpEmbedded{nullptr};
	
	int mnTableStartX{0};
	int mnTableStartY{0};
	
	int mnSliderYOffset{0};
};
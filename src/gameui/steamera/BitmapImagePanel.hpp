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

#include <vgui/controls/Panel.h>

class CBitmapImagePanel : public vgui::Panel
{
	DECLARE_CLASS_SIMPLE(CBitmapImagePanel, vgui::Panel);
public:
	CBitmapImagePanel(vgui::Panel *apParent, const char *asPanelName, const char *asFileName = nullptr);
	
	virtual void PaintBackground();
	
	virtual void SetTexture(const char *asFileName);
	
	virtual void ForceReload();
private:
	void ForceUpload();
private:
	char msTexture[128]{};
	
	int mnTextureID{-1};
	
	bool mbUploaded{false}:
};
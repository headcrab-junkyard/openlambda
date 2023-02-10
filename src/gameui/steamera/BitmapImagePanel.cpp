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

#include "BitmapImagePanel.hpp"

#include <vgui/ISurface.h>

// NOTE: memdbgon must be the last include file in a .cpp file!
//#include <tier0/memdbgon.h>

//using namespace vgui;

CBitmapImagePanel::CBitmapImagePanel(vgui::Panel *apParent, const char *asPanelName, const char *asFileName) : BaseClass(apParent, asPanelName)
{
	SetBounds(0, 0, 100, 100);
	
	if(asFileName && asFileName[0])
		Q_strncpy(msTexture, asFileName, sizeof(msTexture));
};

void CBitmapImagePanel::PaintBackground()
{
	if(!msTexture[0])
		return;
	
	if(!mbUploaded)
		ForceUpload();
	
	int nWidth, nHeight;
	GetSize(nWidth, nHeight);
	
	vgui::surface()->DrawSetColor(Color(255, 255, 255, 255));
	vgui::surface()->DrawSetTexture(mnTextureID);
	vgui::surface()->DrawTexturedRect(0, 0, nWidth, nHeight);
};

void CBitmapImagePanel::SetTexture(const char *asFileName)
{
	Q_strncpy(msTexture, asFileName, sizeof(msTexture));
	
	if(mbUploaded)
		ForceReload();
	else
		ForceUpload();
};

void CBitmapImagePanel::ForceReload()
{
	if(!mbUploaded)
		return;
	
	// Force texture to reupload to GPU
	vgui::surface()->DrawSetTextureFile(mnTextureID, msTexture, false, true);
};

void CBitmapImagePanel::ForceUpload()
{
	if(!msTexture[0])
		return;
	
	mnTextureID = vgui::surface()->CreateNewTextureID();
	vgui::surface()->DrawSetTextureFile(mnTextureID, msTexture, false, true);
	
	mbUploaded = true;
};
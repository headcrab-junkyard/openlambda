/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018, 2023 BlackPhrase
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
/// @brief Audio details, part of options dialog

#pragma once

#include <vgui/controls/PropertyPage.h>

class COptionsSubAdvanced : public vgui::PropertyPage // TODO: is it?
{
	DECLARE_CLASS_SIMPLE(COptionsSubAdvanced, vgui::PropertyPage);
public:
	COptionsSubAdvanced(vgui::Panel *apParent);
	~COptionsSubAdvanced();
private:
	vgui::Button *mpContentLockBtn{nullptr};
	vgui::Label *mpContentLockLabel{nullptr};
};
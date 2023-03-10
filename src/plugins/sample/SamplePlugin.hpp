/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2015-2018, 2023 BlackPhrase
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
/// @brief sample engine plugin

#pragma once

#include <ext/IEnginePlugin.hpp>

interface ISystem;

class CSamplePlugin final : public IEnginePlugin
{
public:
	CSamplePlugin() = default;
	~CSamplePlugin() = default;
	
	bool Load(CreateInterfaceFn afnModuleFactory) override;
	void Unload() override;
	
	void GetInfo(TPluginInfo &apInfo) override;
private:
	ISystem *mpSystem{nullptr};
};
/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2024 BlackPhrase
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
/// @brief legacy engine exports

#pragma once

#include <public/engine_hlds_api.h>

#include "IEngineExports.hpp"
#include "Engine.hpp"

class CEngineExportsLegacy final : IEngineExports
{
public:
	CEngineExportsLegacy(IDedicatedServerAPI *apEngine) : mpEngine(apEngine){}
	
	void AddConsoleText(const char *asText) override
	{
		mpEngine->AddConsoleText(asText);
	};
	
	void UpdateStatus(CEngine::Status &aStatus) override
	{
		mpEngine->UpdateStatus(&aStatus.mfFPS, &aStatus.mnActivePlayers, &aStatus.mnMaxPlayers, &aStatus.msMap);
	};
private:
	IDedicatedServerAPI *mpEngine{nullptr};
};
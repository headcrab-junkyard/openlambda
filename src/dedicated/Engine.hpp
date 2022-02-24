/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2021-2022 BlackPhrase
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
/// @brief engine module holder/wrapper

#pragma once

#include "CommonTypes.hpp"

interface IEngineExecMode;
struct IEngine::InitParams;

class CEngine
{
public:
	CEngine(IEngineExecMode *apExecMode);

	enum class Result
	{
		None,
		Restart,
		UnsupportedVideo
	};

	Result Run(const IEngine::InitParams &aInitParams); // TODO: InitAndRun?

	void AddConsoleText(const char *asText);

	struct Status
	{
		float mfFPS{0.0f};
		int mnActivePlayers{0};
		int mnMaxPlayers{0};
		char msMap[32]{};
	};

	void UpdateStatus(Status &aStatus);
private:
	IEngineExecMode *mpExecMode{nullptr};
};
/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2022 BlackPhrase
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
/// @brief new engine (OGSNext) wrapper

#pragma once

#include <stdexcept>

#include "next/engine/IEngine.hpp"

class CEngineNext final : public IEngine
{
public:
    CEngineNext(CreateInterfaceFn afnEngineFactory, CreateInterfaceFn afnFSFactory) : mfnFSFactory(afnFSFactory)
    {
        mpEngine = reinterpret_cast<IEngine*>(afnEngineFactory(OGS_ENGINE_INTERFACE_VERSION, nullptr));

        if(!mpEngine)
            throw std::runtime_error("Failed to get the engine API!");
    };

    bool Init(const InitParams &aInitParams) override
    {
        return pEngine->Init(aInitParams);
    };

    void Shutdown() override
    {
        mpEngine->Shutdown();
    };

    bool Frame() override
    {
        return mpEnine->Frame();
    };
private:
	IEngine *mpEngine{nullptr};
};
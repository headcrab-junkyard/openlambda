/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
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
/// @brief engine hooks interface

#pragma once

#include "CommonTypes.hpp"

void Host_GetConsoleCommands();

interface IEngineHooks
{
    ///
    virtual void PreFrame(){}

    ///
    virtual void PostFrame(){}
};

class CEngineHooks_DedicatedServer : public IEngineHooks
{
public:
    void PreFrame() override
    {
        // Check for commands typed to the host
        Host_GetConsoleCommands();
    };

    void PostFrame() override
    {
#ifdef _WIN32		
        UpdateStatus(0);
#endif
    };
private:
};
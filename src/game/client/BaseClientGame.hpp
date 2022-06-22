/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2018-2022 BlackPhrase
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

#include <next/game/client/IClientGame.hpp>

interface IFileSystem;
interface IInputSystem;
interface IPhysicsSystem;
interface IScriptSystem;

class CGameWorld;

class CBaseClientGame : public IClientGame
{
public: // IClientGame interface methods
	virtual ~CBaseClientGame() = default;
	
	virtual bool Init(CreateInterfaceFn afnEngineFactory /*, cldll_func_t *pcl_funcs*/) override;
	virtual void Shutdown() override;
	
	virtual void Update(/*double afTime*/) override;
protected:
	virtual void HookEvents(){}
public: // Other public methods
	IFileSystem *GetFileSystem() const {return mpFileSystem;}
	IInputSystem *GetInputSystem() const {return mpInputSystem;}
	IPhysicsSystem *GetPhysicsSystem() const {return mpPhysicsSystem;}
	IScriptSystem *GetScriptSystem() const {return mpScriptSystem;}
	
	CGameWorld *GetWorld() const {return mpWorld;}
	//IGameRules *GetRules() const {return mpRules;}
private:
	IFileSystem *mpFileSystem{nullptr};
	IInputSystem *mpInputSystem{nullptr};
	IPhysicsSystem *mpPhysicsSystem{nullptr};
	IScriptSystem *mpScriptSystem{nullptr};
	
	CGameWorld *mpWorld{nullptr};
};
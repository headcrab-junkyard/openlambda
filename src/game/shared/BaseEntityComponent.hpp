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

#pragma once

class CBaseEntity;

class CBaseEntityComponent
{
public:
	CBaseEntityComponent(CBaseEntity *apOwner) : mpOwner(apOwner){}
	virtual ~CBaseEntityComponent() = default;
	
	//virtual void OnAttach(CBaseEntity *apEntity){}
	//virtual void OnDetach(){}
	
	virtual void Update(float afTimeStep){}
	
	//CBaseEntity *GetOwner() const {return mpOwner;}
private:
	CBaseEntity *mpOwner{nullptr};
};

class CTransformComponent : public CBaseEntityComponent
{
public:
	CTransformComponent(CBaseEntity *apOwner) : CBaseEntityComponent(apOwner){}
	
	void SetOrigin(const idVec3 &avOrigin);
	const idVec3 &GetOrigin() const;
	
	void SetRotation(const idVec3 &avRotation);
	const idVec3 &GetRotation() const;
	
	void SetSize(const Bounds &aSize); // TODO: SetScale?
	const Bounds &GetSize() const; // TODO: GetScale?
private:
	idVec3 mvOrigin{};
	idVec3 mvRotation{};
	Bounds mSize{};
};

class CMeshComponent : public CBaseEntityComponent
{
public:
	CMeshComponent(CBaseEntity *apOwner) : CBaseEntityComponent(apOwner){}
private:
};

class CAnimatorComponent : public CBaseEntityComponent
{
public:
	CAnimatorComponent(CBaseEntity *apOwner) : CBaseEntityComponent(apOwner){}
private:
};

class CScriptComponent : public CBaseEntityComponent
{
public:
	CScriptComponent(CBaseEntity *apOwner) : CBaseEntityComponent(apOwner)
	{
		mpScript->Exec("OnLoad");
	};
	
	~CScriptComponent()
	{
		mpScript->Exec("OnDestroy"); // TODO: OnUnload?
	};
	
	void SetActive(bool abActive)
	{
		if(mbActive == abActive)
			return;
		
		mbActive = abActive;
		
		if(mbActive)
			mpScript->Exec("OnEnable");
		else
			mpScript->Exec("OnDisable");
	};
	
	void Update(float afTimeStep) override
	{
		if(mbActive)
			mpScript->Exec("OnFrame");
	};
private:
	IScript *mpScript{nullptr};
};
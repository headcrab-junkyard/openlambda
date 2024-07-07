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

#include "BaseEntity.hpp"

template<typename T, typename... Args>
T *CBaseEntity::AddComponent(Args... aArgs)
{
	return mvComponents.emplace_back(std::make_unique<T>(aArgs)).get();
};

//template<typename T>
//void CBaseEntity::RemoveComponent(T *apComponent)
//{
//};

template<typename T>
T *CBaseEntity::GetComponent() const
{
	for(auto It : mvComponents)
	{
		auto pComponent{It.get()};
		if(typeid(*pComponent) == typeid(T))
			return pComponent;
	};
	
	return nullptr;
};

template<typename T>
T *CBaseEntity::GetComponentAt(int anIndex) const
{
	return mvComponents.at(anIndex).get();
};

template<typename T>
bool CBaseEntity::TryGetComponent(T *apComponent) const
{
	for(auto It : mvComponents)
	{
		auto pComponent{It.get()};
		if(*pComponent == apComponent)
			return true;
	};
	
	return false;
};

tBaseEntityComponentVec &CBaseEntity::GetComponents() const
{
	return mvComponents;
};

int CBaseEntity::GetComponentCount() const
{
	return mvComponents.size();
};

template<typename T>
bool CBaseEntity::HasComponent() const
{
	for(auto It : mvComponents)
	{
		auto pComponent{It.get()};
		if(typeid(*pComponent) == typeid(T))
			return true;
	};
	
	return false;
};
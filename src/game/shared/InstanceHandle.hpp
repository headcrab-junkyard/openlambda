/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 2019, 2021 BlackPhrase
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

//#include <memory>
#include <functional>

template<typename T>
class CInstanceHandle
{
public:
	CInstanceHandle() = default;
	CInstanceHandle(const T *apInstance) : mpInstance(apInstance){}
	~CInstanceHandle(){mpInstance = nullptr;}
	
	T *Reset(T *apInstance)
	{
		mpInstance = apInstance;
		return mpInstance;
	};
	
	constexpr T *Get() const noexcept {return mpInstance;}
	
	bool IsValid() const {return mpInstance != nullptr;} // TODO: mInstance.get() != nullptr ?
	
	T *operator=(T *apOther){return Reset(apOther);}
	
	constexpr operator T*() const noexcept {return Get();}
	
	operator bool() const {return IsValid();}
private:
	T *mpInstance{nullptr};
};
/// @file

#pragma once

#include "BaseEntity.hpp"

class CBasePlayer : public CBaseEntity
{
public:
	void PreThink();
	void PostThink();
	
	void Jump();
	void Duck();
	
	void HandleImpulseCommands();
	
	void UpdateClientData();
	
	bool ShouldFadeOnDeath() const;
	
	const idVec3 &GetGunPosition() const; // TODO: return by ref?
};
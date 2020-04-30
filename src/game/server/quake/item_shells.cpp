/*
	items.qc

	item functions

	Copyright (C) 1996-1997  Id Software, Inc.

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

	See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to:

		Free Software Foundation, Inc.
		59 Temple Place - Suite 330
		Boston, MA  02111-1307, USA

*/

/// @file

#include "BaseAmmo.hpp"

class CItemShells : public CBaseAmmo
{
public:
	bool GiveTo(CBasePlayer *apPlayer) override;
private:
	void PostSpawn() override;
};

/*QUAKED item_shells (0 .5 .8) (0 0 0) (32 32 32) big
*/
LINK_ENTITY_TO_CLASS(item_shells, CItemShells)

bool CItemShells::PreSpawn()
{
	if(deathmatch == 4)
		return false;
};

void CItemShells::PostSpawn()
{
	if (self->spawnflags & WEAPON_BIG2)
	{
		gpEngine->pfnPrecacheModel("maps/b_shell1.bsp");
		SetModel("maps/b_shell1.bsp");
		self->aflag = 40;
	}
	else
	{
		gpEngine->pfnPrecacheModel("maps/b_shell0.bsp");
		SetModel("maps/b_shell0.bsp");
		self->aflag = 20;
	};

	self->netname = "shells";

	//StartItem ();
};

bool CItemShells::GiveTo(CBasePlayer *apPlayer)
{
	// shotgun
	if (apPlayer->ammo_shells >= 100)
		return false;
	apPlayer->ammo_shells += self->aflag;
	// ammo touch sound
	apPlayer->EmitSound(CHAN_ITEM, "weapons/lock4.wav", 1, ATTN_NORM);
	return true;
};
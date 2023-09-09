/*
 * This file is part of OpenLambda Project
 *
 * Copyright (C) 1996-1997 Id Software, Inc.
 * Copyright (C) 2018-2023 BlackPhrase
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
/// @brief weapon and weapon hit functions


//============================================================================

//void player_run();
//void T_RadiusDamage(entvars_t bomb, entvars_t attacker, float rad, entvars_t ignore, string dtype);
//void SpawnBlood(vec3_t org, float damage);
//void SuperDamageSound();

// Called by worldspawn
void W_Precache()
{
	gpEngine->pfnPrecacheSound("weapons/r_exp3.wav"); // new rocket explosion
	
	gpEngine->pfnPrecacheSound("weapons/rocket1i.wav"); // spike gun
	gpEngine->pfnPrecacheSound("weapons/sgun1.wav");
	gpEngine->pfnPrecacheSound("weapons/guncock.wav"); // player shotgun
	
	gpEngine->pfnPrecacheSound("weapons/ric1.wav"); // ricochet (used in c code)
	gpEngine->pfnPrecacheSound("weapons/ric2.wav"); // ricochet (used in c code)
	gpEngine->pfnPrecacheSound("weapons/ric3.wav"); // ricochet (used in c code)
	
	gpEngine->pfnPrecacheSound("weapons/spike2.wav"); // super spikes
	gpEngine->pfnPrecacheSound("weapons/tink1.wav"); // spikes tink (used in c code)
	gpEngine->pfnPrecacheSound("weapons/grenade.wav"); // grenade launcher
	gpEngine->pfnPrecacheSound("weapons/bounce.wav"); // grenade bounce
	gpEngine->pfnPrecacheSound("weapons/shotgn2.wav"); // super shotgun
};

float crandom()
{
	return 2*(random() - 0.5);
};

/*
================
W_FireAxe
================
*/
/*
void W_FireAxe()
{
	vector  source;
	vector  org;

	makevectors (self.v_angle);
	source = self.origin + '0 0 16';
	traceline (source, source + v_forward*64, FALSE, self);
	if (trace_fraction == 1.0)
		return;
	
	org = trace_endpos - v_forward*4;

	if (trace_ent.takedamage)
	{
		trace_ent.axhitme = 1;
		SpawnBlood (org, 20);
		if (deathmatch > 3)
			T_Damage (trace_ent, self, self, 75);
		else
			T_Damage (trace_ent, self, self, 20);
	}
	else
	{       // hit wall
		sound (self, CHAN_WEAPON, "player/axhit2.wav", 1, ATTN_NORM);

		WriteByte (MSG_MULTICAST, SVC_TEMPENTITY);
		WriteByte (MSG_MULTICAST, TE_GUNSHOT);
		WriteByte (MSG_MULTICAST, 3);
		WriteCoord (MSG_MULTICAST, org_x);
		WriteCoord (MSG_MULTICAST, org_y);
		WriteCoord (MSG_MULTICAST, org_z);
		multicast (org, MULTICAST_PVS);
	}
};

//============================================================================

vec3_t wall_velocity()
{
	vec3_t    vel;
	
	vel = normalize (self.velocity);
	vel = normalize(vel + v_up*(random()- 0.5) + v_right*(random()- 0.5));
	vel = vel + 2*trace_plane_normal;
	vel = vel * 200;
	
	return vel;
};
*/

/*
================
SpawnMeatSpray
================
*/
/*
void SpawnMeatSpray(const idVec3 &org, const idVec3 &vel)
{
	CBaseEntity *missile = gpEngine->pfnSpawn();
	missile->SetOwner(self);
	missile->SetMoveType(MOVETYPE_BOUNCE);
	missile->SetSolidity(SOLID_NOT);

	gpEngine->pfnMakeVectors(self->angles);

	missile->SetVelocity(vel);
	missile->velocity_z = missile->GetVelocity().z + 250 + 50*random();

	missile->SetAngularVelocity(3000.0f, 1000.0f, 2000.0f);
	
	// set missile duration
	missile->SetNextThink(gpGlobals->time + 1);
	missile->SetThinkCallback(SUB_Remove);

	missile->SetModel("models/zom_gib.mdl");
	missile->SetSize('0 0 0', '0 0 0');            
	missile->SetOrigin(org);
};
*/

/*
================
spawn_touchblood
================
*/
/*
void spawn_touchblood(float damage)
{
	idVec3 vel{wall_velocity () * 0.2};
	mpWorld->SpawnBlood(self->GetOrigin() + vel * 0.01, damage);
};
*/

/*
==============================================================================

MULTI-DAMAGE

Collects multiple small damages into a single damage

==============================================================================
*/

//struct SMultiDamageVars
//{
CBaseEntity *multi_ent{nullptr};
float multi_damage{0.0f};
int multi_damage_type{0};
//};

//extern SMultiDamageVars gMultiDamage;

//idVec3 blood_org;
//float blood_count;

//idVec3 puff_org;
//float puff_count;

void ClearMultiDamage()
{
	multi_ent = nullptr;
	multi_damage = 0.0f;
	multi_damage_type = 0;
	
	//blood_count = 0;
	//puff_count = 0;
};

void ApplyMultiDamage(CBaseEntity *apInflictor, CBaseEntity *apAttacker)
{
	if(!multi_ent)
		return;
	multi_ent->TakeDamage(apInflictor, apAttacker, multi_damage, multi_damage_type);
};

void AddMultiDamage(CBaseEntity *apInflictor, CBaseEntity *hit, float damage, int anDmgType)
{
	if(!hit)
		return;
	
	multi_damage_type |= anDmgType;
	
	if(hit != multi_ent)
	{
		ApplyMultiDamage(apInflictor, apInflictor);
		multi_damage = 0.0f;
		multi_ent = hit;
	}
	else
		multi_damage += damage;
};

/*
void Multi_Finish()
{
	if(puff_count)
	{
		gpEngine->pfnWriteByte (MSG_MULTICAST, SVC_TEMPENTITY);
		gpEngine->pfnWriteByte (MSG_MULTICAST, TE_GUNSHOT);
		gpEngine->pfnWriteByte (MSG_MULTICAST, puff_count);
		gpEngine->pfnWriteCoord (MSG_MULTICAST, puff_org_x);
		gpEngine->pfnWriteCoord (MSG_MULTICAST, puff_org_y);
		gpEngine->pfnWriteCoord (MSG_MULTICAST, puff_org_z);
		multicast(puff_org, MULTICAST_PVS);
	};

	if(blood_count)
	{
		gpEngine->pfnWriteByte (MSG_MULTICAST, SVC_TEMPENTITY);
		gpEngine->pfnWriteByte (MSG_MULTICAST, TE_BLOOD);
		gpEngine->pfnWriteByte (MSG_MULTICAST, blood_count);
		gpEngine->pfnWriteCoord (MSG_MULTICAST, blood_org_x);
		gpEngine->pfnWriteCoord (MSG_MULTICAST, blood_org_y);
		gpEngine->pfnWriteCoord (MSG_MULTICAST, blood_org_z);
		multicast (puff_org, MULTICAST_PVS);
	};
};
*/

/*
==============================================================================

ROCKETS

==============================================================================
*/

/*
class CMissile : public CBaseEntity
{
public:
	void Spawn() override;
	
	void Touch(CBaseEntity *other) override;
};

void CMissile::Spawn()
{
	SetMoveType(MOVETYPE_FLYMISSILE);
	SetSolidity(SOLID_BBOX);

	// set speed
	
	SetTouchCallback(CMissile::Touch);
	self->voided = 0;
	
	// set duration
	SetNextThink(gpGlobals->time + 5);
	SetThinkCallback(SUB_Remove);

	SetModel("models/missile.mdl");
	SetSize(idVec3::Origin, idVec3::Origin);             
};

void CMissile::Touch(CBaseEntity *other)
{
	float     damg;

//	if (deathmatch == 4)
//	{
//	if ( ((other.weapon == 32) || (other.weapon == 16)))
//		{	
//			if (random() < 0.1)
//			{
//				if (other != world)
//				{
//	//				bprint (PRINT_HIGH, "Got here\n");
//					other.deathtype = "blaze";
//					T_Damage (other, self, self.owner, 1000 );
//					T_RadiusDamage (self, self.owner, 1000, other);
//				}
//			}
//		}	
//	}

	if (other == self->GetOwner())
		return;         // don't explode on owner

	if (self->voided)
		return;

	self->voided = 1;

	if(mpWorld->GetPointContents(GetOrigin()) == CONTENT_SKY)
	{
		gpEngine->pfnRemove(self);
		return;
	};

	damg = 100 + random()*20;
	
	if (other->GetHealth())
	{
		other->deathtype = "rocket";
		other->TakeDamage(self, self->owner, damg );
	};

	// don't do radius damage to the other, because all the damage
	// was done in the impact

	mpWorld->RadiusDamage (self, self->owner, 120, other, "rocket");

	//EmitSound(CHAN_WEAPON, "weapons/r_exp3.wav", 1, ATTN_NORM);
	SetOrigin(GetOrigin() - 8 * normalize(GetVelocity()));

	gpEngine->pfnWriteByte (MSG_MULTICAST, SVC_TEMPENTITY);
	gpEngine->pfnWriteByte (MSG_MULTICAST, TE_EXPLOSION);
	gpEngine->pfnWriteCoord (MSG_MULTICAST, self->GetOrigin().x);
	gpEngine->pfnWriteCoord (MSG_MULTICAST, self->GetOrigin().y);
	gpEngine->pfnWriteCoord (MSG_MULTICAST, self->GetOrigin().z);
	multicast (self->GetOrigin(), MULTICAST_PHS);

	gpEngine->pfnRemove(self);
};
*/

/*
================
W_FireRocket
================
*/
/*
void CWeaponRocketLauncher::PrimaryAttack()
{
	if (deathmatch != 4)
		GetOwner()->currentammo = GetOwner()->ammo_rockets = GetOwner()->ammo_rockets - 1;
	
	EmitSound (CHAN_WEAPON, "weapons/sgun1.wav", 1, ATTN_NORM);

	msg_entity = self;
	gpEngine->pfnWriteByte (MSG_ONE, SVC_SMALLKICK);

	newmis = mpWorld->SpawnEntity("rocket", self->GetOrigin() + v_forward * 8 + idVec3(0, 0, 16), nullptr, self);
	
	makevectors(self->v_angle);
	newmis->SetVelocity(aim(self, 1000));
	newmis->SetVelocity(newmis->GetVelocity() * 1000);
	newmis->SetAngles(vectoangles(newmis->GetVelocity()));
};
*/

/*
===============================================================================
LIGHTNING
===============================================================================
*/

/*
void LightningHit(CBaseEntity *hitme, CBaseEntity *from, float damage)
{
	WriteByte (MSG_MULTICAST, SVC_TEMPENTITY);
	WriteByte (MSG_MULTICAST, TE_LIGHTNINGBLOOD);
	WriteCoord (MSG_MULTICAST, trace_endpos_x);
	WriteCoord (MSG_MULTICAST, trace_endpos_y);
	WriteCoord (MSG_MULTICAST, trace_endpos_z);
	multicast (gpGlobals->trace_endpos, MULTICAST_PVS);

	// NOTE: hitme = gpGlobals->trace_ent
	hitme->T_Damage (from, from, damage);
};
*/

/*
=================
LightningDamage
=================
*/
/*
void LightningDamage(idVec3 p1, idVec3 p2, entity from, float damage)
{
	entity            e1, e2;
	vector            f;
	
	f = p2 - p1;
	normalize (f);
	f_x = 0 - f_y;
	f_y = f_x;
	f_z = 0;
	f = f*16;

	e1 = e2 = world;

	traceline (p1, p2, FALSE, self);

	if (trace_ent.takedamage)
	{
		LightningHit (from, damage);
		if (self.classname == "player")
		{
			if (other.classname == "player")
				trace_ent.velocity_z = trace_ent.velocity_z + 400;
		}
	}
	e1 = trace_ent;

	traceline (p1 + f, p2 + f, FALSE, self);
	if (trace_ent != e1 && trace_ent.takedamage)
	{
		LightningHit (from, damage);
	}
	e2 = trace_ent;

	traceline (p1 - f, p2 - f, FALSE, self);
	if (trace_ent != e1 && trace_ent != e2 && trace_ent.takedamage)
	{
		LightningHit (from, damage);
	}
};


void W_FireLightning()
{
	vector          org;
	float           cells;

	if (self.ammo_cells < 1)
	{
		self.weapon = W_BestWeapon ();
		W_SetCurrentAmmo ();
		return;
	}

// explode if under water
	if (self.waterlevel > 1)
	{
		if (deathmatch > 3)
		{
			if (random() <= 0.5)
			{
				self.deathtype = "selfwater";
				T_Damage (self, self, self.owner, 4000 );
			}
			else
			{
				cells = self.ammo_cells;
				self.ammo_cells = 0;
				W_SetCurrentAmmo ();
				T_RadiusDamage (self, self, 35*cells, world, "");
				return;
			}
		}
		else
		{
			cells = self.ammo_cells;
			self.ammo_cells = 0;
			W_SetCurrentAmmo ();
			T_RadiusDamage (self, self, 35*cells, world,"");
			return;
		}
	}

	if (self.t_width < time)
	{
		sound (self, CHAN_WEAPON, "weapons/lhit.wav", 1, ATTN_NORM);
		self.t_width = time + 0.6;
	}
	msg_entity = self;
	WriteByte (MSG_ONE, SVC_SMALLKICK);

	if (deathmatch != 4)
		self.currentammo = self.ammo_cells = self.ammo_cells - 1;

	org = self.origin + '0 0 16';
	
	traceline (org, org + v_forward*600, TRUE, self);

	WriteByte (MSG_MULTICAST, SVC_TEMPENTITY);
	WriteByte (MSG_MULTICAST, TE_LIGHTNING2);
	WriteEntity (MSG_MULTICAST, self);
	WriteCoord (MSG_MULTICAST, org_x);
	WriteCoord (MSG_MULTICAST, org_y);
	WriteCoord (MSG_MULTICAST, org_z);
	WriteCoord (MSG_MULTICAST, trace_endpos_x);
	WriteCoord (MSG_MULTICAST, trace_endpos_y);
	WriteCoord (MSG_MULTICAST, trace_endpos_z);
	multicast (org, MULTICAST_PHS);

	LightningDamage (self.origin, trace_endpos + v_forward*4, self, 30);
};


//=============================================================================


void GrenadeExplode()
{
	if (self.voided) {
		return;
	}
	self.voided = 1;

	mpWorld->RadiusDamage (self, self.owner, 120, world, "grenade");

	WriteByte (MSG_MULTICAST, SVC_TEMPENTITY);
	WriteByte (MSG_MULTICAST, TE_EXPLOSION);
	WriteCoord (MSG_MULTICAST, self.origin_x);
	WriteCoord (MSG_MULTICAST, self.origin_y);
	WriteCoord (MSG_MULTICAST, self.origin_z);
	multicast (self.origin, MULTICAST_PHS);

	remove (self);
};

void GrenadeTouch(CBaseEntity *other)
{
	if (other == self.owner)
		return;         // don't explode on owner
	if (other.takedamage == DAMAGE_AIM)
	{
		GrenadeExplode();
		return;
	}
	sound (self, CHAN_WEAPON, "weapons/bounce.wav", 1, ATTN_NORM);  // bounce sound
	if (self.velocity == '0 0 0')
		self.avelocity = '0 0 0';
};
*/

/*
================
W_FireGrenade
================
*/
/*
void W_FireGrenade()
{       
	if (deathmatch != 4)
		self.currentammo = self.ammo_rockets = self.ammo_rockets - 1;
	
	sound (self, CHAN_WEAPON, "weapons/grenade.wav", 1, ATTN_NORM);

	msg_entity = self;
	WriteByte (MSG_ONE, SVC_SMALLKICK);

	newmis = spawn ();
	newmis.voided=0;
	newmis.owner = self;
	newmis.movetype = MOVETYPE_BOUNCE;
	newmis.solid = SOLID_BBOX;
	newmis.classname = "grenade";
		
// set newmis speed     

	makevectors (self.v_angle);

	if (self.v_angle_x)
		newmis.velocity = v_forward*600 + v_up * 200 + crandom()*v_right*10 + crandom()*v_up*10;
	else
	{
		newmis.velocity = aim(self, 10000);
		newmis.velocity = newmis.velocity * 600;
		newmis.velocity_z = 200;
	}

	newmis.avelocity = '300 300 300';

	newmis.angles = vectoangles(newmis.velocity);
	
	newmis.touch = GrenadeTouch;
	
// set newmis duration
	if (deathmatch == 4)
	{
		newmis.nextthink = time + 2.5;		
		self.attack_finished = time + 1.1;
//		self.health = self.health - 1;
		T_Damage (self, self, self.owner, 10 );
	}
	else
		newmis.nextthink = time + 2.5;

	newmis.think = GrenadeExplode;

	setmodel (newmis, "models/grenade.mdl");
	setsize (newmis, '0 0 0', '0 0 0');             
	setorigin (newmis, self.origin);
};


//=============================================================================

void spike_touch(CBaseEntity *apOther);
void superspike_touch(CBaseEntity *apOther);
*/

/*
===============
launch_spike

Used for both the player and the ogre
===============
*/
/*
void launch_spike(const idVec3 &org, const idVec3 &dir)
{
	newmis = spawn ();
	newmis.voided=0;
	newmis.owner = self;
	newmis.movetype = MOVETYPE_FLYMISSILE;
	newmis.solid = SOLID_BBOX;

	newmis.angles = vectoangles(dir);
	
	newmis.touch = spike_touch;
	newmis.classname = "spike";
	newmis.think = SUB_Remove;
	newmis.nextthink = time + 6;
	setmodel (newmis, "models/spike.mdl");
	setsize (newmis, VEC_ORIGIN, VEC_ORIGIN);               
	setorigin (newmis, org);

	newmis.velocity = dir * 1000;
};

void W_FireSuperSpikes()
{
	vector    dir;
	entity    old;
	
	sound (self, CHAN_WEAPON, "weapons/spike2.wav", 1, ATTN_NORM);
	self.attack_finished = time + 0.2;
	if (deathmatch != 4) 
		self.currentammo = self.ammo_nails = self.ammo_nails - 2;
	dir = aim (self, 1000);
	launch_spike (self.origin + '0 0 16', dir);
	newmis.touch = superspike_touch;
	setmodel (newmis, "models/s_spike.mdl");
	setsize (newmis, VEC_ORIGIN, VEC_ORIGIN);               
	msg_entity = self;
	WriteByte (MSG_ONE, SVC_SMALLKICK);
};

void W_FireSpikes(float ox)
{
	vector    dir;
	entity    old;
	
	makevectors (self.v_angle);
	
	if (self.ammo_nails >= 2 && self.weapon == IT_SUPER_NAILGUN)
	{
		W_FireSuperSpikes ();
		return;
	}

	if (self.ammo_nails < 1)
	{
		self.weapon = W_BestWeapon ();
		W_SetCurrentAmmo ();
		return;
	}

	sound (self, CHAN_WEAPON, "weapons/rocket1i.wav", 1, ATTN_NORM);
	self.attack_finished = time + 0.2;
	if (deathmatch != 4)
		self.currentammo = self.ammo_nails = self.ammo_nails - 1;
	dir = aim (self, 1000);
	launch_spike (self.origin + '0 0 16' + v_right*ox, dir);

	msg_entity = self;
	WriteByte (MSG_ONE, SVC_SMALLKICK);
};

.float hit_z;
void spike_touch(CBaseEntity *other)
{
	float rand;
	
	if (other == self->GetOwner())
		return;

	if (self.voided)
		return;

	self->voided = 1;

	if (other->solid == SOLID_TRIGGER)
		return; // trigger field, do nothing

	if(gpEngine->pfnPointContents(GetOrigin()) == CONTENT_SKY)
	{
		gpEngine->pfnRemove(self);
		return;
	};
	
// hit something that bleeds
	if (other->takedamage)
	{
		spawn_touchblood (9);
		other->deathtype = "nail";
		other->TakeDamage(self, self->owner, 9);
	}
	else
	{
		WriteByte (MSG_MULTICAST, SVC_TEMPENTITY);
		if (self.classname == "wizspike")
			WriteByte (MSG_MULTICAST, TE_WIZSPIKE);
		else if (self.classname == "knightspike")
			WriteByte (MSG_MULTICAST, TE_KNIGHTSPIKE);
		else
			WriteByte (MSG_MULTICAST, TE_SPIKE);
		WriteCoord (MSG_MULTICAST, self.origin_x);
		WriteCoord (MSG_MULTICAST, self.origin_y);
		WriteCoord (MSG_MULTICAST, self.origin_z);
		multicast (self.origin, MULTICAST_PHS);
	}

	remove(self);

};

void superspike_touch(CBaseEntity *other)
{
	float rand;
	
	if (other == self.owner)
		return;

	if (self.voided) {
		return;
	}
	self.voided = 1;


	if (other.solid == SOLID_TRIGGER)
		return; // trigger field, do nothing

	if (pointcontents(self.origin) == CONTENT_SKY)
	{
		remove(self);
		return;
	}
	
// hit something that bleeds
	if (other.takedamage)
	{
		spawn_touchblood (18);
		other.deathtype = "supernail";
		T_Damage (other, self, self.owner, 18);
	}
	else
	{
		WriteByte (MSG_MULTICAST, SVC_TEMPENTITY);
		WriteByte (MSG_MULTICAST, TE_SUPERSPIKE);
		WriteCoord (MSG_MULTICAST, self.origin_x);
		WriteCoord (MSG_MULTICAST, self.origin_y);
		WriteCoord (MSG_MULTICAST, self.origin_z);
		multicast (self.origin, MULTICAST_PHS);
	}

	remove(self);
};
*/
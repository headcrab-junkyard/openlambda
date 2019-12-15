/// @file

#include "BaseMonster.hpp"

class CMonsterHoundEye : public CBaseMonster
{
public:
	void Spawn() override;
	
	int TakeDamage(CBaseEntity *apInflictor, CBaseEntity *apAttacker, float afDamage, int anDmgBitSum) override;
};

LINK_ENTITY_TO_CLASS(monster_houndeye, CMonsterHoundEye)

void CMonsterHoundEye::Spawn()
{
};

int CMonsterHoundEye::TakeDamage(CBaseEntity *apInflictor, CBaseEntity *apAttacker, float afDamage, int anDmgBitSum)
{
	return 0;
};
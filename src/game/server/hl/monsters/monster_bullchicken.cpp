/// @file

#include "BaseMonster.hpp"

class CMonsterBullChicken : public CBaseMonster
{
public:
	void Spawn() override;
	
	int TakeDamage(CBaseEntity *apInflictor, CBaseEntity *apAttacker, float afDamage, int anDmgBitSum) override;
};

LINK_ENTITY_TO_CLASS(monster_bullchicken, CMonsterBullChicken)
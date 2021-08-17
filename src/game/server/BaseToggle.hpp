/// @file

#include "BaseDelay.hpp"

class CBaseToggle : public CBaseDelay
{
public:
	bool HandleKeyValue(KeyValueData *apKVData) override;
	
	int GetState() const {return mnState;}
	float GetDelay() const {return mfDelay;}
	
	void LinearMove(const idVec3 &tdest, float tspeed); // TODO: was SUB_CalcMove
	void LinearMoveDone(); // TODO: was SUB_CalcMoveDone
	
	void SUB_CalcAngleMove(const idVec3 &destangle, float tspeed); // TODO: AngularMove
	void SUB_CalcAngleMoveDone(); // TODO: AngularMoveDone
	
	bool IsLockedByMaster() const;
private:
	idVec3 mvFinalDest{idVec3::Origin};
	idVec3 mvFinalAngle{idVec3::Origin};
	
	void (CBaseToggle::*mpfnMoveDoneCallback)();
	
	float mfDelay{0.0f};
	
	int mnState{-1};
};
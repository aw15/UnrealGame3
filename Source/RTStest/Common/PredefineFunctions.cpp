#include "PredefineFunctions.h"
#include "PredefineMacro.h"

bool CheckFunc::IsPlayerIndex(const int OwnerIndex)
{
	return OwnerIndex == PLAYER_INDEX; 
}


bool CheckFunc::IsNotPlayerIndex(const int OwnerIndex)
{
	return OwnerIndex != PLAYER_INDEX; 
}

int CalcFunc::ConvertUnitScale(const int TroopCount)
{
	if(TroopCount < 5000)
		return UNIT_SCALE_1;

	if(TroopCount < 10000)
		return UNIT_SCALE_2;

	if(TroopCount < 20000)
		return UNIT_SCALE_3;

	if(TroopCount < 30000)
		return UNIT_SCALE_4;
	
	return UNIT_SCALE_5;
}

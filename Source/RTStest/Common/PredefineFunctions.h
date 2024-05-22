#pragma once

namespace CheckFunc
{
	bool IsPlayerIndex(const int OwnerIndex);
	bool IsNotPlayerIndex(const int OwnerIndex);
}

namespace CalcFunc
{
	int ConvertUnitScale(const int TroopCount);
}

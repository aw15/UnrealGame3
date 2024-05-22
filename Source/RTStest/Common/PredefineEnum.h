#pragma once

UENUM()
enum class EPlayerColor
{
	Black,
	Blue,
	Brown,
	Green,
	Pink,
	Purple,
	Red,
	Tan,
	White,
	Yellow
};

UENUM(BlueprintType)
enum class EUnitControl: uint8
{
	Move,
	MoveOrAttack,
	Wait
};

UENUM(BlueprintType)
enum class EUnitState: uint8
{
	None = 0,
	Move = 1,
	AttackUnit = 2,
	Dead = 3
};

UENUM(BlueprintType)
enum class EUnitAnimState: uint8
{
	Attack = 0,
	Idle = 1
};
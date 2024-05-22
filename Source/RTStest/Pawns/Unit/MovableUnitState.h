#pragma once
#include "RTStest/Common/PredefineEnum.h"

enum class EStateClassType
{
	Base,
	Idle,
	Attack,
	InputMove
};

class AMovableUnit;

class FMovableUnitState
{
protected:
	virtual EStateClassType GetStateClassType() { return EStateClassType::Base; }
public:
	virtual ~FMovableUnitState() = default;

	virtual bool Enter(AMovableUnit*, FMovableUnitState*) { return true; }
	virtual void ReEnter(AMovableUnit*) {}
	virtual void Exit(AMovableUnit*) {}
	virtual EUnitAnimState GetAnimState() { return  EUnitAnimState::Idle; }
	virtual EUnitState GetUnitState() { return EUnitState::None; }
	static void SetUnitState(const AMovableUnit* MovableUnit, const EUnitState InUnitState);
	static void ClearBlackBoardValue(const AMovableUnit* MovableUnit, const FName& BlackBoardKey);
};

class FIdleState final : public FMovableUnitState
{
protected:
	virtual EStateClassType GetStateClassType() override { return EStateClassType::Idle; }
public:
	virtual bool Enter(AMovableUnit*, FMovableUnitState*) override;
	virtual void Exit(AMovableUnit*) override {}
	virtual EUnitAnimState GetAnimState() override { return EUnitAnimState::Idle; };
	virtual EUnitState GetUnitState() override { return EUnitState::None; };

public:
	static FIdleState State;
};

class FAttackUnitState final : public FMovableUnitState
{
protected:
	virtual EStateClassType GetStateClassType() override { return EStateClassType::Attack; }
public:
	virtual bool Enter(AMovableUnit*, FMovableUnitState*) override;
	virtual void Exit(AMovableUnit*) override;
	virtual EUnitAnimState GetAnimState() override { return EUnitAnimState::Attack; }
	virtual EUnitState GetUnitState() override { return EUnitState::AttackUnit; };

	static FAttackUnitState State;
};

class FInputMoveState final : public FMovableUnitState
{
protected:
	virtual EStateClassType GetStateClassType() override { return EStateClassType::InputMove; }
public:
	virtual bool Enter(AMovableUnit*, FMovableUnitState*) override;
	virtual void Exit(AMovableUnit*) override;
	virtual void ReEnter(AMovableUnit*) override;
	virtual EUnitAnimState GetAnimState() override { return EUnitAnimState::Idle; };
	virtual EUnitState GetUnitState() override { return EUnitState::Move; };

public:
	static FInputMoveState State;
};

class FStateManager
{
public:
	void ChangeState(FMovableUnitState*, AMovableUnit* MovableUnit);
	void ChangeAnimState(const EUnitAnimState InUnitAnimState);
	void RollbackToPrevState(AMovableUnit* MovableUnit);
	FMovableUnitState* GetState() const { return CurrentState; } //Null이 아님을 보장.
	EUnitAnimState GetAnimState() const {return CurrentAnimState;}

private:
	FMovableUnitState* CurrentState = &FIdleState::State;
	EUnitAnimState CurrentAnimState = EUnitAnimState::Idle;
	TArray<FMovableUnitState*> PrevStateStack;
};

#include "MovableUnitState.h"

#include "MovableUnit.h"
#include "UnitAIControllerBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RTStest/Pawns/PlayerControllerBase.h"
#include "RTStest/Common/BlackboardPredefine.h"

FIdleState FIdleState::State;
FAttackUnitState FAttackUnitState::State;
FInputMoveState FInputMoveState::State;

void FMovableUnitState::SetUnitState(const AMovableUnit* MovableUnit, const EUnitState InUnitState)
{
	if(MovableUnit && MovableUnit->GetAiController() && MovableUnit->GetAiController()->GetBlackboard())
	{
		MovableUnit->GetAiController()->GetBlackboard()->SetValueAsInt(BlackboardKeys::UnitAI::CurrentState, static_cast<int>(InUnitState));
	}
}

void FMovableUnitState::ClearBlackBoardValue(const AMovableUnit* MovableUnit, const FName& BlackBoardKey)
{
	if(MovableUnit && MovableUnit->GetAiController() && MovableUnit->GetAiController()->GetBlackboard())
	{
		MovableUnit->GetAiController()->GetBlackboard()->ClearValue(BlackBoardKey);
	}
}

bool FIdleState::Enter(AMovableUnit* MovableUnit, FMovableUnitState* PrevState)
{
	return true;	
}

bool FAttackUnitState::Enter(AMovableUnit* MovableUnit, FMovableUnitState* PrevState)
{
	if(!MovableUnit || !MovableUnit->GetTargetEnemy())
		return false;

	if(!MovableUnit->GetAiController() || !MovableUnit->GetAiController()->GetBlackboard())
		return false;
	
	SetUnitState(MovableUnit,EUnitState::AttackUnit);
	MovableUnit->GetAiController()->GetBlackboard()->SetValueAsObject(BlackboardKeys::UnitAI::TargetEnemy, MovableUnit->GetTargetEnemy()->_getUObject());
	return true;
}

void FAttackUnitState::Exit(AMovableUnit* MovableUnit)
{
	ClearBlackBoardValue(MovableUnit, BlackboardKeys::UnitAI::TargetEnemy);
}

bool FInputMoveState::Enter(AMovableUnit* MovableUnit, FMovableUnitState* PrevState)
{
	if(!MovableUnit)
		return false;

	if(!MovableUnit->GetPlayerController() || !MovableUnit->GetAiController() || !MovableUnit->GetAiController()->GetBlackboard())
		return false;
	
	SetUnitState(MovableUnit, EUnitState::Move);
	MovableUnit->GetAiController()->GetBlackboard()->SetValueAsVector(BlackboardKeys::UnitAI::TargetLocation, MovableUnit->GetPlayerController()->LastRightMouseHitLocation);

	return true;
}

void FInputMoveState::Exit(AMovableUnit* MovableUnit)
{
	//ClearBlackBoardValue(MovableUnit, BlackboardKeys::UnitAI::TargetLocation);
}

void FInputMoveState::ReEnter(AMovableUnit* MovableUnit)
{
	SetUnitState(MovableUnit, EUnitState::Move);
}

void FStateManager::ChangeState(FMovableUnitState* InUnitState, AMovableUnit* MovableUnit)
{
	if(!InUnitState)
		return;

	if(InUnitState == CurrentState)
		return;

	//이전 상태 종료처리
	CurrentState->Exit(MovableUnit);
	if(InUnitState->Enter(MovableUnit, CurrentState))
	{
		//이전 상태 보관
		if(PrevStateStack.Num() > 5)//최대 5개까지 보관.
		{
			PrevStateStack.RemoveAt(0);
		}
		PrevStateStack.Add(CurrentState);
		//현재 상태 변경
		CurrentState = InUnitState;
	}
}

void FStateManager::ChangeAnimState(const EUnitAnimState InUnitAnimState)
{
	CurrentAnimState = InUnitAnimState;
}

void FStateManager::RollbackToPrevState(AMovableUnit* MovableUnit)
{
	if(PrevStateStack.Num() > 0)
	{
		PrevStateStack.Last()->ReEnter(MovableUnit);
		PrevStateStack.RemoveAt(0);
	}
}

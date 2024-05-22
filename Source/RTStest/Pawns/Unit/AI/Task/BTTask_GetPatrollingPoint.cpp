// Fill out your copyright notice in the Description page of Project Settings.


#include "RTStest/Pawns/Unit/AI/Task/BTTask_GetPatrollingPoint.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/Decorators/BTDecorator_Blackboard.h"
#include "RTStest/Pawns/Unit/MovableUnit.h"
#include "RTStest/Pawns/Unit/AI/PatrolPath.h"

UBTTask_GetPatrollingPoint::UBTTask_GetPatrollingPoint(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Get Patrolling Point";
}

EBTNodeResult::Type UBTTask_GetPatrollingPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* BlackboardRef = OwnerComp.GetBlackboardComponent();
	AAIController* MyController = OwnerComp.GetAIOwner();
	if(!MyController || !BlackboardRef)
		return EBTNodeResult::Failed;

	AMovableUnit* Unit = Cast<AMovableUnit>(MyController->GetPawn());
	if(!Unit)
		return EBTNodeResult::Failed;

	const APatrolPath* PathRef = nullptr;//Cast<APatrolPath>(Unit->GetPatrolSmartObject());
	if(!PathRef || PathRef->Locations.Num() < 1)
		return EBTNodeResult::Succeeded;

	OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>("MoveToLocation", PathRef->Locations[Index]);

	if(Index < PathRef->Locations.Num() - 1)
	{
		++Index;
		return EBTNodeResult::Succeeded;
	}

	Index = 0;
	return  EBTNodeResult::Succeeded;
	
}


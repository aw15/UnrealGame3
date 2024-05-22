// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitAIControllerBase.h"

#include "MovableUnit.h"
#include "AI/SmartObject.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

AUnitAIControllerBase::AUnitAIControllerBase(FObjectInitializer const& ObjInitializer)
{
	BehaviorTreeComponent = ObjInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComponent"));
	UnitBlackboardRef = ObjInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("DefaultBlackboard"));

	//AI Sense
	// AiPerceptionComponent = ObjInitializer.CreateDefaultSubobject<UAIPerceptionComponent>(this, TEXT("AIPerceptionComp"));
	// Sight = ObjInitializer.CreateDefaultSubobject<UAISenseConfig_Sight>(this, TEXT("Sight Config"));
	//
	// Sight->SightRadius = 2000.f;
	// Sight->LoseSightRadius = Sight->SightRadius + 500.f;
	// Sight->PeripheralVisionAngleDegrees = 90.f;
	// Sight->DetectionByAffiliation.bDetectNeutrals = false;
	// Sight->DetectionByAffiliation.bDetectEnemies = true;
	// Sight->DetectionByAffiliation.bDetectFriendlies = false;
	//
	// AiPerceptionComponent->ConfigureSense(*Sight);
	// AiPerceptionComponent->SetDominantSense(Sight->GetSenseImplementation());
}

void AUnitAIControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AUnitAIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	AMovableUnit* MovableUnit = Cast<AMovableUnit>(GetPawn());
	if(MovableUnit)
	{
		ControlledUnit = MovableUnit;
	}

	//AiPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AUnitAIControllerBase::OnPerception);

	if(DefaultBTRef)
	{
		if(UnitBlackboardRef)
		{
			UnitBlackboardRef->InitializeBlackboard(*DefaultBTRef->BlackboardAsset);
		}
		RunBehaviorTree(DefaultBTRef);
		BehaviorTreeComponent->StartTree(*DefaultBTRef);
	}	
}

UBlackboardComponent* AUnitAIControllerBase::GetBlackboard() const
{
	return UnitBlackboardRef;
}

// void AUnitAIControllerBase::OnPerception(AActor* Actor, FAIStimulus Stimulus)
// {
// 	//TODO 공격 방식 개선 필요
// 	const bool bSighted = Stimulus.WasSuccessfullySensed();
// 	const auto EnemyUnit = Cast<AMovableUnit>(Actor);
// 	if(EnemyUnit)
// 	{
// 		if(bSighted)
// 		{
// 			ControlledUnit->MoveToEnemy(EnemyUnit);
// 			EnemyUnit->MoveToEnemy(ControlledUnit);
// 			//이미 공격 대상이 있는 공격 상태면 무차별하게 공격대상을 바꾸는 걸 막기 위해 return
// 			// if(ControlledUnit->GetUnitState() != EUnitState::Attack)
// 			// {
// 			// 	ControlledUnit->MoveToEnemy(EnemyUnit);
// 			// }
// 			// if(EnemyUnit->GetUnitState() != EUnitState::Attack)
// 			// {
// 			// 	//적도 나에게 오도록
// 			// 	EnemyUnit->MoveToEnemy(ControlledUnit);
// 			// }
// 		}
// 	}
// }

// ETeamAttitude::Type AUnitAIControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
// {
// 	auto const OtherUnit = Cast<AMovableUnit>(&Other);
// 	if(!OtherUnit)
// 		return ETeamAttitude::Neutral;
// 	if(!ControlledUnit)
// 		return ETeamAttitude::Neutral;
// 	
// 	if(OtherUnit->GetOwnerPlayerIndex() ==  ControlledUnit->GetOwnerPlayerIndex())
// 		return ETeamAttitude::Friendly;
//
// 	return ETeamAttitude::Hostile;
// }

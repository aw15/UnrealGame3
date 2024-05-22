// Fill out your copyright notice in the Description page of Project Settings.
#include "MovableUnit.h"


#include "CavalryClassComponent.h"
#include "RTStest/Common/BlackboardPredefine.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "UnitAIControllerBase.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "RTStest/Common/PredefineFunctions.h"
#include "RTStest/Common/PredefineMacro.h"
#include "RTStest/Data/DataManager.h"
#include "RTStest/Hud/InGameHudBase.h"
#include "RTStest/Pawns/PlayerControllerBase.h"
#include "RTStest/Quest/QuestSubject.h"
#include "RTStest/Enviroment/BuildingSlot.h"


// Sets default values
AMovableUnit::AMovableUnit()
{
	UnitType = EBaseUnitType::Movable;
	StateManager.ChangeAnimState(EUnitAnimState::Idle);
	
	AIControllerClass = AUnitAIControllerBase::StaticClass();

	AttackRangeBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackRangeBoxComponent"));
	AttackRangeBoxComponent->SetupAttachment(RootComponent);

	DetectRangeBoxComponent = CreateDefaultSubobject<USphereComponent>(TEXT("DetectBoxComponent"));
	DetectRangeBoxComponent->SetupAttachment(RootComponent);

	ClickDetectionBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ClickDetectionBoxComponent"));
	ClickDetectionBoxComponent->SetupAttachment(RootComponent);
	
	SelectedDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectedDecal"));
    SelectedDecal->SetupAttachment(RootComponent);
	SubSkeletalMesh1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SubMesh1"));
	SubSkeletalMesh1->SetupAttachment(RootComponent);
	SubSkeletalMesh2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SubMesh2"));
	SubSkeletalMesh2->SetupAttachment(RootComponent);
	SubSkeletalMesh3 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SubMesh3"));
	SubSkeletalMesh3->SetupAttachment(RootComponent);
	SubSkeletalMesh4 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SubMesh4"));
	SubSkeletalMesh4->SetupAttachment(RootComponent);
	SubSkeletalMesh5 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SubMesh5"));
	SubSkeletalMesh5->SetupAttachment(RootComponent);

	PlayerColorDataTable = UDataManager::LoadDataTable(EDataType::PlayerColor);
	MovableUnitDataTable = UDataManager::LoadDataTable(EDataType::MovableUnit);
}

EUnitAnimState AMovableUnit::GetAnimState() const
{
	return StateManager.GetAnimState();
}

EUnitState AMovableUnit::GetUnitState() const
{
	return StateManager.GetState()->GetUnitState();
}

bool AMovableUnit::CanProcessQuest() const
{
	if(!ParentBuildingSlotProxy.IsValid() ||
		!CurrentQuestObject.QuestSubject ||
		!ParentBuildingSlotProxy.GetLeaderUnit())
			return false;

	return true;
}

APlayerControllerBase* AMovableUnit::GetPlayerController() const
{
	GetInstance_Ret(APlayerControllerBase, PlayerControllerBase, GetWorld(), nullptr)
	return PlayerControllerBase;
}

IHasOwnerActor* AMovableUnit::GetTargetEnemy() const
{
	return TargetEnemy.Get();
}

// Called when the game starts or when spawned
void AMovableUnit::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;
	SelectedDecal->SetVisibility(false);
	AttackRangeBoxComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AMovableUnit::OnBeginAttackRangeBoxOverlap);
	AttackRangeBoxComponent->OnComponentEndOverlap.AddUniqueDynamic(this, &AMovableUnit::OnEndAttackRangeBoxOverlap);
}

void AMovableUnit::RotateWhenMovingComplete(const float InYaw) const
{
	if(AiControllerRef)
	{
		AiControllerRef->GetBlackboard()->SetValueAsFloat(BlackboardKeys::UnitAI::RotationWhenMovingComplete, InYaw);
	}
}

void AMovableUnit::SetParentBuildingSlotProxy(const FSpawnedUnitBuildingSlotProxy& InBuildingUnitProxy)
{
	InBuildingUnitProxy.DeepCopy(ParentBuildingSlotProxy);
}

void AMovableUnit::SetMeshByData() const
{
	if(!PlayerColorDataTable)
		return;
	
	const auto ColorData = PlayerColorDataTable->FindRow<FPlayerColorData>(OwnerPlayerData.PlayerColorDataKey, TEXT("ColorDataFind"));
	GetMesh()->SetSkeletalMesh(MovableUnitData.MainMesh);
	SubSkeletalMesh1->SetSkeletalMesh(MovableUnitData.MainMesh);
	SubSkeletalMesh2->SetSkeletalMesh(MovableUnitData.MainMesh);
	SubSkeletalMesh3->SetSkeletalMesh(MovableUnitData.MainMesh);
	SubSkeletalMesh4->SetSkeletalMesh(MovableUnitData.MainMesh);
	SubSkeletalMesh5->SetSkeletalMesh(MovableUnitData.MainMesh);

	if(ColorData)
	{
		GetMesh()->SetMaterial(0, ColorData->MovableUnitColorMaterial);
	}
}

void AMovableUnit::SetRangeBoxByData() const
{
	DetectRangeBoxComponent->SetRelativeLocation(MovableUnitData.DetectBoxPosition);
	DetectRangeBoxComponent->SetSphereRadius(MovableUnitData.DetectBoxRadius);
	AttackRangeBoxComponent->SetRelativeLocation(MovableUnitData.AttackRangeBoxPosition);
	AttackRangeBoxComponent->SetBoxExtent(MovableUnitData.AttackRangeBoxExtent);
}
void AMovableUnit::StartBT()
{
	bCanStartBT = true;
}

void AMovableUnit::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AiControllerRef = Cast<AUnitAIControllerBase>(NewController);
	GetInstance_Void(APlayerControllerBase, OutPlayerControllerRef, GetWorld());
	PlayerControllerRef = OutPlayerControllerRef;
}

void AMovableUnit::SearchEnemyTimerEvent()
{
	TArray<AActor*> OverlappingActors; 
	DetectRangeBoxComponent->GetOverlappingActors(OverlappingActors, AActor::StaticClass());
	
	AActor* NearestActor = nullptr;
	AActor* NearestMovableUnit = nullptr;
	float DistanceFromNearestActor = TNumericLimits<float>::Max();

	for (AActor* ActorToCheck : OverlappingActors)
	{
		const IHasOwnerActor* HasOwnerActor = Cast<IHasOwnerActor>(ActorToCheck);
		if(!HasOwnerActor)
			continue;
		if(ActorToCheck == this)
			continue;
		if(HasOwnerActor->GetOwnerPlayerIndex() == GetOwnerPlayerIndex())
			continue;
		
		if (ActorToCheck)
		{
			const float DistanceFromActorToCheck = (GetActorLocation() - ActorToCheck->GetActorLocation()).SizeSquared();
			if (DistanceFromActorToCheck < DistanceFromNearestActor)
			{
				NearestActor = ActorToCheck;
				DistanceFromNearestActor = DistanceFromActorToCheck;
				if(HasOwnerActor->UnitType == EBaseUnitType::Movable)
				{
					NearestMovableUnit = ActorToCheck;
				}
			}
		}
	}
	//TODO 꼭 찾아야 하나?
	if(NearestActor)
	{
		if(NearestMovableUnit)//적유닛 우선순위.
		{
			//MoveToTargetLocation(NearestMovableUnit->GetActorLocation());	
		}
		else
		{
			//MoveToTargetLocation(NearestActor->GetActorLocation());
		}
	}
}

void AMovableUnit::CompleteAttackBuildingQuest()
{
	CurrentQuestObject.CompleteQuest();
	StateManager.ChangeAnimState(EUnitAnimState::Idle);
}

void AMovableUnit::ProcessEngineEvent(const EEngineEvent& EngineEvent)
{
	switch (EngineEvent)
	{
	case EEngineEvent::LeftMousePressed:
		{
			if(CheckFunc::IsPlayerIndex(GetOwnerPlayerIndex()))
			{
				GetInstance_Void(APlayerControllerBase, PlayerControllerBase, GetWorld());
				PlayerControllerBase->SetLastSelectedMovableUnit(this);
				SelectedDecal->SetVisibility(true);
			}
		}
		break;
	case EEngineEvent::RightMousePressed:
		{
			if(CheckFunc::IsPlayerIndex(GetOwnerPlayerIndex()))
			{
				StateManager.ChangeState(&FInputMoveState::State, this);
			}
		}
	case EEngineEvent::RightMousePressedOnUnit:
		{
			GetInstance_Void(APlayerControllerBase, PlayerControllerBase, GetWorld());
			if(PlayerControllerBase->GetLastSelectedMovableUnit())
			{
				PlayerControllerBase->GetLastSelectedMovableUnit()->SetQuestTarget(this);
			}
		}
		break;
	default:;
	}
}

void AMovableUnit::SetQuestTarget(AActor* ActorRef)
{
	if(this == ActorRef)
		return;

	const auto TargetUnitRef = Cast<IHasOwnerActor>(ActorRef);
	if(!TargetUnitRef)
		return;

	if(CheckFunc::IsPlayerIndex(TargetUnitRef->GetOwnerPlayerIndex()))
	{
		StateManager.ChangeState(&FInputMoveState::State, this);	
	}
	else
	{
		SetTargetEnemy(TargetUnitRef);
		StateManager.ChangeState(&FAttackUnitState::State, this);	
	}
}

void AMovableUnit::OnSelectionCancel() const
{
	SelectedDecal->SetVisibility(false);
}

void AMovableUnit::SetMovableUnitDataKey(const FName& InMovableUnitDataKey)
{
	MovableUnitDataKey = InMovableUnitDataKey;
	if(!MovableUnitDataTable)
		return;

	const FMovableUnitData* MovableUnitDataPtr = MovableUnitDataTable->FindRow<FMovableUnitData>(InMovableUnitDataKey, TEXT("MovableUnitData Fail"));
	if(MovableUnitDataPtr)
	{
		MovableUnitData = *MovableUnitDataPtr;
		//TODO 다른 유닛 타입 추가 필요
		switch (MovableUnitDataPtr->UnitType)
		{
		case EMovableUnitType::None: break;
		case EMovableUnitType::Worker: break;
		case EMovableUnitType::Infantry:
			{
			}
			break;
		case EMovableUnitType::Cavalry:
			{
				UnitClassComponent = NewObject<UCavalryClassComponent>(this,UCavalryClassComponent::StaticClass());		
			}
			break;
		default: ;
		}

		//공격 쿨타임 타이머 생성.
		GetWorldTimerManager().SetTimer(AttackCoolTimeTimer, this, &AMovableUnit::OnResetAttackCoolTime, MovableUnitData.AttackCoolTime, true);

		//근처 적 탐지 타이머 생성
		GetWorldTimerManager().SetTimer(NoiseEventTimerHandle, this, &AMovableUnit::SearchEnemyTimerEvent, MovableUnitData.SearchEnemyRate, true);


		SetMeshByData();
		SetRangeBoxByData();
	}
}

void AMovableUnit::SetUnitLevelAndCalculateUnitStat(const int Level)
{
	CurrentMaxHp = MovableUnitData.Hp + (MovableUnitData.HpUpgradeAmount * Level);
	CurrentHp = CurrentMaxHp;
	CurrentNormalDamage = MovableUnitData.NormalDamage + (MovableUnitData.NormalDamageUpgradeAmount * Level);
}

void AMovableUnit::SetOwnerPlayerData(const FOwnerPlayerData& InOwnerPlayerData)
{
	OwnerPlayerData = InOwnerPlayerData;
}

void AMovableUnit::SetTroopCount(const int InTroopCount)
{
	TroopCount = InTroopCount;
	HideAllSubMesh();
	//TroopScale에 따라 메시 개수 설정.
	const int UnitScale = CalcFunc::ConvertUnitScale(InTroopCount);
	if(UnitScale >= UNIT_SCALE_2)
	{
		SubSkeletalMesh1->SetVisibility(true);
		SubSkeletalMesh2->SetVisibility(true);
	}

	if(UnitScale >= UNIT_SCALE_3)
	{
		SubSkeletalMesh4->SetVisibility(true);
	}

	if(UnitScale >= UNIT_SCALE_4)
	{
		SubSkeletalMesh3->SetVisibility(true);
	}

	if(UnitScale >= UNIT_SCALE_5)
	{
		SubSkeletalMesh5->SetVisibility(true);
	}
}

void AMovableUnit::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

// Called every frame
void AMovableUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMovableUnit::Dead()
{
	//유닛이 죽었으므로 퀘스트 완료 이벤트에 바인딩된 유닛지우기. 
	if(CurrentQuestObject.QuestSubject)
	{
		CurrentQuestObject.QuestSubject->OnQuestComplete.RemoveAll(this);
	}
	//TargetEnemy쪽에 연결된 델리게이트로 제거
	if(TargetEnemy.IsValid())
	{
		TargetEnemy->OnUnitDead.RemoveAll(this);
	}
	OnUnitDead.Broadcast(this);
	Destroy();
}

float AMovableUnit::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                               AActor* DamageCauser)
{
	const float Damage =  Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	CurrentHp -= Damage;

	if(CurrentHp <= 0)
	{
		Dead();
	}
	
	return 0;
}
void AMovableUnit::OnResetAttackCoolTime()
{
	TArray<AActor*> OutActors;
	
	//TODO DamageType 개선 필요.
	if(TargetEnemy.IsValid())
	{
		AttackRangeBoxComponent->GetOverlappingActors(OutActors, AActor::StaticClass());
		
		for (AActor* ActorRef : OutActors)
		{
			if(ActorRef == TargetEnemy.GetObject())
			{
				UGameplayStatics::ApplyDamage(ActorRef, CurrentNormalDamage, GetController(), this, UDamageType::StaticClass());
				return;
			}
		}
	}
}

void AMovableUnit::SetTargetEnemy(IHasOwnerActor* UnitRef)
{
	if(!UnitRef || UnitRef->_getUObject() == TargetEnemy.GetObject())
		return;

	if(TargetEnemy.IsValid())
	{
		TargetEnemy->OnUnitDead.Remove(TargetEnemyDeadHandle);
	}
	
	TargetEnemy = UnitRef;
	TargetEnemyDeadHandle = TargetEnemy->OnUnitDead.AddUObject(this, &AMovableUnit::OnTargetEnemyDead);
}

void AMovableUnit::OnTargetEnemyDead(const TScriptInterface<IHasOwnerActor>& DeadUnitRef)
{
	StateManager.RollbackToPrevState(this);
	// TArray<AActor*> OutMovableUnit;
	// AttackRangeBoxComponent->GetOverlappingActors(OutMovableUnit, AMovableUnit::StaticClass());
	//
	// bool bNoEnemyNearby = true;
	// for (AActor* ActorRef : OutMovableUnit)
	// {
	// 	AMovableUnit* MovableUnitRef = Cast<AMovableUnit>(ActorRef);
	// 	if(MovableUnitRef &&
	// 		TargetEnemy != MovableUnitRef &&  //현재 타겟인 적이 아니고
	// 		MovableUnitRef->GetOwnerPlayerIndex() != GetOwnerPlayerIndex()) //적 유닛일 경우
	// 	{
	// 		SetTargetEnemy(MovableUnitRef);
	// 		bNoEnemyNearby = false;
	// 		break;
	// 	}
	// }
	//
	// //근처에 적이 하나도 없는 경우
	// if(bNoEnemyNearby)
	// {
	// 	StateManager.ChangeAnimState(EUnitAnimState::Idle);
	//
	// 	//TODO 죽고나서 어떻게 할지 필요.
	// 	
	// 	TargetEnemy.Reset();
	// }
}

void AMovableUnit::OnTargetBuildingDestroyed()
{
	CompleteAttackBuildingQuest();
}

void AMovableUnit::HideAllSubMesh()
{
	SubSkeletalMesh1->SetVisibility(false, true);
	SubSkeletalMesh2->SetVisibility(false, true);
	SubSkeletalMesh3->SetVisibility(false, true);
	SubSkeletalMesh4->SetVisibility(false, true);
	SubSkeletalMesh5->SetVisibility(false, true);
}

void AMovableUnit::OnBeginAttackRangeBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor)
		return;
	
	if(TargetEnemy.GetObject() == OtherActor)
	{
		StateManager.ChangeAnimState(EUnitAnimState::Attack);
		StateManager.ChangeState(&FAttackUnitState::State, this);
		return;
	}

	const ABuildingSlot* Building = Cast<ABuildingSlot>(OtherActor);
	if(!Building)
		return;
	if(Building->GetOwnerPlayerIndex() == GetOwnerPlayerIndex())
		return;
	if(Building->IsEnable())
	{
		StateManager.ChangeAnimState(EUnitAnimState::Attack);
		StateManager.ChangeState(&FAttackUnitState::State, this);
		return;
	} 
}

void AMovableUnit::OnEndAttackRangeBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	StateManager.ChangeAnimState(EUnitAnimState::Idle);
	//TODO 잘 작동되나?
	StateManager.RollbackToPrevState(this);
}


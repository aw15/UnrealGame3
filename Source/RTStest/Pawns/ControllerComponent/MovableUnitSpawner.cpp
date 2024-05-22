// Fill out your copyright notice in the Description page of Project Settings.


#include "MovableUnitSpawner.h"
#include "RTStest/Pawns/PlayerControllerBase.h"
#include "RTStest/Pawns/Unit/MovableUnit.h"


// Sets default values for this component's properties
UMovableUnitSpawner::UMovableUnitSpawner()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

DefineInstanceGetter(UMovableUnitSpawner)
{
	GetInstance_Ret(APlayerControllerBase, PlayerController, WorldContextObject, nullptr);
	return PlayerController->MovableUnitController;
}
// Called when the game starts
void UMovableUnitSpawner::BeginPlay()
{
	Super::BeginPlay();
}

AMovableUnit* UMovableUnitSpawner::Spawn(const FVector& SpawnLocation, const FName& UnitDataKey,
	const FOwnerPlayerData& InOwnerPlayerData, const int TroopCount, const FSpawnedUnitBuildingSlotProxy& SpawnedUnitBuildingSlotProxy)
{
	AMovableUnit* MovableUnit = Cast<AMovableUnit>(GetWorld()->SpawnActor<AMovableUnit>(
		MovableUnitClassRef,
		SpawnLocation,
		FRotator::ZeroRotator));
	
	if(MovableUnit)
	{
		//순서 유지 필수
		MovableUnit->SetOwnerPlayerData(InOwnerPlayerData);
		MovableUnit->SetMovableUnitDataKey(UnitDataKey);
		MovableUnit->SetTroopCount(TroopCount);
		MovableUnit->StartBT();
		MovableUnit->SetParentBuildingSlotProxy(SpawnedUnitBuildingSlotProxy);
		MovableUnit->SetEnable(true);
	}

	return MovableUnit;
}
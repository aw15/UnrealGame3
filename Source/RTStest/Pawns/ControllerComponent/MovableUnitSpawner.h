// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTStest/Common/PredefineMacro.h"
#include "RTStest/Pawns/Interface/UnitControllerInterface.h"
#include "MovableUnitSpawner.generated.h"


enum class EEngineEvent;
struct FOwnerPlayerData;
class FSpawnedUnitBuildingSlotProxy;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RTSTEST_API UMovableUnitSpawner :  public UActorComponent
{
	GENERATED_BODY()

public:
	DeclareInstanceGetter(UMovableUnitSpawner);
	// Sets default values for this component's properties
	UMovableUnitSpawner();

	
	
	//virtual void ProcessEngineEvent(const EEngineEvent& EngineEvent, APlayerControllerBase* const OwnerController, ASelectableUnit* Unit) override;
	virtual class AMovableUnit* Spawn(const FVector& SpawnLocation, const FName& UnitDataKey, const FOwnerPlayerData& InOwnerPlayerData, const int TroopCount,const FSpawnedUnitBuildingSlotProxy& SpawnedUnitBuildingSlotProxy);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<class AMovableUnit> MovableUnitClassRef;
};


class FBuildingLevelBuilder
{
public:
	FBuildingLevelBuilder(AMovableUnit* InMovableUnitRef) { MovableUnitRef = InMovableUnitRef; }
	void SetBuildingLevel(const int BuildingLevel);
	void SetFormationIndex(const int FormationIndex);
	AMovableUnit* Build();

private:
	AMovableUnit* MovableUnitRef = nullptr;
};
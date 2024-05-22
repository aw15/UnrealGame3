// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTStest/Data/PlayerData.h"
#include "UnitControllerInterface.generated.h"


UCLASS()
class RTSTEST_API UUnitControllerInterface : public UActorComponent
{
	GENERATED_BODY()
public:
	virtual void ProcessEngineEvent(const enum class EEngineEvent& EngineEvent, class APlayerControllerBase* const OwnerController, class ASelectableUnit* Unit)  {}
	virtual class AMovableUnit* Spawn(const FVector& SpawnLocation, const FName& UnitDataKey, const FOwnerPlayerData& OwnerPlayerId) { return nullptr; }
};

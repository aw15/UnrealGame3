// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingUnitController.h"

#include "Kismet/GameplayStatics.h"
#include "RTStest/Enviroment/BuildingSlot.h"
#include "RTStest/Pawns/PlayerControllerBase.h"
#include "RTStest/SaveSystem/BuildingSaveObject.h"

DefineInstanceGetter(UBuildingUnitController)
{
	GetInstance_Ret(APlayerControllerBase, PlayerControllerRef, WorldContextObject, nullptr);
	return PlayerControllerRef->BuildingController;
}
// Sets default values for this component's properties
UBuildingUnitController::UBuildingUnitController()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UBuildingUnitController::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> RetActors;

	RetActors.Reset();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABuildingSlot::StaticClass(),RetActors);
	for(AActor* Actor : RetActors)
	{
		ABuildingSlot* BuildingSlot = Cast<ABuildingSlot>(Actor);
		if(BuildingSlot)
		{
			BuildingSlots.Add(BuildingSlot);
			BuildingSlotsByName.FindOrAdd(Actor->GetName(),BuildingSlot);
		}
	}
}

ABuildingSlot* FBuildingSlotBuilder::Build(const FBuildingSaveData& BuildingSaveData) const
{
	if(!BuildingSlotRef)
		return nullptr;
	//순서 유지 필수
	BuildingSlotRef->SetEnable(BuildingSaveData.bEnable);
	BuildingSlotRef->SetOwnerPlayerIndex(BuildingSaveData.OwnerPlayerIndex);
	BuildingSlotRef->SetOwnerPlayerColorDataKey(BuildingSaveData.OwnerPlayerColorDataKey);
	BuildingSlotRef->UpdateByDataAndManagedList();

	return BuildingSlotRef;
}

void UBuildingUnitController::AddActiveBuilding(ABuildingSlot* BuildingSlot)
{
	ActiveBuildingSlots.Add(BuildingSlot);
}

void UBuildingUnitController::RemoveActiveBuilding(const ABuildingSlot* BuildingSlot)
{
	ActiveBuildingSlots.Remove(BuildingSlot);
}

void UBuildingUnitController::AddEnemyBuilding(ABuildingSlot* BuildingSlot)
{
	EnemyBuildingSlots.Add(BuildingSlot);
}

void UBuildingUnitController::RemoveEnemyBuilding(const ABuildingSlot* BuildingSlot)
{
	EnemyBuildingSlots.Remove(BuildingSlot);
}

TUniquePtr<FBuildingSlotBuilder> UBuildingUnitController::FindBuildingSlotByName(const FString& Name)
{
	if(BuildingSlotsByName.Contains(Name))
	{
		return MakeUnique<FBuildingSlotBuilder>(*BuildingSlotsByName.Find(Name));
	}
	 return nullptr;
}

const TArray<ABuildingSlot*>& UBuildingUnitController::GetAllBuildingSlots() const
{
	return BuildingSlots;
}

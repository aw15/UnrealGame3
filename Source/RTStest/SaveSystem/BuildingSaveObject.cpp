// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSaveObject.h"

#include "Kismet/GameplayStatics.h"
#include "RTStest/Enviroment/BuildingSlot.h"
#include "RTStest/Pawns/ControllerComponent/BuildingUnitController.h"

FString FBuildingSaveData::ToString() const
{
	return FString::Printf(TEXT("%s enable : %d Index : %d %s %s"), *BuildingName, bEnable, OwnerPlayerIndex, *OwnerPlayerColorDataKey.ToString(), *UnitSlotName);
}

UBuildingSaveSystem::UBuildingSaveSystem()
{
	SaveInstance = Cast<UBuildingSaveObject>(UGameplayStatics::CreateSaveGameObject(UBuildingSaveObject::StaticClass()));
}

void UBuildingSaveSystem::SaveData() const
{
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABuildingSlot::StaticClass(), OutActors);
	SaveInstance->BuildingDataList.Reset(OutActors.Num());
	
	for(AActor* ActorRef : OutActors)
	{
		const ABuildingSlot* BuildingSlot = Cast<ABuildingSlot>(ActorRef);
		if(BuildingSlot)
		{
			SaveInstance->BuildingDataList.Emplace(FBuildingSaveData{});
			SaveInstance->BuildingDataList.Last().BuildingName = BuildingSlot->GetName();
			SaveInstance->BuildingDataList.Last().bEnable = BuildingSlot->IsEnable();
			SaveInstance->BuildingDataList.Last().OwnerPlayerIndex = BuildingSlot->GetOwnerPlayerIndex();
			SaveInstance->BuildingDataList.Last().OwnerPlayerColorDataKey = BuildingSlot->GetOwnerPlayerColorDataKey();
		}
	}
	
	UGameplayStatics::SaveGameToSlot(SaveInstance, GetName(), 1);
}

void UBuildingSaveSystem::LoadData()
{
	SaveInstance = Cast<UBuildingSaveObject>(UGameplayStatics::CreateSaveGameObject(UBuildingSaveObject::StaticClass()));
	SaveInstance = Cast<UBuildingSaveObject>(UGameplayStatics::LoadGameFromSlot(GetName(), 1));

	GetInstance_Void(UBuildingUnitController, BuildingUnitController, GetWorld());
	
	for(const FBuildingSaveData& BuildingSaveData : SaveInstance->BuildingDataList)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, BuildingSaveData.ToString());
		}

		TUniquePtr<FBuildingSlotBuilder> BuildingSlotBuilder = BuildingUnitController->FindBuildingSlotByName(BuildingSaveData.BuildingName);
		if(!BuildingSlotBuilder.IsValid())
			continue;
		//순서대로 Set해야 하는 부분을 Builder패턴을 사용해서 감춤.
		ABuildingSlot* RetBuildingSlot = BuildingSlotBuilder->Build(BuildingSaveData);
		if(!RetBuildingSlot)
			continue;

		BuildingSlotBuilder.Reset();
	}
	
}



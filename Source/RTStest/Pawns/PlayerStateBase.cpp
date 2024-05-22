// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateBase.h"

#include "PlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "RTStest/Common/PredefineFunctions.h"
#include "RTStest/Enviroment/BuildingSlot.h"
#include "RTStest/AutoControlledPlayer/AutoControlledPlayer.h"

DefineInstanceGetter(APlayerStateBase)
{
	GetInstance_Ret(APlayerControllerBase, Controller, WorldContextObject, nullptr);
	return Cast<APlayerStateBase>(Controller->PlayerState);
}

void APlayerStateBase::BeginPlay()
{
	Super::BeginPlay();
	//BuildingSlot들에서 발생하는 updateResource 처리를 위해 델리게이트 바인딩.
	TArray<AActor*> RetActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABuildingSlot::StaticClass(),RetActors);
	for(AActor* Actor : RetActors)
	{
		const auto BuildingSlot = Cast<ABuildingSlot>(Actor);
		if(BuildingSlot)
		{
			BuildingSlot->OnUpdateResources.AddUObject(this, &APlayerStateBase::OnUpdateResource);
		}
	}
}

void APlayerStateBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
void APlayerStateBase::OnUpdateResource(const int OwnerPlayerIndex, const FResourceUnitData& ResourceUnitData)
{
	if(CheckFunc::IsPlayerIndex(OwnerPlayerIndex))
	{
		AddResource(EResourceType::Gold, ResourceUnitData.Gold);
		AddResource(EResourceType::Stone, ResourceUnitData.Stone);
		AddResource(EResourceType::Wood, ResourceUnitData.Wood);
	}
	else
	{
		GetInstance_Void(UAutoControlledPlayer, AutoPlayerRef, GetWorld())
		AutoPlayerRef->SetResource(EResourceType::Gold, ResourceUnitData.Gold);
		AutoPlayerRef->SetResource(EResourceType::Wood, ResourceUnitData.Wood);
		AutoPlayerRef->SetResource(EResourceType::Stone, ResourceUnitData.Stone);
		//PrintScreenMacro(*FString::Printf(TEXT("CPU%d : %d %d %d"),OwnerPlayerIndex, CpuGoldCount[OwnerPlayerIndex], CpuWoodCount[OwnerPlayerIndex], CpuStoneCount[OwnerPlayerIndex]))
	}
}

void APlayerStateBase::AddResource(const EResourceType& ResourceType, const int Amount)
{
	switch (ResourceType)
	{
	case EResourceType::Gold:
		{
			GoldCount += Amount;
		}
		break;
	case EResourceType::Stone:
		{
			StoneCount += Amount;
		}
		break;
	case EResourceType::Wood:
		{
			WoodCount += Amount;
		}
		break;
	default: ;
	}
}

bool APlayerStateBase::ConsumeResource(const EResourceType ResourceType, const int Amount)
{
	switch (ResourceType)
	{
	case EResourceType::Gold:
		{
			if(GoldCount < Amount)
			{
				PrintScreenMacro(TEXT("골드 부족!"))
				return false;
			}
			GoldCount -= Amount;
		}
		break;
	case EResourceType::Stone:
		{
			if(StoneCount < Amount)
			{
				PrintScreenMacro(TEXT("돌 부족!"))
				return false;
			}
			StoneCount -= Amount;
		}
		break;
	case EResourceType::Wood:
		{
			if(WoodCount < Amount)
			{
				PrintScreenMacro(TEXT("나무 부족!"))
				return false;
			}
			WoodCount -= Amount;
		}
		break;
	default: ;
	}
	return true;
}
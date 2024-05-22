// Fill out your copyright notice in the Description page of Project Settings.


#include "AutoControlledPlayer.h"

#include "Kismet/GameplayStatics.h"
#include "RTStest/Common/PredefineFunctions.h"
#include "RTStest/Enviroment/BuildingSlot.h"
#include "RTStest/GameDefault/RtsGameStateBase.h"


DefineInstanceGetter(UAutoControlledPlayer)
{
	GetInstance_Ret(ARtsGameStateBase, GameStateRef, WorldContextObject, nullptr);
	return GameStateRef->GetAutoControlledPlayer();
}

UAutoControlledPlayer::UAutoControlledPlayer()
{
}

void UAutoControlledPlayer::BeginPlay()
{
	//TODO 테스트 코드 실제 게임 로드가 된 이후에 호출할 것.
	OnBuildingLoadCompleted();
	GetWorld()->GetTimerManager().SetTimer(StrategyRefreshTimerHandle, this, &UAutoControlledPlayer::RefreshStrategy, 5, true );
}

void UAutoControlledPlayer::SetResource(const EResourceType ResourceType, const int Amount)
{
	switch (ResourceType)
	{
	case EResourceType::Stone:
		{
			StoneCount += Amount;
		}
		break;
	case EResourceType::Gold:
		{
			GoldCount += Amount;
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

void UAutoControlledPlayer::OnBuildingLoadCompleted()
{
	TArray<AActor*> AllBuildings;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABuildingSlot::StaticClass(), AllBuildings);
	for (const auto ActorRef : AllBuildings)
	{
		const auto BuildingRef = Cast<ABuildingSlot>(ActorRef);
		if(BuildingRef)
		{
			if(CheckFunc::IsPlayerIndex(BuildingRef->GetOwnerPlayerIndex()))
			{
				AllPlayerBuildingSlots.Add(BuildingRef);	
			}
			else
			{
				AllEnemyBuildingSlots.Add(BuildingRef);
			}
		}
	}
}

void UAutoControlledPlayer::RefreshStrategy()
{
	PrintScreenMacro("RefreshStrategy")
}

void UAutoControlledPlayer::OnBuildingDestroyed(const TScriptInterface<IHasOwnerActor>& DestroyedBuilding)
{
	const auto BuildingRef = Cast<ABuildingSlot>(DestroyedBuilding.GetObject());
	if(AllEnemyBuildingSlots.Contains(BuildingRef))
	{
		AllEnemyBuildingSlots.Remove(BuildingRef);
		return;
	}

	if(AllPlayerBuildingSlots.Contains(BuildingRef))
	{
		AllPlayerBuildingSlots.Remove(BuildingRef);
		return;
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "RtsGameStateBase.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "RTStest/AutoControlledPlayer/AutoControlledPlayer.h"
#include "RTStest/Pawns/PlayerStateBase.h"

DefineInstanceGetter(ARtsGameStateBase)
{
	const AGameModeBase* const GameMode = UGameplayStatics::GetGameMode(WorldContextObject);
	if(GameMode)
	{
		return Cast<ARtsGameStateBase>(GameMode->GameState);
	}

	return nullptr;
}

ARtsGameStateBase::ARtsGameStateBase()
{
	AutoControlledPlayer = CreateDefaultSubobject<UAutoControlledPlayer>(TEXT("AutoControlledPlayer'"));
}

void ARtsGameStateBase::TestColorAdded()
{
	//TODO 테스트 코드
	OwnerPlayerDataList.Add({1, "red"});
	OwnerPlayerDataList.Add({2, "blue"});
	OwnerPlayerDataList.Add({3, "black"});
}

void ARtsGameStateBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARtsGameStateBase::OnPostLogin(const APlayerController* NewPlayer)
{
	const auto PlayerStateBaseRef = Cast<APlayerStateBase>(NewPlayer->PlayerState);
	if(PlayerStateBaseRef)
	{
		OwnerPlayerDataList.Add({PlayerStateBaseRef->PlayerIndex, PlayerStateBaseRef->PlayerColorDataKey});
	}

	//TODO 테스트 코드
	TestColorAdded();
}

FName ARtsGameStateBase::FindPlayerColor(const int PlayerIndex)
{
	auto const FindElement = OwnerPlayerDataList.FindByPredicate([&PlayerIndex](const FOwnerPlayerData& Data)
	{
		if(Data.PlayerIndex == PlayerIndex)
			return true;

		return false;
	});
	if(FindElement)
	{
		return FindElement->PlayerColorDataKey;
	}

	return FName{};
}

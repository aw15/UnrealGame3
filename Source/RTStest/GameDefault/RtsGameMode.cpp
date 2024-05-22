// Fill out your copyright notice in the Description page of Project Settings.


#include "RtsGameMode.h"

#include "RtsGameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "RTStest/Pawns/PlayerStateBase.h"

void ARtsGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	//상위단계에서 컨트롤러는 널 체크 중.
	APlayerStateBase* PlayerStateBase = Cast<APlayerStateBase>(NewPlayer->PlayerState);
	if(!PlayerStateBase)
	{
		UE_LOG(LogNet, Fatal, TEXT("PlayerState Invalid"));
		return;
	}
	
	//TODO PlayerIndex는 Session 생성시 받는다고 가정.
	UserDataByPlayerId.Add(PlayerStateBase->GetPlayerId(), PlayerLoginCount);
	PlayerStateBase->PlayerIndex = PlayerLoginCount;
	//TODO 색지정 일단 하드코딩
	switch (PlayerLoginCount)
	{
	case 0:
		{
			PlayerStateBase->PlayerColorDataKey = "green";
		}
		break;
	case 1:
		{
			PlayerStateBase->PlayerColorDataKey = "yellow";
		}
		break;
	case 2:
		{
			PlayerStateBase->PlayerColorDataKey = "blue";
		}
		break;
	case 3:
		{
			PlayerStateBase->PlayerColorDataKey = "purple";
		}
		break;
	default: ;
	}
	PlayerLoginCount += 1;

	const auto GameStateBase =GetGameState<ARtsGameStateBase>();
	GameStateBase->OnPostLogin(NewPlayer);
	//TODO Test를 위한 하드코딩
}

void ARtsGameMode::BeginPlay()
{
	Super::BeginPlay();
}

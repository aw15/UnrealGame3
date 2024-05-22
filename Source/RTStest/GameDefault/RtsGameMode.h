// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "RtsGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RTSTEST_API ARtsGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void BeginPlay() override;
public:
	UPROPERTY(BlueprintReadOnly)
	TMap<int, int> UserDataByPlayerId;
private:
	int PlayerLoginCount = 0;
};

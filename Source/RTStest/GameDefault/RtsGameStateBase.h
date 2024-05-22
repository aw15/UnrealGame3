// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "RTStest/Data/PlayerData.h"
#include "RtsGameStateBase.generated.h"

class UAutoControlledPlayer;
/**
 * 
 */
UCLASS()
class RTSTEST_API ARtsGameStateBase : public AGameState
{
	GENERATED_BODY()
public:
	ARtsGameStateBase();
	void TestColorAdded();
protected:
	virtual void BeginPlay() override;

public:
	DeclareInstanceGetter(ARtsGameStateBase)
	void OnPostLogin(const APlayerController* NewPlayer);
	FName FindPlayerColor(const int PlayerIndex);
	UAutoControlledPlayer* GetAutoControlledPlayer() const  {return AutoControlledPlayer;}

protected:
	UPROPERTY(BlueprintReadWrite)
	UAutoControlledPlayer* AutoControlledPlayer;

private:
	UPROPERTY()
	TArray<FOwnerPlayerData> OwnerPlayerDataList;
};

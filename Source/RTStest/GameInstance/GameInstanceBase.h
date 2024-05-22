// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RTStest/Common/PredefineMacro.h"
#include "GameInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class RTSTEST_API UGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()
	virtual void Init() override;
public:
	DeclareInstanceGetter(UGameInstanceBase)
	UPROPERTY(BlueprintReadOnly)
	class UDataManager* DataManager;
	UPROPERTY(BlueprintReadOnly)
	class UBuildingSaveSystem* BuildingSaveSystem;
};

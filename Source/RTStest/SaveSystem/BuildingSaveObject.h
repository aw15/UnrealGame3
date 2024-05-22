// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "BuildingSaveObject.generated.h"

/**
 * 
 */

USTRUCT(Blueprintable)
struct FBuildingSaveData
{
	GENERATED_BODY()

public:
	FString ToString() const;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString BuildingName;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString UnitSlotName;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FName OwnerPlayerColorDataKey;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	int OwnerPlayerIndex;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	int BuildingLevel;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	bool bEnable;
};

UCLASS()
class RTSTEST_API UBuildingSaveObject : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FBuildingSaveData> BuildingDataList;
};

UCLASS()
 class UBuildingSaveSystem : public UObject
{
	GENERATED_BODY()

public:
	UBuildingSaveSystem();
	UFUNCTION(BlueprintCallable)
	void SaveData() const;
	UFUNCTION(BlueprintCallable)
	void LoadData();


public:
	UPROPERTY()
	UBuildingSaveObject* SaveInstance;
};
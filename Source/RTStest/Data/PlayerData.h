// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RTStest/Common/PredefineMacro.h"
#include "UObject/Object.h"
#include "PlayerData.generated.h"


USTRUCT(BlueprintType)
struct FPlayerColorData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* MovableUnitColorMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* BuildingUnitColorMaterial = nullptr;
};

USTRUCT(BlueprintType)
struct FOwnerPlayerData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int PlayerIndex = INVALID_INDEX;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName PlayerColorDataKey;
	bool IsValid() const
	{
		return PlayerIndex!= INVALID_INDEX;
	}
};



UCLASS()
class RTSTEST_API UPlayerData : public UObject
{
	GENERATED_BODY()
};

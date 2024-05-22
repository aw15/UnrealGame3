// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Math/UnitConversion.h"
#include "CommonUnitMenuBase.generated.h"

/**
 * 
 */
UCLASS()
class RTSTEST_API UCommonUnitMenuBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent)
	void Show(const bool bShow);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FloatingProgressbarBase.generated.h"

/**
 * 
 */
UCLASS()
class RTSTEST_API UFloatingProgressbarBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetPercent(const float InPercent, const float InMax);
	UFUNCTION(BlueprintImplementableEvent)
	void SetFillColor(UPARAM(ref) const FLinearColor& Color);
	UFUNCTION(BlueprintImplementableEvent)
	void SetText(UPARAM(ref)const FText& Text);
};

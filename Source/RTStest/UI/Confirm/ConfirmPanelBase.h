// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ConfirmOwner.h"
#include "Blueprint/UserWidget.h"
#include "ConfirmPanelBase.generated.h"

/**
 * 
 */
UCLASS()
class RTSTEST_API UConfirmPanelBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OpenConfirmPanel(const TScriptInterface<IConfirmOwner>& InConfirmOwner);

	UPROPERTY(BlueprintReadOnly)
	TScriptInterface<IConfirmOwner> ConfirmOwner = nullptr;
};

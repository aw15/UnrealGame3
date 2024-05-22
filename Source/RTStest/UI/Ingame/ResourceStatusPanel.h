// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResourceStatusPanel.generated.h"

/**
 * 
 */
UCLASS()
class RTSTEST_API UResourceStatusPanel : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	

protected:
	void UpdateResource();
	UPROPERTY(EditDefaultsOnly)
	class UTextBlock* StoneText;
	UPROPERTY(EditDefaultsOnly)
	UTextBlock* GoldText;
	UPROPERTY(EditDefaultsOnly)
	UTextBlock* WoodText;
private:
	FTimerHandle UpdateHandle;
	const int UPDATE_TIME = 1;
	bool bUiInitialized = false;
};

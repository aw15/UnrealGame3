// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FloatingButtonBase.generated.h"

/**
 * 
 */
UCLASS()
class RTSTEST_API UFloatingButtonBase : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

public:
	DECLARE_DELEGATE(FOnClickBtn)
	FOnClickBtn OnClickBtn;
protected:
	UFUNCTION()
	void OnClicked();
	UPROPERTY(EditDefaultsOnly,Transient)
	class UButton* ButtonRef;
};

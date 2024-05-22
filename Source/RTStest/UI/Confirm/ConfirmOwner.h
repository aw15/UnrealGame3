// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "ConfirmOwner.generated.h"

UENUM(BlueprintType)
enum class EConfirmPanelType : uint8
{
	ConstructDecision,
	BuildingUpgradeDecision,
	SetQuestTargetDecision,
	SetUnitSlotDecision
};
// This class does not need to be modified.
UINTERFACE(NotBlueprintable, MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UConfirmOwner : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RTSTEST_API IConfirmOwner
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable)
	virtual void ConfirmComplete(const EConfirmPanelType ConfirmPanelType, const bool bResult) = 0;
};

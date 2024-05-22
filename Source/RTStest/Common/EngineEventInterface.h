// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EngineEventInterface.generated.h"


UENUM()
enum class EEngineEvent
{
	LeftMousePressed,
	LeftMouseReleased,
	RightMousePressed,
	RightMousePressedOnUnit,
};

// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UEngineEventListener : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RTSTEST_API IEngineEventListener
{
	GENERATED_BODY()
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ProcessEngineEvent(const enum class EEngineEvent& EngineEvent) {}
};

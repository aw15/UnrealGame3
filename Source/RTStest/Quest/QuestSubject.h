// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuestSubject.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUpdateQuest, const TScriptInterface<class IQuestSubject>&);

UENUM()
enum class EQuestType
{
	None,
	DestroyBuilding
};
// This class does not need to be modified.
UINTERFACE()
class UQuestSubject : public UInterface
{
	GENERATED_BODY()
};
/**
 * 
 */
class RTSTEST_API IQuestSubject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FVector GetSubjectLocation() const { return FVector::ZeroVector; }
	DECLARE_MULTICAST_DELEGATE(FOnQuestComplete);
	FOnQuestComplete OnQuestComplete;
};

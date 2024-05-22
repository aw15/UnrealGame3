// // Fill out your copyright notice in the Description page of Project Settings.
//
#pragma once

#include "CoreMinimal.h"
#include "UnitInterface.generated.h"

UENUM()
enum EBaseUnitType
{
	None,
	Movable,
	Building
};

// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UHasOwnerActor : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RTSTEST_API IHasOwnerActor
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	EBaseUnitType UnitType = EBaseUnitType::None;
	virtual int GetOwnerPlayerIndex() const = 0;

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnUnitDead, const TScriptInterface<IHasOwnerActor>&); //포인터로 넘겨주는 값은 객체가 Destroy중이므로 안전하지 않을 수 있음. 단순 비교용으로 사용할 것.
	FOnUnitDead OnUnitDead;
};



// This class does not need to be modified.
UINTERFACE(Blueprintable, MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class USetQuestTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RTSTEST_API ISetQuestTarget
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	public:
	virtual void SetQuestTarget(AActor* ActorRef) = 0;
};


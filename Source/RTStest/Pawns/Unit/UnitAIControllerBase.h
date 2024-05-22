// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UnitAIControllerBase.generated.h"

enum class EUnitState : uint8;
class UBehaviorTreeComponent;
class UBlackboardComponent;
/**
 * 
 */
UCLASS()
class RTSTEST_API AUnitAIControllerBase : public AAIController
{
	GENERATED_BODY()
public:
	AUnitAIControllerBase(FObjectInitializer const& ObjInitializer);
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	UBlackboardComponent* GetBlackboard() const;
protected:
	UPROPERTY(BlueprintReadWrite)
	class AMovableUnit* ControlledUnit;
	//BT
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI")
	class UBehaviorTree* DefaultBTRef;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI")
	UBehaviorTreeComponent* BehaviorTreeComponent;
	//Blackboard
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="AI")
	UBlackboardComponent* UnitBlackboardRef;
};

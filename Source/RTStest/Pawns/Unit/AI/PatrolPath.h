// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SmartObject.h"
#include "Components/SplineComponent.h"
#include "PatrolPath.generated.h"

UCLASS()
class RTSTEST_API APatrolPath : public ASmartObject
{
	GENERATED_BODY()

public:
	APatrolPath();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="SplinePath")
	class USplineComponent* Path;
	void GetSplinePoints();
	TArray<FVector> Locations;

protected:
	virtual void BeginPlay() override;
};

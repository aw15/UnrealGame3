#pragma once
#include "CoreMinimal.h"
#include "RTStest/Pawns/SpecificUnit/UnitClassComponent.h"
#include "CavalryClassComponent.generated.h"


class UStaticMeshComponent;
class USkeletalMeshComponent;

UCLASS()
class RTSTEST_API UCavalryClassComponent : public UUnitClassComponent
{
	GENERATED_BODY()
public:
	UCavalryClassComponent();
	virtual void BeginPlay() override;
};


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTStest/Pawns/Interface/UnitControllerInterface.h"
#include "BuildingUnitController.generated.h"

class APlayerControllerBase;
class AUnitSlot;
class FBuildingSlotBuilder;
struct FBuildingSaveData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class RTSTEST_API UBuildingUnitController : public UUnitControllerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UBuildingUnitController();
	DeclareInstanceGetter(UBuildingUnitController)
	void AddActiveBuilding(class ABuildingSlot* BuildingSlot);
	void RemoveActiveBuilding(const ABuildingSlot* BuildingSlot);
	void AddEnemyBuilding(class ABuildingSlot* BuildingSlot);
	void RemoveEnemyBuilding(const ABuildingSlot* BuildingSlot);
	TUniquePtr<FBuildingSlotBuilder> FindBuildingSlotByName(const FString& Name);
	UFUNCTION(BlueprintCallable)
	const TSet<ABuildingSlot*>& GetActiveBuildingSlots() const { return ActiveBuildingSlots; }
	UFUNCTION(BlueprintCallable)
	const TSet<ABuildingSlot*>& GetEnemyBuildingSlots() const { return EnemyBuildingSlots; }
	UFUNCTION(BlueprintCallable)
	const TArray<ABuildingSlot*>& GetAllBuildingSlots() const;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	//유닛 슬롯에서 빌딩슬롯 연결할 목록 정할 때 사용됨.
	UPROPERTY()
	TSet<ABuildingSlot*> ActiveBuildingSlots;
	UPROPERTY()
	TArray<ABuildingSlot*> BuildingSlots;
	UPROPERTY()
	TSet<ABuildingSlot*> EnemyBuildingSlots;
	TMap<FString, ABuildingSlot*> BuildingSlotsByName;
};


class FBuildingSlotBuilder
{
public:
	explicit FBuildingSlotBuilder(ABuildingSlot* InBuildingSlot) : BuildingSlotRef(InBuildingSlot){}
	ABuildingSlot* Build(const FBuildingSaveData& BuildingSaveData) const;
	
private:
	ABuildingSlot* BuildingSlotRef = nullptr;
};
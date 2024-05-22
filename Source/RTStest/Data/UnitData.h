// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "UnitData.generated.h"


UENUM(BlueprintType)
enum class ECostType : uint8
{
	None,
	Wood,
	Gold,
	Stone
};

UENUM(BlueprintType)
enum class  EBuildingType : uint8
{
	None,
	Basic,
	Advanced
};

USTRUCT(BlueprintType)
struct FCostData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECostType CostType = ECostType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Amount = 0;
};

USTRUCT(BlueprintType)
struct FBuildingData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBuildingType UnitType = EBuildingType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GoldCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int WoodCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int StoneCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GoldCostMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WoodCostMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StoneCostMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* PreBuildingMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* BuildingMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ResourceUnitDataKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* UnitSlotIconImage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Hp = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ConstructionSecond = 1;
};

UENUM(BlueprintType)
enum class  EMovableUnitType : uint8
{
	None,
	Worker,
	Infantry,
	Cavalry
};

UENUM(BlueprintType)
enum class  EFormationType : uint8
{
	None,
	ThreeByTwo
};
UENUM(BlueprintType)
enum class EDamageType : uint8
{
	None,
	MeleeOneTarget
};


USTRUCT(BlueprintType)
struct FMovableUnitData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMovableUnitType UnitType = EMovableUnitType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* MainMesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector AttackRangeBoxExtent{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector AttackRangeBoxPosition{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DetectBoxRadius{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DetectBoxPosition{};
	//Formation 형태, 아직 사용하지 않음.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EFormationType FormationType = EFormationType::None;
	//공격 방식
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageType DamageType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AttackCoolTime = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalDamage = 1;
	//근처 적을 찾는 시간 간격
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SearchEnemyRate = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Hp = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HpUpgradeAmount = 1; //체력 = HP + HpUpgradeAmount * BuildingLevel
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NormalDamageUpgradeAmount = 1;//공격력 = NormalDamage + AttackUpgradeAmount * BuildingLevel
};
//자원 유닛.
UENUM(BlueprintType)
enum class  EResourceType : uint8
{
	None,
	Soldier,
	Stone,
	Gold,
	Wood
};

USTRUCT(BlueprintType)
struct FSpawnSoldierData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EResourceType ResourceType = EResourceType::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MovableUnitDataKey; //Type이 유닛일 때만 생성.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Amount = 0;
};

USTRUCT(BlueprintType)
struct FResourceUnitData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Gold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Stone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Wood;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> SpawnSoldierList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SpawnCoolTime = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ResourceCoolTime = 1;
};


UCLASS()
class RTSTEST_API AUnitData : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUnitData();
};

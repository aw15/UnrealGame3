// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RTStest/Common/PredefineEnum.h"
#include "RTStest/Common/PredefineMacro.h"
#include "RTStest/Data/UnitData.h"
#include "PlayerStateBase.generated.h"

/**
 * 
 */
UCLASS()
class RTSTEST_API APlayerStateBase : public APlayerState
{
	GENERATED_BODY()
public:
	DeclareInstanceGetter(APlayerStateBase)
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	//리소스 관련
	//각 리소스 유닛으로부터 오는 리소스 업데이트 요청.
	UFUNCTION()
	void OnUpdateResource(const int OwnerPlayerIndex , const FResourceUnitData& ResourceUnitData);
	void AddResource(const EResourceType& ResourceType, const int Amount);
	bool ConsumeResource(const EResourceType ResourceType, const int Amount);
public:
	UPROPERTY(BlueprintReadOnly)
	int StoneCount = 500; // TODO 자원 초기값 수정 필요
	UPROPERTY(BlueprintReadOnly)
	int WoodCount = 500;
	UPROPERTY(BlueprintReadOnly)
	int GoldCount = 500;
	UPROPERTY(BlueprintReadOnly)
	int PlayerIndex = INVALID_INDEX;
	UPROPERTY(BlueprintReadOnly)
	FName PlayerColorDataKey;
	//플레이어가 아닌 CPU들의 자원.
	TMap<int, int> CpuStoneCount;
	TMap<int, int> CpuWoodCount;
	TMap<int, int> CpuGoldCount;
};

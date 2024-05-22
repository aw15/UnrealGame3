// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MovableUnitState.h"
#include "GameFramework/Character.h"
#include "RTStest/Common/EngineEventInterface.h"
#include "RTStest/Common/PredefineEnum.h"
#include "RTStest/Data/PlayerData.h"
#include "RTStest/Data/UnitData.h"
#include "RTStest/Enviroment/Pattern/FSpawnedUnitBuildingSlotProxy.h"
#include "RTStest/Pawns/Interface/UnitInterface.h"
#include "RTStest/Quest/QuestManager.h"
#include "RTStest/UI/Ingame/CommonUnitMenuBase.h"
#include "UObject/WeakInterfacePtr.h"
#include "MovableUnit.generated.h"

class FMovableUnitState;
class AMovableUnit;
class ABuildingSlot;
class FSpawnedUnitBuildingSlotProxy;

DECLARE_MULTICAST_DELEGATE(FOnSpawnComplete);
UCLASS()
class RTSTEST_API AMovableUnit : public ACharacter, public IHasOwnerActor, public IEngineEventListener, public ISetQuestTarget
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	AMovableUnit();

	//AI//////////////////////////////////////////////
	void StartBT();
	virtual void PossessedBy(AController* NewController) override;
	//Unit Control
	void RotateWhenMovingComplete(const float InYaw) const;
	void SetParentBuildingSlotProxy(const FSpawnedUnitBuildingSlotProxy& InBuildingSlot);
	void SearchEnemyTimerEvent();
	void CompleteAttackBuildingQuest();
	//IEngineEventListener
	virtual void ProcessEngineEvent(const EEngineEvent& EngineEvent) override;
	//ISetQuestTarget
	virtual void SetQuestTarget(AActor* ActorRef) override;
	//Decal Control
	void OnSelectionCancel() const;
	//Set
	void SetMeshByData() const;
	void SetRangeBoxByData() const;
	void SetMovableUnitDataKey(const FName& InMovableUnitDataKey);
	void SetUnitLevelAndCalculateUnitStat(const int Level);//BuildingData 세팅한 이후에 호출할 것.
	void SetOwnerPlayerData(const FOwnerPlayerData& InOwnerPlayerData);
	void SetTroopCount(const int InTroopCount); //병력수를 설정.
	//Spawn시 모든 사전 초기화 함수들을 수행후 호출
	void SetEnable(const bool bInEnable) { bEnable = bInEnable; }
	void SetFormationIndex(const int InFormationIndex) { FormationIndex = InFormationIndex; }
	//Get
	const FName& GetMovableUnitDataKey() const { return MovableUnitDataKey; }
	const FMovableUnitData& GetMovableUnitData() const { return MovableUnitData; }
	virtual int GetOwnerPlayerIndex() const override { return OwnerPlayerData.PlayerIndex; }
	bool CanStartBT() const {return bCanStartBT;}
	bool IsEnable() const {return bEnable;}
	int GetFormationIndex() const { return FormationIndex; }
	UFUNCTION(BlueprintCallable)
	EUnitAnimState GetAnimState() const;
	UFUNCTION(BlueprintCallable)
	EUnitState GetUnitState() const;
	bool CanProcessQuest() const;
	class AUnitAIControllerBase* GetAiController() const { return AiControllerRef;};
	class APlayerControllerBase* GetPlayerController() const;
	IHasOwnerActor* GetTargetEnemy() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(class TArray<FLifetimeProperty>&) const override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Dead();
private:
	UFUNCTION()
	void OnBeginAttackRangeBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndAttackRangeBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//Damage, Attack
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void OnResetAttackCoolTime();
	void SetTargetEnemy(IHasOwnerActor* UnitRef);
	void OnTargetEnemyDead(const TScriptInterface<IHasOwnerActor>& DeadUnitRef); //타겟 적이 죽은 경우 호출, OnUnitDead 델리게이트에 연결됨.
	UFUNCTION()
	void OnTargetBuildingDestroyed();
private:
	void HideAllSubMesh();
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Property")
	FName MovableUnitDataKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMovableUnitData MovableUnitData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FOwnerPlayerData OwnerPlayerData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UUnitClassComponent* UnitClassComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* AttackRangeBoxComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* ClickDetectionBoxComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent* DetectRangeBoxComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDecalComponent* SelectedDecal = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SubSkeletalMesh1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SubSkeletalMesh2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SubSkeletalMesh3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SubSkeletalMesh4;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* SubSkeletalMesh5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Need Save")
	int TroopCount = 0;

	//Data
	UPROPERTY()
	UDataTable* PlayerColorDataTable = nullptr;
	UPROPERTY()
	UDataTable* MovableUnitDataTable = nullptr;

	//AI
	UPROPERTY(BlueprintReadOnly)
	bool bCanStartBT =false;
	FTimerHandle NoiseEventTimerHandle;
	
	//Damage, Attack
	TWeakInterfacePtr<IHasOwnerActor> TargetEnemy;
	FTimerHandle AttackCoolTimeTimer;
	UPROPERTY(VisibleAnywhere, Category="Debug")
	int CurrentHp = 1;
	FDelegateHandle TargetEnemyDeadHandle;
	FDelegateHandle TargetBuildingDestroyedHandle;
	UPROPERTY(VisibleAnywhere, Category="Debug")
	int CurrentMaxHp = 1; //레벨 값을 더한 값. 계산시 이 값을 사용할것.
	UPROPERTY(VisibleAnywhere, Category="Debug")
	int CurrentNormalDamage;//레벨 값을 더한 값. 계산시 이 값을 사용할것.
	
	//Animation
	int OverlappingEnemyCount = 0;
	
	//Formation
	int FormationIndex = 0;
	
	//부모 BuildingSlot
	FSpawnedUnitBuildingSlotProxy ParentBuildingSlotProxy;
	
	//Spawn시 모든 사전 초기화 함수들을 수행 후 true로 바꿈 
	UPROPERTY(BlueprintReadOnly)
	bool bEnable = false;
	UPROPERTY()
	AUnitAIControllerBase* AiControllerRef = nullptr;
	UPROPERTY()
	class APlayerControllerBase* PlayerControllerRef;

	//BlackBoard키를 한번더 갱신안하면 제대로 안움직이는 버그가 있어 SpawnPoint를 보관하였다가 추후 다시 SpawnPoint로 갈 일이 있다면 사용. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Debug")
	FVector SpawnPoint;

	//State
	FStateManager StateManager;

	//Quest
	FQuestObject CurrentQuestObject;
	FTimerHandle CheckQuestSubjectValidHandle;
};
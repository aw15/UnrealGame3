#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RTStest/Common/EngineEventInterface.h"
#include "RTStest/Common/PredefineMacro.h"
#include "RTStest/Data/UnitData.h"
#include "RTStest/Pawns/Interface/UnitInterface.h"
#include "RTStest/Quest/QuestSubject.h"
#include "RTStest/UI/Confirm/ConfirmOwner.h"
#include "Pattern/FSpawnedUnitBuildingSlotProxy.h"
#include "BuildingSlot.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdateResources, int OwnerPlayerIndex, const FResourceUnitData&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBuildingSlotDestroyed, class ABuildingSlot*);

class UBuildingSaveObject;

UCLASS()
class RTSTEST_API ABuildingSlot : public AActor, public IConfirmOwner, public IEngineEventListener, public IQuestSubject, public IHasOwnerActor, public ISetQuestTarget
{
	GENERATED_BODY()
public:
	enum class EUnitSlotEvent
	{
		Enable,
		Disable,
		NeedUnitReset,
		SetQuestTarget
	};
public:
	ABuildingSlot();
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	void SetMesh() const;
	////////////////////////////////////////////////
	//ISetQuestTarget
	virtual void SetQuestTarget(AActor* ActorRef) override;
	//빌딩 메시 세팅 및 빌딩 컨트롤러에 건물추가.
	void UpdateByDataAndManagedList();
	//Confirm창 선택 후 빌딩이 지어짐.
	UFUNCTION(BlueprintCallable)
	virtual void ConfirmComplete(const EConfirmPanelType ConfirmPanelType, const bool bResult) override;
	//마우스로 플레이어의 다른 유닛 슬롯이나 빌딩을 선택했을때 호출
	UFUNCTION()
	void OnCancelSelection(AActor* ActorRef);
	virtual void ProcessEngineEvent(const enum class EEngineEvent& EngineEvent) override;
	//IQuestSubject Implement/////////////////////////
	virtual FVector GetSubjectLocation() const override { return GetActorLocation(); }
	//////////////////////////////////////////////////
	void DestroyBuilding();
	//Damage
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	//Set
	void SetEnable(const bool InEnable);
	void SetOwnerPlayerIndex(const int InOwnerPlayerIndex) { OwnerPlayerIndex = InOwnerPlayerIndex; }
	void SetOwnerPlayerColorDataKey(const FName& InOwnerPlayerColorDataKey) { OwnerPlayerColorDataKey = InOwnerPlayerColorDataKey; }
	
protected:
	//자원
	void UpdateCreateResourceTimer(const bool bCurrentEnableState);
	//유닛 생산
	UFUNCTION(BlueprintCallable)
	void MakeTroop();

	//Floating UI
private:
	void CheckConstructProcess(); 
	void ShowHpProgressbarWidget(const bool bShow) const;
	static void SetPercentOnProgressbarWidget(class UFloatingProgressbarBase* ProgressWidgetRef, const float Current, const float Max);
	static void SetTextOnProgressbarWidget(UFloatingProgressbarBase* ProgressWidgetRef, const FText& InText);
	static void SetColorOnProgressbarWidget(UFloatingProgressbarBase* ProgressWidgetRef, const FLinearColor& InColor);
	void ShowCommonProgressbarWidget(const bool bShow) const;

	//Upgrade
	static int CalculateRequiredResourceForUpgrade(const int InitialCost,const float CostMultiplier, const int BuildingLevel);

	//Quest
	void UpdateTargetEnemyBuilding(ABuildingSlot* TargetBuildingSlot);
public:
	//Get
	bool IsEnable() const {return  bEnable;}
	virtual int GetOwnerPlayerIndex() const override {return OwnerPlayerIndex;}
	const FBuildingData& GetBuildingData() const { return BuildingData; }
	bool GetSpawnPointYaw(float& OutYaw) const;
	FName GetOwnerPlayerColorDataKey() const { return OwnerPlayerColorDataKey; }
	
protected:
	void ShowReceiveResourceButton();//자원 받기 버튼을 띄워줍니다.
	void HideReceiveResourceButton();
	void CreateResource() const;//자원을 생성합니다.
	//Unit을 생성합니다.
	void SpawnSoldierType();
	void UpdateSpawnSoldierTimer(const bool bCurrentEnableState);
	//건물 업그레이드
	UFUNCTION(BlueprintCallable)
	void UpgradeBuilding();
public:
	FOnUpdateResources OnUpdateResources;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USceneComponent* SceneRoot = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UStaticMeshComponent* BuildingMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UDecalComponent* AreaDecal = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent* DetectBoxComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UArrowComponent* SpawnPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="IngameUI")
	class UTextRenderComponent* FloatingBuildingName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="IngameUI")
	class UWidgetComponent* CommonProgressbarWidgetComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="IngameUI")
	UWidgetComponent* HpProgressbarWidgetComponent = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="IngameUI")
	UWidgetComponent* ReceiveResourceWidgetComponent = nullptr;
	TWeakObjectPtr<class UFloatingButtonBase> ReceiveResourceButtonWidget;

	//Property
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Need Save")
	int OwnerPlayerIndex = INVALID_INDEX;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Need Save")
	FName OwnerPlayerColorDataKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Need Save")
	FName BuildingUnitKey;
	UPROPERTY(BlueprintReadOnly, Category="Need Save")
	FBuildingData BuildingData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Need Save")
	bool bEnable = false; //변수를 세팅 할때는 반드시 SetEnable를 사용할 것.
	UPROPERTY(BlueprintReadOnly)
	bool bConstructing = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Need Save")
	FText BuildingName; // UI에 표시될 지역명
	
	//Attack, Damage
	UPROPERTY(VisibleAnywhere)
	int CurrentHp = 1;
	//Widget
	TWeakObjectPtr<class UFloatingProgressbarBase> CommonProgressbar;
	TWeakObjectPtr<class UFloatingProgressbarBase> HpProgressbar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor HpProgressbarColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FLinearColor ConstructProgressbarColor;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ConstructProgressbarText;
private:
	//자원
	FResourceUnitData ResourceUnitData;
	FTimerHandle CreateResourceHandle;
	//유닛 관리
	FTimerHandle CreateSoldierHandle;
	int CurrentSpawnUnitIndex = 0;
	int DeadUnitCount = 0;
	int ArrivedUnitSlotCount = 0;
	//기본 오브젝트 Ref
	UPROPERTY()
	class ARtsGameStateBase* GameStateRef = nullptr;
	//데이터 테이블
	UPROPERTY()
	UDataTable* BuildingDataTable = nullptr;
	UPROPERTY()
	UDataTable* ResourceUnitDataTable = nullptr;
	UPROPERTY()
	UDataTable* PlayerColorDataTable = nullptr;
	//유닛 생성 타이머
	FTimerHandle CheckConstructProcessHandle;
	int SecondsDuringConstruction = 0;
	const int CheckConstructProcessInterval = 1;

	//MovableUnit과 커플링 방지를 위한 프록시 패턴
	FSpawnedUnitBuildingSlotProxy SpawnedUnitBuildingSlotProxy;
};
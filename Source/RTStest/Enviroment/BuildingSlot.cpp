#include "BuildingSlot.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RTStest/Common/PredefineFunctions.h"
#include "RTStest/Data/DataManager.h"
#include "RTStest/Data/UnitData.h"
#include "RTStest/GameDefault/RtsGameStateBase.h"
#include "RTStest/Hud/InGameHudBase.h"
#include "RTStest/Pawns/PlayerControllerBase.h"
#include "RTStest/Pawns/PlayerStateBase.h"
#include "RTStest/Pawns/ControllerComponent/BuildingUnitController.h"
#include "RTStest/Pawns/ControllerComponent/MovableUnitSpawner.h"
#include "RTStest/Pawns/Unit/MovableUnit.h"
#include "RTStest/UI/FloatingWidget/FloatingButtonBase.h"
#include "RTStest/UI/FloatingWidget/FloatingProgressbarBase.h"

ABuildingSlot::ABuildingSlot()
{
	UnitType = EBaseUnitType::Building;
	
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneRoot;
	
	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh"));
	BuildingMesh->SetupAttachment(RootComponent);
	AreaDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("AreaDecal"));
	AreaDecal->SetupAttachment(RootComponent);
	
	DetectBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectBoxComponent"));
	DetectBoxComponent->SetupAttachment(RootComponent);
	SpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnPoint"));
	SpawnPoint->SetupAttachment(RootComponent);

	FloatingBuildingName = CreateDefaultSubobject<UTextRenderComponent>(TEXT("BuildingName"));
	FloatingBuildingName->SetupAttachment(RootComponent);

	CommonProgressbarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Progressbar"));
	CommonProgressbarWidgetComponent->SetupAttachment(RootComponent);
	CommonProgressbarWidgetComponent->SetVisibility(false);
	CommonProgressbarWidgetComponent->SetCastShadow(false);
	
	HpProgressbarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpProgressbar"));
	HpProgressbarWidgetComponent->SetupAttachment(RootComponent);
	HpProgressbarWidgetComponent->SetCastShadow(false);

	ReceiveResourceWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	ReceiveResourceWidgetComponent->SetupAttachment(RootComponent);
	ReceiveResourceWidgetComponent->SetVisibility(false);
	ReceiveResourceWidgetComponent->SetCastShadow(false);
	
	BuildingDataTable = UDataManager::LoadDataTable(EDataType::BuildingUnit);
	ResourceUnitDataTable = UDataManager::LoadDataTable(EDataType::ResourceUnit);
	PlayerColorDataTable = UDataManager::LoadDataTable(EDataType::PlayerColor);
}

void ABuildingSlot::BeginPlay()
{
	Super::BeginPlay();

	SpawnedUnitBuildingSlotProxy.SetBuildingSlot(this);
	GameStateRef = Cast<ARtsGameStateBase>(UGameplayStatics::GetGameState(GetWorld()));
	
	//선택 데칼 Off
	AreaDecal->SetVisibility(false);
	
	//데이터 세팅
	if(BuildingDataTable)
	{
		const auto BuildingDataPtr = BuildingDataTable->FindRow<FBuildingData>(BuildingUnitKey, TEXT(""));
		if(BuildingDataPtr)
		{
			BuildingData = *BuildingDataPtr;
			CurrentHp = BuildingData.Hp;
			FloatingBuildingName->SetText(BuildingName);
		}
	}
	if(ResourceUnitDataTable)
	{
		const auto ResourceDataPtr = ResourceUnitDataTable->FindRow<FResourceUnitData>(BuildingData.ResourceUnitDataKey, TEXT(""));
		if(ResourceDataPtr)
		{
			ResourceUnitData = *ResourceDataPtr;
		}
	}

	//처음부터 빌딩이 활성화 되있을 수 있으므로 bEnable 프로퍼티 기본값으로 SetEnable 호출
	SetEnable(bEnable);

	//Progressbar Widget 업데이트
	HpProgressbar = Cast<UFloatingProgressbarBase>(HpProgressbarWidgetComponent->GetWidget());
	CommonProgressbar = Cast<UFloatingProgressbarBase>(CommonProgressbarWidgetComponent->GetWidget());
	if(HpProgressbar.IsValid() && CommonProgressbar.IsValid())
	{
		SetTextOnProgressbarWidget(CommonProgressbar.Get(), ConstructProgressbarText);
		SetColorOnProgressbarWidget(HpProgressbar.Get(), HpProgressbarColor);
		SetColorOnProgressbarWidget(CommonProgressbar.Get(), ConstructProgressbarColor);
	}
	//ReceiveResourceWidget 업데이트
	ReceiveResourceButtonWidget = Cast<UFloatingButtonBase>(ReceiveResourceWidgetComponent->GetWidget());
	if(ReceiveResourceButtonWidget.IsValid())
	{
		ReceiveResourceButtonWidget->OnClickBtn.BindWeakLambda(this, [this]()
		{
			CreateResource();
			GetWorldTimerManager().SetTimer(CreateResourceHandle, this, &ABuildingSlot::ShowReceiveResourceButton, ResourceUnitData.ResourceCoolTime, false);
		});
	}
	
	UpdateByDataAndManagedList();

	//TODO 테스트 코드
	if(bEnable)
	{
		MakeTroop();
	}
}

void ABuildingSlot::BeginDestroy()
{
	Super::BeginDestroy();
}

void ABuildingSlot::SetMesh() const
{
	const auto ColorData = PlayerColorDataTable->FindRow<FPlayerColorData>(OwnerPlayerColorDataKey, TEXT("ColorDataFind"));
	if(bEnable)
	{
		BuildingMesh->SetStaticMesh(BuildingData.BuildingMesh);
		if(ColorData)
		{
			BuildingMesh->SetMaterial(0, ColorData->BuildingUnitColorMaterial);
		}
	}
	else
	{
		BuildingMesh->SetStaticMesh(BuildingData.PreBuildingMesh);
	}
}

void ABuildingSlot::SetQuestTarget(AActor* ActorRef)
{
	ABuildingSlot* BuildingSlotRef = Cast<ABuildingSlot>(ActorRef);
	UpdateTargetEnemyBuilding(BuildingSlotRef);
}

void ABuildingSlot::UpdateByDataAndManagedList()
{
	SetMesh();
	ShowHpProgressbarWidget(bEnable);
	SetPercentOnProgressbarWidget(HpProgressbar.Get(), CurrentHp, BuildingData.Hp);
	
	GetInstance_Void(UBuildingUnitController,BuildingController, GetWorld())
	//플레이어 일 때만 빌딩 컨트롤러에서 관리하는 빌딩 목록에 추가.
	if(CheckFunc::IsPlayerIndex(OwnerPlayerIndex))
	{
		if(bEnable)
		{
			BuildingController->AddActiveBuilding(this);
		}
		else
		{
			BuildingController->RemoveActiveBuilding(this);
		}
	}
	else
	{
		if(bEnable)
		{
			BuildingController->AddEnemyBuilding(this);
		}
		else
		{
			BuildingController->RemoveEnemyBuilding(this);
		}
	}
}

void ABuildingSlot::ConfirmComplete(const EConfirmPanelType ConfirmPanelType, const bool bResult)
{
	switch (ConfirmPanelType)
	{
	case EConfirmPanelType::ConstructDecision:
		{
			if(bResult && !bConstructing && !bEnable)
			{
				ShowCommonProgressbarWidget(true);
				bConstructing = true;
				GetWorldTimerManager().SetTimer(CheckConstructProcessHandle,this, &ABuildingSlot::CheckConstructProcess,CheckConstructProcessInterval, true);
			}
		}
		break;
	case EConfirmPanelType::BuildingUpgradeDecision:
		{
			if(bResult)
			{
				UpgradeBuilding();
			}
		}
		break;
	case EConfirmPanelType::SetQuestTargetDecision:
		{
			// if(bResult && false == CheckFunc::IsPlayerIndex(OwnerPlayerIndex))
			// {
			// 	GetInstance_Void(APlayerControllerBase, PlayerControllerRef, GetWorld())
			// 	ISetQuestTarget* LastSelectedSlot = Cast<ISetQuestTarget>(PlayerControllerRef->GetLastSelectedSlot());
			// 	if(LastSelectedSlot)
			// 	{
			// 		LastSelectedSlot->SetQuestTarget(this);
			// 	}
			// }
		}
		break;
	default: ;
	}
}

void ABuildingSlot::OnCancelSelection(AActor* ActorRef)
{
	AreaDecal->SetVisibility(false);
}

void ABuildingSlot::ProcessEngineEvent(const EEngineEvent& EngineEvent)
{
	switch (EngineEvent)
	{
	case EEngineEvent::LeftMousePressed:
		{
			GetInstance_Void(AInGameHudBase, HudRef, GetWorld())
			if(bConstructing) 
				return;

			GetInstance_Void(APlayerControllerBase, PlayerControllerRef, GetWorld())
	
			if(CheckFunc::IsPlayerIndex(OwnerPlayerIndex))
			{
				HudRef->OnClickBuildingSlot(this);//활성화 되어 있지 않으면 건설 여부 Confirm창 띄움. 활성화 되어 있다면 건물 상태 창 띄움.
				AreaDecal->SetVisibility(true);
				PlayerControllerRef->SetLastSelectedPlayerBuildingSlot(this);//자신이 선택되었음을 설정.
			}
			else
			{
				// GetInstance_Void(AInGameHudBase, InGameHudBase, GetWorld())
				// InGameHudBase->OpenConfirmPanel(EConfirmPanelType::SetQuestTargetDecision, this);
			}
		}
		break;
	case EEngineEvent::RightMousePressedOnUnit:
		{
			if(CheckFunc::IsNotPlayerIndex(OwnerPlayerIndex))
			{
				GetInstance_Void(APlayerControllerBase, PlayerControllerBase, GetWorld())
				ISetQuestTarget* QuestTargetUnitRef = PlayerControllerBase->GetLastSelectedMovableUnit();
				if(QuestTargetUnitRef)
				{
					QuestTargetUnitRef->SetQuestTarget(this);
				}
			}
		}
		break;
	default: ;
	}
}

void ABuildingSlot::DestroyBuilding()
{
	SetEnable(false);//false 시 자원, 군사 생산 타이머도 끔.
	UpdateByDataAndManagedList();

	HideReceiveResourceButton(); //자원 생산 버튼 감추기.
	OnQuestComplete.Broadcast(); //건물 파괴 퀘스트 이벤트를 알림.

	OnUnitDead.Broadcast(this);//유닛 죽음 이벤트
}

void ABuildingSlot::ShowHpProgressbarWidget(const bool bShow) const
{
	HpProgressbarWidgetComponent->SetVisibility(bShow);
}

void ABuildingSlot::SetPercentOnProgressbarWidget(UFloatingProgressbarBase* ProgressWidgetRef, const float Current,
	const float Max)
{
	if(ProgressWidgetRef)
	{
		ProgressWidgetRef->SetPercent(Current, Max);
	}
}

void ABuildingSlot::SetTextOnProgressbarWidget(UFloatingProgressbarBase* ProgressWidgetRef, const FText& InText)
{
	if(ProgressWidgetRef)
	{
		ProgressWidgetRef->SetText(InText);
	}
}

void ABuildingSlot::CheckConstructProcess()
{
	if(!bEnable) //활성화 전에는 건축 대기시간 진행바로 사용.
	{
		SecondsDuringConstruction += 1;
		SetPercentOnProgressbarWidget(CommonProgressbar.Get(),SecondsDuringConstruction, BuildingData.ConstructionSecond);
	
		if(SecondsDuringConstruction >= BuildingData.ConstructionSecond) //건축 완료
		{
			bConstructing = false;
			SetEnable(true); //true시 자원, 군사 생산 타이머 켬.
			UpdateByDataAndManagedList();
			SecondsDuringConstruction = 0;
			SetPercentOnProgressbarWidget(CommonProgressbar.Get(),0, 0);
			ShowCommonProgressbarWidget(false);
			if(CheckConstructProcessHandle.IsValid())
			{
				GetWorldTimerManager().ClearTimer(CheckConstructProcessHandle);
			}
		}
	}
}

void ABuildingSlot::SetColorOnProgressbarWidget(UFloatingProgressbarBase* ProgressWidgetRef, const FLinearColor& InColor)
{
	if(ProgressWidgetRef)
	{
		ProgressWidgetRef->SetFillColor(InColor);
	}
}

void ABuildingSlot::ShowCommonProgressbarWidget(const bool bShow) const
{
	CommonProgressbarWidgetComponent->SetVisibility(bShow);
}

int ABuildingSlot::CalculateRequiredResourceForUpgrade(const int InitialCost, const float CostMultiplier, const int BuildingLevel)
{
	if(BuildingLevel == 0 || InitialCost == 0)
		return InitialCost;

	int ResultCost = InitialCost;
	for(int Index = 0; Index <= BuildingLevel; ++Index)
	{
		ResultCost = ResultCost * CostMultiplier;
	}

	return ResultCost;
}

void ABuildingSlot::UpdateTargetEnemyBuilding(ABuildingSlot* TargetBuildingSlot)
{
	if(!TargetBuildingSlot)
		return;
	
	if(false == TargetBuildingSlot->IsEnable())
		return;
}


float ABuildingSlot::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                                AActor* DamageCauser)
{
	if(!bEnable)
		return false;
	
	CurrentHp -= DamageAmount;
	SetPercentOnProgressbarWidget(HpProgressbar.Get(),CurrentHp, BuildingData.Hp);
	
	if(CurrentHp <= 0)
	{
		DestroyBuilding();
	}
	
	return DamageAmount;
}

void ABuildingSlot::SetEnable(const bool InEnable)
{
	bEnable = InEnable;
	//활성화 되었음으로 자원/군사 타이머 업데이트.
	UpdateCreateResourceTimer(bEnable);
	UpdateSpawnSoldierTimer(bEnable);
}

void ABuildingSlot::UpdateCreateResourceTimer(const bool bCurrentEnableState)
{
	if(bCurrentEnableState)
	{
		if(CheckFunc::IsPlayerIndex(OwnerPlayerIndex)) //플레이어일 땐 UI 클릭을 통해 자원을 얻도록 설정.
		{
			GetWorldTimerManager().SetTimer(CreateResourceHandle, this, &ABuildingSlot::ShowReceiveResourceButton, ResourceUnitData.ResourceCoolTime, false);
		}
		else
		{
			GetWorldTimerManager().SetTimer(CreateResourceHandle, this, &ABuildingSlot::CreateResource, ResourceUnitData.ResourceCoolTime, true);
		}
	}
	else
	{
		if(CreateResourceHandle.IsValid())
			GetWorldTimerManager().ClearTimer(CreateResourceHandle);
	}
}

void ABuildingSlot::MakeTroop()
{
	if(!bEnable || OwnerPlayerIndex == INVALID_INDEX)
		return;
	
	GetInterface_Void(UMovableUnitSpawner, UMovableUnitSpawner, UnitController, GetWorld());
	
	const FName& MovableUnitDataKey = ResourceUnitData.SpawnSoldierList[0];//TODO 하드코딩
	UnitController->Spawn(SpawnPoint->GetComponentLocation(), MovableUnitDataKey, {OwnerPlayerIndex, OwnerPlayerColorDataKey},  26999, SpawnedUnitBuildingSlotProxy);//TODO 수정 필요
}

bool ABuildingSlot::GetSpawnPointYaw(float& OutYaw) const
{
	OutYaw = SpawnPoint->GetComponentRotation().Yaw;
	
	return true; //TODO 반환값 void로 바꿀것.
}

void ABuildingSlot::ShowReceiveResourceButton()
{
	if(ReceiveResourceWidgetComponent)
	{
		ReceiveResourceWidgetComponent->SetVisibility(true);
	}
}

void ABuildingSlot::HideReceiveResourceButton()
{
	if(ReceiveResourceWidgetComponent)
	{
		ReceiveResourceWidgetComponent->SetVisibility(false);
	}
}

void ABuildingSlot::CreateResource() const
{
	if(!bEnable || OwnerPlayerIndex == INVALID_INDEX)
		return;

	OnUpdateResources.Broadcast(OwnerPlayerIndex, ResourceUnitData);
	ReceiveResourceWidgetComponent->SetVisibility(false);
}

void ABuildingSlot::SpawnSoldierType()
{
	
}

void ABuildingSlot::UpdateSpawnSoldierTimer(const bool bCurrentEnableState)
{
	if(ResourceUnitData.SpawnSoldierList.Num() > 0)
	{
		if(bCurrentEnableState)
		{
			GetWorldTimerManager().SetTimer(CreateSoldierHandle, this, &ABuildingSlot::SpawnSoldierType, ResourceUnitData.SpawnCoolTime, true);
		}
		else
		{
			if(CreateSoldierHandle.IsValid())
				GetWorldTimerManager().ClearTimer(CreateSoldierHandle);
		}
	}
}

void ABuildingSlot::UpgradeBuilding()
{
	GetInstance_Void(APlayerStateBase, PlayerStateBase, GetWorld())
	//재료가 부족하면 진행시키지 않음.
	if(PlayerStateBase->GoldCount < BuildingData.GoldCost || PlayerStateBase->StoneCount < BuildingData.StoneCost || PlayerStateBase-> WoodCount < BuildingData.WoodCost)
		return;
	
	PlayerStateBase->ConsumeResource(EResourceType::Gold, BuildingData.GoldCost);
	PlayerStateBase->ConsumeResource(EResourceType::Wood, BuildingData.WoodCost);
	PlayerStateBase->ConsumeResource(EResourceType::Stone, BuildingData.StoneCost);

	//빌딩 정보창이 켜져있다면 업데이트 요청 보냄.
	GetInstance_Void(APlayerControllerBase, PlayerControllerRef, GetWorld())
	PlayerControllerRef->OnNeedToUpdateBuildingStatusPanel.Broadcast(this);//BuildingStatusPanel에서 받음.
}

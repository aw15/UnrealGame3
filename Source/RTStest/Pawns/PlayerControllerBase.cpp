// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"

#include "PlayerBase.h"
#include "ControllerComponent/BuildingUnitController.h"
#include "ControllerComponent/MovableUnitSpawner.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "RTStest/Hud/InGameHudBase.h"
#include "Unit/MovableUnit.h"
#include "RTStest/Common/EngineEventInterface.h"
#include "RTStest/Enviroment/BuildingSlot.h"

APlayerControllerBase::APlayerControllerBase()
{
	PrimaryActorTick.bCanEverTick = true;
	BuildingController = CreateDefaultSubobject<UBuildingUnitController>(TEXT("BuildingController"));
	MovableUnitController = CreateDefaultSubobject<UMovableUnitSpawner>(TEXT("MovableUnitController"));
}

inline void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

DefineInstanceGetter(APlayerControllerBase)
{
	return Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(WorldContextObject,0));
}

void APlayerControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APlayerControllerBase::EdgeScrolling() const
{
	const auto PlayerRef = GetPlayer();
	if(!PlayerRef)
		return;
	
	int SizeX = 0, SizeY = 0;
	GetViewportSize(SizeX, SizeY);
	if(SizeX == 0 || SizeY == 0)
		return;
	float MouseX = 0, MouseY = 0;
	GetMousePosition(MouseX, MouseY);
	//UE_LOG(LogTemp, Warning, TEXT("mouseX %f mouseY %f"), MouseX, MouseY);

	if(MouseX < FLT_EPSILON || MouseY < FLT_EPSILON)
		return;
	
	const float XPercent = MouseX/static_cast<float>(SizeX);
	const float YPercent = MouseY/static_cast<float>(SizeY);
	
	if(XPercent > 0.975)
	{
		PlayerRef->FloatingPawnMovement->AddInputVector(FVector(0,XPercent,0));
	}
	else if (XPercent < 0.025)
	{
		PlayerRef->FloatingPawnMovement->AddInputVector(FVector(0,XPercent - 1,0));
	}

	if(YPercent > 0.975)
	{
		PlayerRef->FloatingPawnMovement->AddInputVector(FVector(YPercent * -1,0,0));
	}
	else if (YPercent < 0.025)
	{
		PlayerRef->FloatingPawnMovement->AddInputVector(FVector(1 - YPercent,0,0));
	}
}

void APlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();
	if(InputComponent)
	{
		InputComponent->BindAxis("Zoom",this, &APlayerControllerBase::Zoom);
		InputComponent->BindAxis("MoveForward",this, &APlayerControllerBase::MoveVertical);
		InputComponent->BindAxis("MoveRight",this, &APlayerControllerBase::MoveHorizontal);
		InputComponent->BindAction("LeftMouseButton",IE_Pressed, this, &APlayerControllerBase::LeftMousePressed);
		//InputComponent->BindAction("LeftMouseButton",IE_Released, this, &APlayerControllerBase::LeftMouseRelease);
		InputComponent->BindAction("RightMouseButton",IE_Pressed, this, &APlayerControllerBase::RightMousePressed);
	}
}

void APlayerControllerBase::SetLastSelectedPlayerBuildingSlot(ABuildingSlot* BuildingSlotRef)
{
	if(LastSelectedPlayerBuildingSlot.IsValid())
	{
		LastSelectedPlayerBuildingSlot->OnCancelSelection(LastSelectedPlayerBuildingSlot.Get());
	}
	LastSelectedPlayerBuildingSlot = BuildingSlotRef;
}

AActor* APlayerControllerBase::GetLastSelectedSlot() const
{
	return LastSelectedPlayerBuildingSlot.Get();
}

ISetQuestTarget* APlayerControllerBase::GetLastSelectedMovableUnit() const
{
	return LastSelectedMovableUnit.Get();
}

void APlayerControllerBase::ClearLastSelectedBuildingSlot()
{
	if(LastSelectedPlayerBuildingSlot.IsValid())
	{
		LastSelectedPlayerBuildingSlot->OnCancelSelection(LastSelectedPlayerBuildingSlot.Get());
	}
	LastSelectedPlayerBuildingSlot.Reset();
}

void APlayerControllerBase::SetLastSelectedMovableUnit(AMovableUnit* InMovableUnitRef)
{
	if(LastSelectedMovableUnit.IsValid())
	{
		LastSelectedMovableUnit->OnSelectionCancel();
	}

	LastSelectedMovableUnit = InMovableUnitRef;
}

APlayerBase* APlayerControllerBase::GetPlayer() const
{
	APlayerBase* PlayerRef = Cast<APlayerBase>(GetPawn());

	return PlayerRef;
}

void APlayerControllerBase::Zoom(const float Amount)
{
	const auto PlayerRef = GetPlayer();
	if(!PlayerRef)
		return;
	
	PlayerRef->Zoom(Amount);
}

void APlayerControllerBase::MoveVertical(const float Amount)
{
	const auto PlayerRef = GetPlayer();
	if(!PlayerRef)
		return;
	PlayerRef->MoveVertical(Amount);
}

void APlayerControllerBase::MoveHorizontal(const float Amount)
{
	const auto PlayerRef = GetPlayer();
	if(!PlayerRef)
		return;

	PlayerRef->MoveHorizontal(Amount);
}

void APlayerControllerBase::LeftMousePressed()
{
	Cast<AInGameHudBase>(GetHUD()) ? Cast<IEngineEventListener>(GetHUD())->ProcessEngineEvent(EEngineEvent::LeftMousePressed) : NULL;
	
	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility/*interactable*/),false, HitResult);
	if(HitResult.IsValidBlockingHit())
	{
		OnResetUiSelectionEvent.Broadcast();	
	}
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(TRACE_TYPE_INTERACTABLE),false, HitResult);
	if(HitResult.IsValidBlockingHit())
	{
		IEngineEventListener* EngineEventListener = Cast<IEngineEventListener>(HitResult.GetActor());
		if(EngineEventListener)
		{
			EngineEventListener->ProcessEngineEvent(EEngineEvent::LeftMousePressed);
		}
	}

	// if(false == SlotHitResult.IsValidBlockingHit() && HitResult.IsValidBlockingHit())
	// {
	// 	LastSelectedPlayerUnitSlot.Reset();
	// 	LastSelectedPlayerBuildingSLot.Reset();
	// 	PrintScreenMacro("순수 땅바닥 클릭");
	// }
}

// void APlayerControllerBase::LeftMouseRelease() 
// {
// 	Cast<AInGameHudBase>(GetHUD()) ? Cast<IEngineEvent>(GetHUD())->ProcessEngineEvent(EEngineEvent::LeftMouseReleased) : NULL;
// }

void APlayerControllerBase::RightMousePressed()
{
	//이동을 위해 땅쪽 trace는 무조건 계산.
	FHitResult GroundHitResult;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility),false, GroundHitResult);
	if(GroundHitResult.IsValidBlockingHit())
	{
		LastRightMouseHitLocation = GroundHitResult.Location;
	}
	
	FHitResult UnitHitResult;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(TRACE_TYPE_INTERACTABLE),false, UnitHitResult);
	
	if(UnitHitResult.IsValidBlockingHit())
	{
		IEngineEventListener* EngineEventListener = Cast<IEngineEventListener>(UnitHitResult.GetActor());
		EngineEventListener->ProcessEngineEvent(EEngineEvent::RightMousePressedOnUnit);
		return;
	}
	
	if(GroundHitResult.IsValidBlockingHit() && LastSelectedMovableUnit.IsValid())
	{
		LastSelectedMovableUnit->ProcessEngineEvent(EEngineEvent::RightMousePressed);
	}
}

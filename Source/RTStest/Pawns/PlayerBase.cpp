// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
APlayerBase::APlayerBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	RootComponent = SphereMesh;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(SphereMesh);
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>("FloatingPawnMovement");
}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
	SpringArmComp->TargetArmLength = SpringArmLenghthDefault;
	XScrollSpeed = XScrollSpeedDefault;
	YScrollSpeed = YScrollSpeedDefault;
}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APlayerBase::Zoom(const float Amount)
{
	if(!GetWorld())
		return;
	
	if(!SpringArmComp)
		return;
	
	const float NewLength =SpringArmComp->TargetArmLength + (Amount * ZoomSpeed * GetWorld()->GetDeltaSeconds());
	// if(GEngine)
	// 	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%f"), NewLength));
	
	 float FinalLength = FMath::Lerp(SpringArmComp->TargetArmLength, NewLength, 0.7f);
	FinalLength = FMath::Clamp(FinalLength, SpringArmLengthMin, SpringArmLenghthMax);

	
	SpringArmComp->TargetArmLength = FinalLength;
}

void APlayerBase::MoveHorizontal(const float Amount)
{
	const float NewValue = Amount * YScrollSpeed;
	FloatingPawnMovement->AddInputVector(FVector(0.f,NewValue ,0.f));
	
}

void APlayerBase::MoveVertical(const float Amount)
{
	const float NewValue = Amount * YScrollSpeed;
	FloatingPawnMovement->AddInputVector(FVector(NewValue, 0.f,0.f));
}

void APlayerBase::ResetCamera(const FVector& Location)
{
	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.CallbackTarget = this;
	LatentActionInfo.UUID = GetUniqueID();
	LatentActionInfo.Linkage = 0;
	UKismetSystemLibrary::MoveComponentTo(RootComponent,Location, GetActorRotation(),false, false,0.2f, false,EMoveComponentAction::Move, LatentActionInfo);
	SetActorLocation(Location);
}

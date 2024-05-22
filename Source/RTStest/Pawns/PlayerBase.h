// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerBase.generated.h"

class UFloatingPawnMovement;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class RTSTEST_API APlayerBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Zoom(const float Amount);
	void MoveHorizontal(const float Amount);
	void MoveVertical(const float Amount);
	void ResetCamera(const FVector& Location);
	
protected:
	//DEFAULT
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpringArmLenghthDefault = 1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float XScrollSpeedDefault = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float YScrollSpeedDefault = 1000.f;

	//OPTION
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float XScrollSpeed = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float YScrollSpeed = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ZoomSpeed = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpringArmLengthMin = 800.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpringArmLenghthMax = 1800.f;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Component")
	UFloatingPawnMovement* FloatingPawnMovement;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Component")
	UStaticMeshComponent* SphereMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Component")
	USpringArmComponent* SpringArmComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Component")
	UCameraComponent* CameraComp;
};

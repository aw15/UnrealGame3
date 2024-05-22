// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPath.h"


// Sets default values
APatrolPath::APatrolPath()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Path = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
	Path->SetupAttachment(RootComponent);
	//TODO 디버깅용
	Path->bDrawDebug = true;
}

void APatrolPath::GetSplinePoints()
{
	for(int Index = 0; Index < Path->GetNumberOfSplinePoints(); ++Index)
	{
		Locations.Add(Path->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World));
	}
}

// Called when the game starts or when spawned
void APatrolPath::BeginPlay()
{
	Super::BeginPlay();
	GetSplinePoints();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	StartPoint = FVector(0.f);

	EndPoint = FVector(0.f);

	interpspeed = 4.f;

	InterpTime = 0.1f;

	bInterping = false;

}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartPoint = GetActorLocation();

	EndPoint += StartPoint;

	bInterping = false;

	GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterpeling, InterpTime);
	Distance = (EndPoint - StartPoint).Size();
	
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterping)
	{

		FVector CurrentLocation = GetActorLocation();

		FVector Interp = FMath::VInterpTo(CurrentLocation,EndPoint,DeltaTime,interpspeed);
		SetActorLocation(Interp);
		float DistanceTraveled = (GetActorLocation() - StartPoint).Size();

		if (Distance - DistanceTraveled <= 1.f) {

			ToggleInterpeling();
			GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterpeling, InterpTime);

			SwapVectors(StartPoint, EndPoint);


		}}}
void AFloatingPlatform::SwapVectors(FVector& Vec1, FVector& Vec2)
{
	FVector Temp = Vec1;
	Vec1 = Vec2;
	Vec2 = Temp;
}

void AFloatingPlatform::ToggleInterpeling()
{
	bInterping = !bInterping;
}




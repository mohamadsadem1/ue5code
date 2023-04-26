// Fill out your copyright notice in the Description page of Project Settings.


#include "Schar_gamemode.h"
#include "GameFramework/Actor.h"

ASchar_gamemode::ASchar_gamemode() {
	PrimaryActorTick.bCanEverTick = true;
}
	
	void ASchar_gamemode::BeginPlay() {
		Super::BeginPlay();
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(
			UnusedHandle, this, &ASchar_gamemode::SpawnPlayerRecharge, FMath::RandRange(1,3), true);
	
	}



	void ASchar_gamemode::Tick(float DeltaTime) {
		Super::Tick(DeltaTime);
	}

	void ASchar_gamemode::SpawnPlayerRecharge()
	{
		float RandX = FMath::RandRange(Spawn_X_Min, Spawn_X_Max);
		float RandY = FMath::RandRange(Spawn_Y_Min, Spawn_Y_Max);

		FVector SpawnPosition = FVector(RandX, RandY, Spawn_Z);
		FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
		GetWorld()->SpawnActor(PlayerRecharge, &SpawnPosition, &SpawnRotation);
	}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Schar_gamemode.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT9_API ASchar_gamemode : public AGameModeBase
{
	GENERATED_BODY()
		ASchar_gamemode();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime)override;
	UPROPERTY(editAnywhere)
		TSubclassOf<APawn> PlayerRecharge;
	float Spawn_Z= 500.0f;

	UPROPERTY(editAnywhere)
		float Spawn_X_Min;

	UPROPERTY(editAnywhere)
		float Spawn_X_Max;

	UPROPERTY(editAnywhere)
		float Spawn_Y_Min;

	UPROPERTY(editAnywhere)
		float Spawn_Y_Max;

	void SpawnPlayerRecharge();
};

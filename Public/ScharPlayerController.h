// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ScharPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT9_API AScharPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Widget")
	TSubclassOf<class UUserWidget>HUDoverlayAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
		UUserWidget* HUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUserWidget>WEnemyHealthBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widget")
	UUserWidget* EnemyHealthBar;

	bool bEnemyHealthBarVisible;

	void DisplayEnemyhealthBar();
	void RemoveEnemyHealthBar();
	FVector EnemyLocation;
protected:
	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime) override;
};

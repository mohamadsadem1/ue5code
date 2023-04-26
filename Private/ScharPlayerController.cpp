// Fill out your copyright notice in the Description page of Project Settings.


#include "ScharPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Schar.h"


void AScharPlayerController::BeginPlay()
{
	Super::BeginPlay();


	if (HUDoverlayAsset) 
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDoverlayAsset );

	}
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	if (WEnemyHealthBar)
	{
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
		if (EnemyHealthBar)
		{
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);

			FVector2D Vec(0.f, 0.f);
			EnemyHealthBar->SetAlignmentInViewport(Vec);
		}
	}
}

void AScharPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (EnemyHealthBar)
	{
		FVector2D PositionViewPort;
		ProjectWorldLocationToScreen(EnemyLocation, PositionViewPort);
		PositionViewPort.Y -= 50.f;


		FVector2D SizeInViewPort = FVector2D(200.f, 25.f);

		EnemyHealthBar->SetPositionInViewport(PositionViewPort);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewPort);
	}

}


void AScharPlayerController::DisplayEnemyhealthBar()
{
	if (EnemyHealthBar)
	{

		bEnemyHealthBarVisible = true;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
	}
}

void AScharPlayerController::RemoveEnemyHealthBar()
{
	if (EnemyHealthBar)
	{

		bEnemyHealthBarVisible = false;
		EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
	}
}
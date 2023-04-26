// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "collect.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT9_API APickup : public Acollect
{

	GENERATED_BODY()

public:
		APickup();


	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;


	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	UFUNCTION(BlueprintImplementableEvent, Category= "Pickup")
		void OnPickup(class ASchar*Target);
	
};

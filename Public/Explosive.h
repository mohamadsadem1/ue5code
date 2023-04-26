// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "collect.h"
#include "Explosive.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT9_API AExplosive : public Acollect
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned

public:
	AExplosive();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Damage")
	float Damage;
	
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
				UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
				AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;


};

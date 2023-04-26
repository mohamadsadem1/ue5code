// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "collect.generated.h"

UCLASS()
class MYPROJECT9_API Acollect : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	Acollect();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Collect | Mesh ")
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = " Collect | Collision ")
		class UBoxComponent* Collision;

	UPROPERTY(EditAnywhere, BluePrintReadWrite , Category = " Collect | Particles ")
		class UParticleSystemComponent* IdleParticleComponent;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = " Collect | Particles ")
		class UParticleSystem* OverlapParticle;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = " Collect | Sound ")
		class USoundCue* OverlapSound;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = " Collect | CollectProperties ")
	bool bRotate;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = " Collect | CollectProperties ")
		float RotationRate;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd( UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};

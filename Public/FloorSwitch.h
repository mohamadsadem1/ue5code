// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FloorSwitch.generated.h"

UCLASS()
class MYPROJECT9_API AFloorSwitch : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFloorSwitch();
	//overlap volume for the functionality to be trigger
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="FloorSwitch")
		class UBoxComponent* TriggerBox;

	//the switchfor the characetr to step on
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FloorSwitch")
		class UStaticMeshComponent* FloorSwitch;

	//door move when step on the switch
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FloorSwitch")
		UStaticMeshComponent* Door;

	UPROPERTY(BlueprintReadWrite,Category="FloorSwitch")
	FVector InitialDoorLoaction;

	UPROPERTY(BlueprintReadWrite, Category = "FloorSwitch")
		FVector InitialSwitchLoaction;

	UPROPERTY(EditAnywhere,Category="FloorSwicth")
	float SwicthTime;

	bool bCharacterOnSwitch;

protected:
	// Called when the game starts or when spawned
		virtual void BeginPlay() override;

public:	
	// Called every frame
		virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	    void OnOverlapBegin( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd( UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent*OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent, Category = "FloorSwitch")
		void RaiseDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "FloorSwitch")
		void LowerDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "FloorSwitch")
		void RaiseFloorSwitch();

	UFUNCTION(BlueprintImplementableEvent, Category = "FloorSwitch")
		void LowerFloorSwitch();

	UFUNCTION(BlueprintCallable, Category = "FloorSwitch")
	void UpdateDoorLocation(float z);

	UFUNCTION(BlueprintCallable, Category = "FloorSwitch")
		void UpdateSwitchFloorLocation(float z);

	FTimerHandle SwitchHandle; 

	void CloseDoor();

};

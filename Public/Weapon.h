// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "collect.h"
#include "Weapon.generated.h"


UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Pickup		 UMETA(Display_Name = "Pickup"),
	EWS_Wquipped	 UMETA(Display_Name = "Equipped"),

	EWS_Max   UMETA(Display_Name = "Deafaultmax")

};
/*
 * 
 */
UCLASS()
class MYPROJECT9_API  AWeapon : public Acollect
{
	GENERATED_BODY()


public:
	AWeapon();


	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = "Collect ")
	EWeaponState WeaponState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "collect | Particules")
		bool bWeaponParticules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "collect | Sound")
		class USoundCue* OnEquipSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
		class USkeletalMeshComponent* SkeletalMesh1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "collect | Combat")
		class UBoxComponent* CombatCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "collect | Combat")
		float Damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="collect | Combat")
	bool bWeaponOverlaing;

private:
	virtual void BeginPlay()override;


public:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;


	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;

	void Equip(class ASchar* Char);

	FORCEINLINE void SetWeaponState(EWeaponState state) { WeaponState = state; }
	FORCEINLINE EWeaponState GetWeaponState() {return WeaponState;}

	UFUNCTION(BlueprintCallable)
	void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	 void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
		void ActivateCollision();

	UFUNCTION(BlueprintCallable)
		void DeactivateCollision();

	UFUNCTION(BlueprintCallable)
		void PlaySwingSoundSword();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "collect | sound")
		USoundCue* SwingSound;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Combat")
	TSubclassOf<UDamageType>DamageTypeClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	AController* WeaponInstigator;

	FORCEINLINE void SetInstigator(AController* Inst) 
	{
		WeaponInstigator = Inst;
	}
};




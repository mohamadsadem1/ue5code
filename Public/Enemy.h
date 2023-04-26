// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnimyMovementStatus :uint8
{
	EMS_Idle		    UMETA(DisplayName = "Idle"),
	EMS_MoveToTarget	UMETA(DisplayName = "MoveToTarget"),
	EMS_Attackking		UMETA(DisplayName = "Attackking"),
	EMS_Dead			UMETA(DisplayName = "Dead"),

	EMS_Max				UMETA(DisplayName = "Default")

};





UCLASS()
class MYPROJECT9_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	/*
				ENUM
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		EEnimyMovementStatus EnimyMovementStatus;

	FORCEINLINE void SetEnimyMovementStatus(EEnimyMovementStatus  Status) { EnimyMovementStatus = Status; }

	FORCEINLINE EEnimyMovementStatus GetEnimyMovementStatus() { return EnimyMovementStatus; }

public:
	/*
			MESHES
	*/

	/*
						AGRO SPHERE BEGIN/END
	*/
	UFUNCTION()
		virtual void AgroOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void AgroOnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	/*

					COMBAT SPHERE FOR THE ENEMY BEGIN/END
	*/
	UFUNCTION()
		virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	/*

					COMBAT FOR THE ENEMY'S MELE BEGIN/END
	*/
	UFUNCTION()
	void MeleCombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void MeleCombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* AgroSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* CombatSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	class UBoxComponent* MeleCombatCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	bool bOverlapingWithAgro;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	bool bOverlapingWithMeleCombat;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	bool bOverlapingWithCombatSphere;

	bool bDead;
	  
	bool bHasValidTarget;


	/*
				GAME MECANICS
	*/
	UFUNCTION(BlueprintCallable)
		void ActivateCollision();

	UFUNCTION(BlueprintCallable)
		void DeactivateCollision();

	UFUNCTION(BlueprintCallable)
		void AttackEnd();
	
	UFUNCTION(BlueprintCallable)
	void Attack();

	FORCEINLINE void SetCombatTarget(ASchar* Target) { CombatTarget = Target; }

	FRotator GetLookAtRotationYawForCharacter(FVector Target);
	float InterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	bool bInterpTocharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TSubclassOf<UDamageType>DamageTypeClass;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;

	FTimerHandle DeathTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DeathDealy;

	void Die(AActor*Causer);

	UFUNCTION(BlueprintCallable)
		void DeathEnd();

	bool Alive();

	void Disapear();

	


	/*
				STATS
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float Enemy_Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float Enemy_MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float Damage;




	/*
				SOUNDS
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		class USoundCue* HitSound_Enemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		USoundCue* EnemySwingSound;

	UFUNCTION(BlueprintCallable)
		void  PlayHitSound();

	UFUNCTION(BlueprintCallable)
		void EnemyPlaySwingSound();


	/*
			PARTICLES
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		class UParticleSystem* HitParticle_Enemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		bool bAttacking;

	DECLARE_EVENT(Enemy, FOnDeathEvent);



public:


	/*
			AI
	*/
	UFUNCTION(BlueprintCallable)
		void MoveToTarget(class ASchar* Target);


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		ASchar* CombatTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "combat")
		class UAnimMontage* CombatMontage;

	FTimerHandle AttackTimer;
	FTimerHandle MoveTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "combat")
		float AttackMinTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "combat")
		float AttackMaxTime;


};
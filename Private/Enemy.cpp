// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h" 
#include "Schar.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Schar.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "ScharPlayerController.h"





// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AgroSphere = CreateDefaultSubobject<USphereComponent>("AgroSphere");
	AgroSphere->SetupAttachment(GetRootComponent());
	AgroSphere->InitSphereRadius(600.f);


	CombatSphere = CreateDefaultSubobject<USphereComponent>("CombatSphere");
	CombatSphere->SetCollisionProfileName(TEXT("AgroSphere_Weapon"));
	CombatSphere->SetupAttachment(GetRootComponent());
	CombatSphere->InitSphereRadius(75.f);


	MeleCombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	MeleCombatCollision->SetupAttachment(GetMesh(), FName("EnemySocket"));

	MeleCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	bOverlapingWithCombatSphere = false;
	bOverlapingWithMeleCombat = false;
	bOverlapingWithAgro = false;

	Enemy_Health = 75.f;
	Enemy_MaxHealth = 100.f;
	Damage = 10.f;
	AttackMinTime = 0.8f;
	AttackMaxTime = 3.5f;

	bInterpTocharacter = false;
	InterpSpeed = 15.f;
	EnimyMovementStatus = EEnimyMovementStatus::EMS_Idle;

	DeathDealy=3.f;

	bHasValidTarget = false;
	
}




// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	AIController = Cast<AAIController>(GetController());
	AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroOnOverlapBegin);
	AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroOnOverlapEnd);

	CombatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	CombatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);

	MeleCombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::MeleCombatOnOverlapBegin);
	MeleCombatCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::MeleCombatOnOverlapEnd);


	//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float Health_perc = Enemy_Health / Enemy_MaxHealth;

	if (bInterpTocharacter && CombatTarget)
	{
		FRotator LookAtYawn = GetLookAtRotationYawForCharacter(CombatTarget->GetActorLocation() );
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYawn, DeltaTime, InterpSpeed);
		SetActorRotation(InterpRotation);
	}
}

FRotator AEnemy::GetLookAtRotationYawForCharacter(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
/*
									AGRO SPHERE BEGIN OVERLAP
*/
void AEnemy::AgroOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bOverlapingWithAgro = true;
	if (OtherActor && Alive())
	{
		ASchar* Schar = Cast<ASchar>(OtherActor);
		if (Schar)
		{
			bHasValidTarget = true;
			CombatTarget = Schar;
			
			Schar->SetCombatTarget(this);
			

				MoveToTarget(CombatTarget);
		}
	}
}

/*
									AGRO SPHERE END OVERLAP
*/
void AEnemy::AgroOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bOverlapingWithAgro = false;
	if (OtherActor)
	{
		ASchar* Schar = Cast<ASchar>(OtherActor);
		if (Schar)
		{	
			//enemy don't have a valid target. target=null
			bHasValidTarget = false;

			//character don't have a target 
			Schar->CombatTartg = nullptr;

			//set the enemy status to idle
			SetEnimyMovementStatus(EEnimyMovementStatus::EMS_Idle);

		//	if (Schar->CombatTartg == this)
		//	{
		//		Schar->SetHasCombatTarget(false);
		//	}
			Schar->UpdateCombatTarget();

			//enemy dont have a target
			CombatTarget = nullptr;

			if (AIController)
			{
				AIController->StopMovement();
			}

		}
	}
}
/*
								    COMBAT SPHERE BEGIN OVERLAP
*/
void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && Alive())
	{
		ASchar* Schar = Cast<ASchar>(OtherActor);
		if (Schar)
		{
			bOverlapingWithCombatSphere = true;

			//interping to character
			Schar->SetInterpToEnmey(true);

			//initialize the enemy as a target for the Schar(Character)
		//	Schar->SetCombatTarget(this);
			Schar->SetHasCombatTarget(true);

			//
			SetCombatTarget(Schar);
			
			
			Schar->UpdateCombatTarget();

			Attack();
			
			float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		}
	}
}
/*
									COMBAT SPHERE END OVERLAP							
*/
void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherComp)
	{

		ASchar* Schar = Cast<ASchar>(OtherActor);
		if (Schar)
		{
			bOverlapingWithCombatSphere = false;
			
			//combat target was defined in the agro sphere overlap begin
			MoveToTarget(CombatTarget);
			Schar->SetInterpToEnmey(false);

			if (Schar->CombatTartg == this)
			{
				Schar->SetCombatTarget(nullptr);

				Schar->bHasAcombatTarget = false;

				Schar->UpdateCombatTarget();
			}


			if (Schar->ScharPlayerController)
			{
				USkeletalMeshComponent* ScharMesh = Cast<USkeletalMeshComponent>(OtherComp);
				if (ScharMesh)
					Schar->ScharPlayerController->RemoveEnemyHealthBar();
			}

			GetWorldTimerManager().ClearTimer(AttackTimer);
			
		}

	}
}
/*
									MELE COMBAT ON BEGIN OVERLAP
*/
void AEnemy::MeleCombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bOverlapingWithMeleCombat = true;

	bAttacking = true;
	if (OtherActor && Alive() )
	{
		ASchar* Schar = Cast<ASchar>(OtherActor);
		if (Schar)
		{
			if (Schar->HitParticle_Schar)
			{


				const USkeletalMeshSocket* TipSocket = GetMesh()->GetSocketByName("EnemySocketParticle");

				if (TipSocket)
				{
					FVector SocketLocation = TipSocket->GetSocketLocation(GetMesh());

					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Schar->HitParticle_Schar, GetActorLocation(), FRotator(0.f));

				

				}
				PlayHitSound();


				if (DamageTypeClass)
				{
					UGameplayStatics::ApplyDamage(Schar, Damage, AIController, this, DamageTypeClass);
				}

			}
		}
	}

}
/*
									MELE COMBAT END
*/
void AEnemy::MeleCombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bOverlapingWithMeleCombat = false;

	if (OtherActor )
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy)
		{
			if (bOverlapingWithAgro)
			{
				MoveToTarget(CombatTarget);
			}	
			SetEnimyMovementStatus(EEnimyMovementStatus::EMS_Idle);

		}

	}
}

/*
									MOVE TO TARGET METHOD

*/
void AEnemy::MoveToTarget(ASchar* Target)
{
	SetEnimyMovementStatus(EEnimyMovementStatus::EMS_MoveToTarget);
	if (AIController)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(Target);
		MoveRequest.SetAcceptanceRadius(5.f);
		

		FNavPathSharedPtr NavPath;

		AIController->MoveTo(MoveRequest, &NavPath);
	}

}

/*
					ATTACK FUNCTION FOR THE AI 
					ANIMATION/SOUND/PARTICLE
*/
void AEnemy::Attack()
{
	if (Alive() && bHasValidTarget) {

		if (AIController)
		{
			AIController->StopMovement();
			SetEnimyMovementStatus(EEnimyMovementStatus::EMS_Attackking);

		}
		if (!bAttacking)
		{
			bAttacking = true;
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(CombatMontage, 1.2f);
				AnimInstance->Montage_JumpToSection(FName("attack"), CombatMontage);
			}
		}

	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Enemy_Health - DamageAmount <= 0.f)
	{
		Enemy_Health -= DamageAmount;
		Die(DamageCauser);
	}
	else
	{
		Enemy_Health -= DamageAmount;
	}
	return DamageAmount;

}


/*
		ATTACK END
*/
void AEnemy::AttackEnd()
{
	bAttacking = false;
	if (bOverlapingWithCombatSphere)
	{
	//	MoveToTarget(CombatTarget);

		float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, 1);
	}
	if (bOverlapingWithAgro && !bOverlapingWithCombatSphere)
	{
		MoveToTarget(CombatTarget);
	}
}
/*
		SWORD COLLISION
*/
void AEnemy::ActivateCollision()
{
	MeleCombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
void AEnemy::DeactivateCollision()
{
	MeleCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
/*
		SOUNDS
*/

void AEnemy::PlayHitSound()
{
	if (CombatTarget ->HitSound_Schar)
	{
		UGameplayStatics::PlaySound2D(this, CombatTarget->HitSound_Schar);
	}
}
void AEnemy::EnemyPlaySwingSound()
{
	UGameplayStatics::PlaySound2D(this, EnemySwingSound);
	
		if (EnemySwingSound)
		{
			UGameplayStatics::PlaySound2D(this, EnemySwingSound);
		}
	
}

void AEnemy:: DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disapear, DeathDealy);
}

void AEnemy::Disapear()
{
	Destroy();
}

bool AEnemy::Alive()
{
	return GetEnimyMovementStatus() != EEnimyMovementStatus::EMS_Dead;
}


void AEnemy::Die(AActor* Causer)
{

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && CombatMontage)
	{
		AnimInstance->Montage_Play(CombatMontage, 1.0f);
		AnimInstance->Montage_JumpToSection(FName("Death"));
	}
	bDead = true;

	SetEnimyMovementStatus(EEnimyMovementStatus::EMS_Dead);
	MeleCombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bAttacking = false;

	ASchar* Schar = Cast<ASchar>(Causer);
	if (Schar)
	{
		Schar->UpdateCombatTarget();

	}

}



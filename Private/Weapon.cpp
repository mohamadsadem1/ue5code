 // Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Schar.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "collect.h"
#include "Schar.h"
#include "Enemy.h"




AWeapon::AWeapon()
{
	SkeletalMesh1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh1"));
	SkeletalMesh1->SetupAttachment(GetRootComponent());

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetRootComponent());

	bWeaponParticules = false;
	WeaponState = EWeaponState::EWS_Pickup;

	Damage = 25.f;
	bWeaponOverlaing = false;

}

void AWeapon::BeginPlay()
{

	Super::BeginPlay();

	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::CombatOnOverlapEnd);


	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	//CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
//	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);



}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("weapon_begin3"));

	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if((WeaponState==EWeaponState::EWS_Pickup) || OtherActor)
	{
		ASchar * Schar = Cast<ASchar>(OtherActor);
		if (Schar )
		{
			Schar->SetActiveOverlapingItem(this);

		}
	}

}


void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT("weapon_end3"));
	if (OtherActor)
	{

		ASchar* Schar = Cast<ASchar>(OtherActor);
		if (Schar)
		{
			Schar->SetActiveOverlapingItem(nullptr);

		}
	}

}

void AWeapon::Equip(ASchar* Char)
{
	if (Char)
	{
		SetInstigator(Char->GetController());

		SkeletalMesh1->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);//make the camera of character not affected by the sword movement
		SkeletalMesh1->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);//
	

		SkeletalMesh1->SetSimulatePhysics(false);//stop the physic of the weapon

		const USkeletalMeshSocket* RighHandSocket = Char->GetMesh()->GetSocketByName("Right_Hand");//call the socket created in the class and attach it to the socket in the skeleton

		if (RighHandSocket)//attach the weapon to the right hand socket
		{
			RighHandSocket->AttachActor(this, Char->GetMesh());
			bRotate = false;
			Char->SetEquippedWeapon(this);
	
		}
				Char->SetActiveOverlapingItem(nullptr);

		if (OnEquipSound)//sound generated on equip after pressing f
		{
			UGameplayStatics::PlaySound2D(this,OnEquipSound);
		}

		if (!bWeaponParticules)// particule in idle , in teh ground
		{
			IdleParticleComponent->Deactivate();
		}

		
	}

}

void AWeapon::CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	

 
		 if (OtherActor && bWeaponOverlaing == false)
		{
			AEnemy* Enemy = Cast<AEnemy>(OtherActor);
			if (Enemy)
			{
				if (Enemy->HitParticle_Enemy)
				{
					const USkeletalMeshSocket* WeaponSocket = SkeletalMesh1->GetSocketByName("WeaponSocket");
					if (WeaponSocket)
					{

						FVector SocketLocation = WeaponSocket->GetSocketLocation(SkeletalMesh1);
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Enemy->HitParticle_Enemy, GetActorLocation(), FRotator(0.f), true);
					}
					bWeaponOverlaing = false;
				}
				if (Enemy->HitSound_Enemy)
				{
					UGameplayStatics::PlaySound2D(this, Enemy->HitSound_Enemy);
				}
				UE_LOG(LogTemp, Warning, TEXT("Hit"));
				if (DamageTypeClass)
				{
					UGameplayStatics::ApplyDamage(Enemy, Damage, WeaponInstigator, this, DamageTypeClass);
				}
			}
		}


	}


void AWeapon::CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && bWeaponOverlaing == true) 
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy)
		{
			if (bWeaponOverlaing == true)
			{
				bWeaponOverlaing = false;
			}
		}
	}
	

}

void AWeapon::ActivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}

void AWeapon::DeactivateCollision()
{
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeapon::PlaySwingSoundSword()
{
	UGameplayStatics::PlaySound2D(this, SwingSound);
}

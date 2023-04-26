// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"
#include "SChar.h"
#include "Components/SphereComponent.h"
#include "Schar.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"




AExplosive::AExplosive()
{

	Damage = 15.f;


}

void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UE_LOG(LogTemp, Warning, TEXT("begin3"));

		if (OtherActor && OtherActor != this && OtherComp && Cast<ASchar>(OtherActor))
		{
			ASchar* SChar = Cast<ASchar>(OtherActor);
			if (SChar)
			{
				SChar->DecrementHealth(Damage);
				if (OverlapParticle)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticle, GetActorLocation(), FRotator(0.f), true);

				}
				if (OverlapSound)
				{

					UGameplayStatics::PlaySound2D(this, OverlapSound);
				}

				Destroy();

			}
		}
	

}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT("end3"))

}

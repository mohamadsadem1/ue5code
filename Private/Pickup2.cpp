// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup2.h"
#include "SChar.h"


APickup2::APickup2()
{
	CoinCount = 1.f;


}

void APickup2::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	UE_LOG(LogTemp, Warning, TEXT("begin2"));


		if (OtherActor)
		{
			ASchar* SChar = Cast<ASchar>(OtherActor);
			if (SChar)
			{
				SChar->IncrementCoin(CoinCount);

			}
		}
}




void APickup2::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT("end2"))

}
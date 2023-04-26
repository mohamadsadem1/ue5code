// Fill out your copyright notice in the Description page of Project Settings.


#include "collect.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Schar.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"
// Sets default values
Acollect::Acollect()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = Collision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());
	IdleParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticleComponent"));
	IdleParticleComponent->SetupAttachment(GetRootComponent());

	bRotate = false;

}

// Called when the game starts or when spawned
void Acollect::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &Acollect::OnOverlapBegin);
	Collision->OnComponentEndOverlap.AddDynamic(this, &Acollect::OnOverlapEnd);

	Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Collision->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);


}

// Called every frame
void Acollect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator Rotation = GetActorRotation();
	Rotation.Yaw += DeltaTime * RotationRate;
	SetActorRotation(Rotation);

}

// Called to bind functionality to input
void Acollect::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void Acollect::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	
}


void Acollect::OnOverlapEnd( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Collect_end1"))



}




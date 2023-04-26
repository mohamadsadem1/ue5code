// Fill out your copyright notice in the Description page of Project Settings.

#include "FloorSwitch.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AFloorSwitch::AFloorSwitch()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//create a box space to start the event
	 TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	 RootComponent = TriggerBox;

	 //chnage collision override to bp for a class
	 //first make the sollsion enable
	 TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	 //set the collision of the object typer to world static
	 TriggerBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);

	//make all the collsiion ignore 
	 TriggerBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	//define one of the collsion to change to be a specific type of collision to interact
	 TriggerBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	 TriggerBox->SetBoxExtent(FVector(62.f,62.f,32.f));

	 //create a meshcomponent for the floorswitch to attach a custom mesh using bp
	 FloorSwitch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FloorSwitch"));
	 FloorSwitch->SetupAttachment(GetRootComponent());

	 //create a meshcomponent for the door to attach a custom mesh using bp
	 Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	 Door->SetupAttachment(GetRootComponent());
	 
	 SwicthTime = 2.f;
	 bCharacterOnSwitch = false;

}

// Called when the game starts or when spawned
void AFloorSwitch::BeginPlay()
{
	Super::BeginPlay();
	//OnComponentBeginOverlap is a functio that is colled when the character hit the trigger using the collision of teh character
//this add a dynamic and the adddynamic take 2 params(this,and the location of a  function)
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AFloorSwitch::OnOverlapEnd);
	InitialDoorLoaction = Door->GetComponentLocation();
	InitialSwitchLoaction = FloorSwitch->GetComponentLocation();


}

// Called every frame
void AFloorSwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFloorSwitch::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AFloorSwitch::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OverlapBegin"));
	if(!bCharacterOnSwitch)	bCharacterOnSwitch = true;
	RaiseDoor();
	LowerFloorSwitch();


}

void AFloorSwitch::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapend"));
	if (bCharacterOnSwitch) bCharacterOnSwitch = false;
	GetWorldTimerManager().SetTimer(SwitchHandle, this, &AFloorSwitch::CloseDoor, SwicthTime);
}

void AFloorSwitch::UpdateDoorLocation(float z)
{
	FVector NewLocation = InitialDoorLoaction;
	NewLocation.Z += z;
	Door->SetWorldLocation(NewLocation);
}

void AFloorSwitch::UpdateSwitchFloorLocation(float z)
{
	FVector NewLocation = InitialSwitchLoaction;
	NewLocation.Z += z;
	FloorSwitch->SetWorldLocation(NewLocation);
}

void AFloorSwitch::CloseDoor()
{
	if (!bCharacterOnSwitch) {
	LowerDoor();
	RaiseFloorSwitch();
}

}






#include "SpawVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "collect.h"
#include "Enemy.h"
#include "TimerManager.h"


// Sets default values
ASpawVolume::ASpawVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));


	SpawnDelayMin = 1.0f;

	SpawnDelayMax = 5.0f;

	/*
			ENEMIES COUNTERS
	*/
	MaxEnemiesAlive = 5;

	Number_SpawnedEnemies = 0;

	Number_AliveEnemies = 0;

	MaxEnemies = 10;

	bCharacterOverlapingWithSpawn = false;

}

// Called when the game starts or when spawned
void ASpawVolume::BeginPlay()
{
	Super::BeginPlay();

	SpawningBox->OnComponentBeginOverlap.AddDynamic(this, &ASpawVolume::HandleOverlapBegin);
	SpawningBox->OnComponentEndOverlap.AddDynamic(this, &ASpawVolume::HandleOverlapEnd);

	if (Spell_1 && Spell_2 && Spell_3 && Spell_4)
	{
		spellSpawnArray.Add(Spell_1);
		spellSpawnArray.Add(Spell_2);
		spellSpawnArray.Add(Spell_3);
		spellSpawnArray.Add(Spell_4);
	}
	if (Actor_1 && Actor_2 && Actor_3 && Actor_4)
	{
		EnemiesSpawnArray.Add(Actor_1);
		EnemiesSpawnArray.Add(Actor_2);
		EnemiesSpawnArray.Add(Actor_3);
		EnemiesSpawnArray.Add(Actor_4);

	}
}

// Called every frame
void ASpawVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawVolume::GetSpawnPoint()
{
	FVector Extent = SpawningBox->GetScaledBoxExtent();
	FVector Origin = SpawningBox->GetComponentLocation();
	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	return Point;
}


void ASpawVolume::SpawnOurPawn_Implementation(UClass* ToSpawn, const FVector& Location)
{
	if (ToSpawn)
	{
		UWorld* World = GetWorld();

		FActorSpawnParameters SpawnParams;

		if (World)
		{
			if (Number_SpawnedEnemies < MaxEnemies && Number_AliveEnemies < MaxEnemiesAlive)
			{
				APawn* PawnSpawned = World->SpawnActor<APawn>(ToSpawn, Location, FRotator::ZeroRotator, SpawnParams);

				if (PawnSpawned)
				{
					// Add spawned enemy to array and increment counters
					Number_SpawnedEnemies++;
					Number_AliveEnemies++;

					// Set timer to spawn next enemy
					float SpawnDelay = FMath::FRandRange(SpawnDelayMin, SpawnDelayMax);

					GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawVolume::SpawnOurPawn_Implementation, SpawnDelay);
					FString EnemyName = PawnSpawned->GetName();
					EnemiesNamesArray.Add(EnemyName);

				}
			}
		}
	}
}

void ASpawVolume::SpawnOurSpell(UClass * ToSpawn, const FVector & Location)
{
	UWorld* World = GetWorld();

	if (World)
	{
			FActorSpawnParameters SpawnParams;
			class ACollect* SpellToSpawn = World->SpawnActor<ACollect>(ToSpawn, Location, FRotator::ZeroRotator, SpawnParams);


	}
}


	void ASpawVolume::EnemyDeath(AEnemy* DeadEnemy)
	{
		if (DeadEnemy)
		{
			// Get name of dead enemy to remove it from the array
			FString DeadEnemyName = DeadEnemy->GetName();

			// Check if dead enemy is in the array
			if (EnemiesNamesArray.Contains(DeadEnemyName))
			{
				// Remove dead enemy from array and decrement counters
				EnemiesSpawnArray.Remove(DeadEnemy);
				Number_AliveEnemies--;
			}
			FVector LastLocation = DeadEnemy->GetActorLocation();

			SpawnOurSpell(SpellClass, LastLocation);
		}
	}


	/*
				TO HANDLE IF THE CHARACTER IS OVERLAPING WITH THE SPAWN VOLUME
	*/
	void ASpawVolume::HandleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
	{

		if (OtherActor)
		{
			bCharacterOverlapingWithSpawn = true;
			AEnemy* Enemies = Cast<AEnemy>(OtherActor);
			if (OtherActor == Enemies)
			{
				if (Number_AliveEnemies < MaxEnemiesAlive && Number_SpawnedEnemies < MaxEnemies)
				{
					SpawnOurPawn_Implementation(, GetSpawnPoint());
				}
			}


			if (Enemies->bDead == true)
			{
				if (Enemies)
				{
					EnemyDeath(Enemies);
				}
			}
		}
	}



void ASpawVolume::HandleOverlapEnd(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{

		bCharacterOverlapingWithSpawn = false;
	}
}

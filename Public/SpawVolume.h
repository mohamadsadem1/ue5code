#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawVolume.generated.h"

UCLASS()
class MYPROJECT9_API ASpawVolume : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawVolume();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
		class UBoxComponent* SpawningBox;
	/*
				ENEMY TYPES TO SPAWN IN _SPAWNVOLUME_
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<class AEnemy> Actor_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<AEnemy> Actor_2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<AEnemy> Actor_3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<AEnemy> Actor_4;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<AEnemy> EnemyClass;

	TArray<TSubclassOf<AEnemy>>EnemiesSpawnArray;

	/*
				TYPE OF SPELL TO SPAWN WHEN ENEMY DIE
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<class AEnemy> Spell_1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<AEnemy> Spell_2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<AEnemy> Spell_3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
		TSubclassOf<AEnemy> Spell_4;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<AEnemy> SpellClass;

	TArray<TSubclassOf<AActor>>spellSpawnArray;

	TArray<FString>EnemiesNamesArray;

	/*
				LOGIC OF SPAWNING ENEMIES
	*/

	bool bCharacterOverlapingWithSpawn;

	int32 Number_SpawnedEnemies;

	int32 Number_AliveEnemies;

	int32 MaxEnemies;

	int32 MaxEnemiesAlive;

	/*
			TIMER TO SPAWN ENEMIES
	*/
	float SpawnDelayMin;

	float SpawnDelayMax;

	FTimerHandle SpawnTimer;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Spawning")
		void SpawnEnemy();

	virtual void SpawnEnemy_Implementation();


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "Spawning")
		TSubclassOf<class AActor> GetSpawnActor();

	UFUNCTION(BlueprintPure, Category = "Spawning")
		FVector GetSpawnPoint();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Spawning")
		void SpawnOurPawn(UClass* ToSpawn, const FVector& Location);

	UFUNCTION()
		void SpawnOurSpell(UClass* ToSpawn, const FVector& Location);


	void SpawnNextEnemy(float Delay);

	FVector GetRandomSpawnLocation() const;

	void EnemyDeath(AEnemy* DeadEnemy);

	UFUNCTION()
		void ActorDestroy(AActor* Actor);


	/*
			OVERLAPING LOGIC
	*/
	UFUNCTION()
		virtual void HandleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void HandleOverlapEnd(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);





};
// Copyright ChairNation, 2019

#pragma once

#include "GameFramework/Actor.h"
#include "Public/Enemy.h"
#include "DestructibleActor.h"
#include "EnemySpawner.generated.h"

UCLASS(HideCategories=("Rendering", "Input", "Actor", "LOD", "Cooking"))
class HOVERCRAFTGANGWARS_API AEnemySpawner final : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();

	UFUNCTION()
		void StopSpawning(const FVector& HitPoint, const FVector& HitDirection);

protected:
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;
	bool ShouldTickIfViewportsOnly() const override;

	void StartSpawner();

	UFUNCTION()
		void SpawnEnemy();

	UPROPERTY()
		USceneComponent* SceneComponent;

	UPROPERTY()
		class UBillboardComponent* BillboardComponent;

	UPROPERTY(EditInstanceOnly)
		ADestructibleActor* DestructibleActor;

	UPROPERTY(EditInstanceOnly)
		TSubclassOf<AEnemy> EnemyType;

	UPROPERTY(EditInstanceOnly)
		float SpawnRadius = 100.0f;

	UPROPERTY(EditInstanceOnly)
		float SpawnDelay = 1.0f;

	UPROPERTY(EditInstanceOnly)
		FVector SpawnOffset;

	UPROPERTY(EditInstanceOnly)
		int32 EnemySpeed = 100;

	UPROPERTY(EditInstanceOnly)
		float EnemyFireRate = 0.1f;

	UPROPERTY(EditInstanceOnly)
		int32 EnemyBulletSpeed = 100;

	UPROPERTY(EditInstanceOnly)
		int32 EnemyBulletDamage = 10;

	UPROPERTY(VisibleAnywhere, Category = "Debug")
		int32 EnemyCount;

private:
	UTexture2D* BillboardSprite;

	FTimerHandle Timer;
};

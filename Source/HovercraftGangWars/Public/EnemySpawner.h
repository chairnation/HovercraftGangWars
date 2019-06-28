// Copyright ChairNation, 2019

#pragma once

#include "GameFramework/Actor.h"
#include "Public/Enemy.h"
#include "EnemySpawner.generated.h"

UCLASS(HideCategories=("Rendering", "Input", "Actor", "LOD", "Cooking"))
class HOVERCRAFTGANGWARS_API AEnemySpawner final : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();

	void StopSpawning();

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
		TSubclassOf<AEnemy> EnemyType;

	UPROPERTY(EditInstanceOnly)
		float SpawnRadius = 100.0f;

	UPROPERTY(EditInstanceOnly)
		float SpawnDelay = 1.0f;

	UPROPERTY(EditInstanceOnly)
		FVector SpawnOffset;

	UPROPERTY(EditInstanceOnly)
		int32 EnemySpeed = 100;

	UPROPERTY(VisibleAnywhere, Category = "Debug")
		int32 EnemyCount;

private:
	UTexture2D* BillboardSprite;

	FTimerHandle Timer;
};

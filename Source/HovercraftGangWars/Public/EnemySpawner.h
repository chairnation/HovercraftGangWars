// Fill out your copyright notice in the Description page of Project Settings.

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

protected:
	void BeginPlay() override;
	void PostEditMove(bool bFinished) override;

	void StartSpawner();

	UFUNCTION()
		void SpawnEnemy();

	UPROPERTY()
		USceneComponent* SceneComponent;

	UPROPERTY()
		UBillboardComponent* BillboardComponent;

	UPROPERTY(EditInstanceOnly)
		TSubclassOf<AEnemy> EnemyType;

	UPROPERTY(EditInstanceOnly)
		float SpawnDelay = 1.0f;

	UPROPERTY(VisibleAnywhere)
		int32 EnemyCount;

private:
	UTexture2D* BillboardSprite;

	FTimerHandle Timer;
};

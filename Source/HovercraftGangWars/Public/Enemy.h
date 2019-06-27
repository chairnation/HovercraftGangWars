// Copyright ChairNation, 2019

#pragma once

#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS(HideCategories = ("Rendering", "Input", "Actor", "LOD", "Cooking"))
class HOVERCRAFTGANGWARS_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
		USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditInstanceOnly, Category = "Enemy", meta = (ClampMin = 1, ClampMax = 10000))
		int32 Health = 100;

	UStaticMesh* StaticMesh;
	UMaterialInterface* Material;
};

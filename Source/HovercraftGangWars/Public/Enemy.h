// Copyright ChairNation, 2019

#pragma once

#include "GameFramework/Pawn.h"
#include "Enemy.generated.h"

UCLASS(HideCategories = ("Rendering", "Input", "Actor", "LOD", "Cooking"))
class HOVERCRAFTGANGWARS_API AEnemy : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

	void SetSpeed(float InSpeed);

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void ApplyHitMaterial();
	void ResetMaterialAfter(float Seconds);

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	void ApplyDefaultMaterial();

	AActor* Player;

	UPROPERTY(EditDefaultsOnly)
		USceneComponent* SceneComponent;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly)
		class UFloatingPawnMovement* PawnMovementComponent;

	UPROPERTY(VisibleAnywhere, Category = "Enemy", meta = (ClampMin = 1, ClampMax = 10000))
		int32 Health = 100;

	UPROPERTY(VisibleAnywhere, Category = "Enemy", meta = (ClampMin = 1, ClampMax = 10000))
		int32 Speed = 100;

	UStaticMesh* StaticMesh;
	UMaterialInterface* Material;
	UMaterialInterface* HitMaterial;

	FTimerHandle TimerHandle;
};

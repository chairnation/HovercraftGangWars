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
	void SetBulletSpeed(int32 InSpeed);
	void SetFirerate(float InFirerate);

	float ApplyDamage(int32 DamageAmount);

	void ApplyHitMaterial() const;
	void ResetMaterialAfter(float Seconds);

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	void ApplyDefaultMaterial();

	void FireShot(FVector FireDirection);

	void ShotTimerExpired();

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

	UPROPERTY(VisibleAnywhere, Category = "Enemy", meta = (ClampMin = 1, ClampMax = 10000))
		int32 BulletSpeed = 2000;

	UPROPERTY(VisibleAnywhere, Category = "Enemy", meta = (ClampMin = 1, ClampMax = 10000))
		int32 DamagePerShot = 10;

	UPROPERTY(VisibleAnywhere, Category = "Enemy", meta = (ClampMin = 1, ClampMax = 10000))
		float FireRate = 0.1;

	UPROPERTY(VisibleAnywhere, Category = "Enemy", meta = (ClampMin = 1, ClampMax = 10000))
		FVector GunOffset = FVector(150.0f, 0.0f, 0.0f);

	UStaticMesh* StaticMesh;
	UMaterialInterface* Material;
	UMaterialInterface* HitMaterial;

	FTimerHandle TimerHandle;
	FTimerHandle Timer_ShotExpiry;

	bool bCanFire;
};

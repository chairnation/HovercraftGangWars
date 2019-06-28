// Copyright ChairNation, 2019

#pragma once

#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bullet.generated.h"

UCLASS()
class HOVERCRAFTGANGWARS_API ABullet : public AActor
{
	GENERATED_BODY()
	

protected:
	/** Sphere collision component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMeshComponent;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovement;

	UStaticMesh* ProjectileMesh;
	UMaterialInterface* Material;

public:
	ABullet();

	/** Function to handle the projectile hitting something */
	UFUNCTION()
		virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns ProjectileMesh subobject **/
	FORCEINLINE UStaticMeshComponent* GetProjectileMesh() const { return ProjectileMeshComponent; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = "true"))
		float Damage;

};

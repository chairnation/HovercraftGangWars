// Copyright ChairNation, 2019

#include "Bullet.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Enemy.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	// Static mesh component
	ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	RootComponent = ProjectileMeshComponent;
	ProjectileMeshComponent->SetRelativeLocation(FVector(0.0f));
	ProjectileMeshComponent->BodyInstance.SetCollisionProfileName("Projectile");
	ProjectileMeshComponent->OnComponentHit.AddDynamic(this, &ABullet::OnHit);		// set up a notification for when this component hits something

	ProjectileMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/TwinStickCPP/EdsFolder/M_Bullet.M_Bullet'")));

	if (ProjectileMesh)
	{
		ProjectileMeshComponent->SetStaticMesh(ProjectileMesh);
		ProjectileMeshComponent->SetMaterial(0, Material);
	}

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement0"));
	ProjectileMovement->UpdatedComponent = ProjectileMeshComponent;
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f; // No gravity

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (OtherActor->IsA(AEnemy::StaticClass()))
		{
			AEnemy* Enemy = Cast<AEnemy>(OtherActor);
			Enemy->ApplyDamage(Damage);
			OtherComp->AddImpulseAtLocation(GetVelocity() * 20.0f, GetActorLocation());
			Enemy->ApplyHitMaterial();
			Enemy->ResetMaterialAfter(0.05f);
		}

		Destroy();
	}
}


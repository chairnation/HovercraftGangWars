// Copyright ChairNation, 2019

#include "PlayerBullet.h"
#include "Materials/MaterialInterface.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy.h"
#include "EnemyBullet.h"

APlayerBullet::APlayerBullet()
{
	Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("MaterialInstanceConstant'/Game/TwinStickCPP/EdsFolder/m_PlayerBullet.m_PlayerBullet'")));

	ProjectileMeshComponent->SetMaterial(0, Material);
}

void APlayerBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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

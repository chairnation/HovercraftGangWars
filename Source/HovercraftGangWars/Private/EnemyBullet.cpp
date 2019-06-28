// Copyright ChairNation, 2019

#include "EnemyBullet.h"
#include "Materials/MaterialInterface.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerBullet.h"


AEnemyBullet::AEnemyBullet()
{
	Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("MaterialInstanceConstant'/Game/TwinStickCPP/EdsFolder/m_EnemyBullet.m_EnemyBullet'")));

	ProjectileMeshComponent->SetMaterial(0, Material);
}

void AEnemyBullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		Destroy();
	}
}

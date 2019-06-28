// Copyright ChairNation, 2019

#include "PlayerBullet.h"
#include "Materials/MaterialInterface.h"
#include "Components/StaticMeshComponent.h"

APlayerBullet::APlayerBullet()
{
	Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("MaterialInstanceConstant'/Game/TwinStickCPP/EdsFolder/m_PlayerBullet.m_PlayerBullet'")));

	ProjectileMeshComponent->SetMaterial(0, Material);
}

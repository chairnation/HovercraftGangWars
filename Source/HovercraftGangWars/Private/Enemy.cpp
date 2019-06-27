// Copyright ChairNation, 2019

#include "Public/Enemy.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Scene component
	SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("Scene Component"));
	SceneComponent = RootComponent;

	// Static mesh component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	StaticMeshComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'")));

	if (StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(StaticMesh);
	}

	StaticMeshComponent->SetWorldLocation(GetActorLocation());
	StaticMeshComponent->SetWorldScale3D(FVector(0.5f, 0.5f, 1.0f));
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComponent->SetWorldLocation(GetActorLocation());
}

void AEnemy::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::PostEditMove(const bool bFinished)
{
	Super::PostEditMove(bFinished);

	//StaticMeshComponent->SetRelativeLocation(FVector(0.0f));
}


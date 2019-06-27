// Copyright ChairNation, 2019

#include "Public/Enemy.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Scene component
	SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("Scene Component"));
	RootComponent = SceneComponent;

	// Static mesh component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'")));

	if (StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(StaticMesh);
	}

	StaticMeshComponent->SetRelativeLocation(FVector(0.0f));
	StaticMeshComponent->SetWorldScale3D(FVector(0.5f, 0.5f, 1.0f));
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetMassOverrideInKg(NAME_None, 200.0f);
	StaticMeshComponent->SetCollisionProfileName(FName("PhysicsActor"));

	bCanBeDamaged = true;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

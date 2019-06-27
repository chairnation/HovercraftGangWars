// Copyright ChairNation, 2019

#include "Public/Enemy.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/FloatingPawnMovement.h"


AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Scene component
	SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("Scene Component"));
	RootComponent = SceneComponent;

	// Static mesh component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("MaterialInstanceConstant'/Game/TwinStick/Meshes/OrangeMaterial.OrangeMaterial'")));
	HitMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/TwinStick/Meshes/BaseMaterial.BaseMaterial'")));
	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'")));

	if (StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetMaterial(0, Material);
	}

	StaticMeshComponent->SetRelativeLocation(FVector(0.0f));
	StaticMeshComponent->SetWorldScale3D(FVector(0.5f, 0.5f, 1.0f));
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionProfileName(FName("PhysicsActor"));

	// Movement component
	PawnMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(FName("Movement Component"));
	PawnMovementComponent->UpdatedComponent = RootComponent;

	bCanBeDamaged = true;
}

void AEnemy::SetSpeed(const float InSpeed)
{
	Speed = InSpeed;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	StaticMeshComponent->SetMassOverrideInKg(NAME_None, 200.0f);
}

void AEnemy::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Direction = Player->GetActorLocation() - GetActorLocation();
	Direction.Normalize();

	AddMovementInput(Direction);
}

float AEnemy::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health -= DamageAmount;

	if (Health <= 0)
	{
		Destroy();
	}

	return DamageAmount;
}

void AEnemy::ApplyHitMaterial() const
{
	StaticMeshComponent->SetMaterial(0, HitMaterial);
}

void AEnemy::ResetMaterialAfter(const float Seconds)
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnemy::ApplyDefaultMaterial, 1.0f, false, Seconds);
}

void AEnemy::ApplyDefaultMaterial()
{
	StaticMeshComponent->SetMaterial(0, Material);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

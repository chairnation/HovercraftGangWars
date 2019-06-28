// Copyright ChairNation, 2019

#include "Public/Enemy.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Static mesh component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetCollisionProfileName(FName("Pawn"));

	Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("MaterialInstanceConstant'/Game/TwinStick/Meshes/OrangeMaterial.OrangeMaterial'")));
	HitMaterial = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/TwinStick/Meshes/BaseMaterial.BaseMaterial'")));
	StaticMesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, TEXT("StaticMesh'/Game/TwinStickCPP/BradsFolder/Gangster.Gangster'")));

	if (StaticMesh)
	{
		StaticMeshComponent->SetStaticMesh(StaticMesh);
		StaticMeshComponent->SetMaterial(0, Material);
	}

	StaticMeshComponent->SetRelativeLocation(FVector(0.0f));
	StaticMeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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
	
	const FVector Movement = Direction * Speed * DeltaTime;

	const FRotator NewRotation = Direction.Rotation() + FRotator(0.0f, -90.0f, 0.0f);
	SetActorRotation(NewRotation);

	if (Movement.SizeSquared() > 0.0f)
	{		
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);

		if (Hit.bBlockingHit)
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);

			StaticMeshComponent->SetSimulatePhysics(true);
			StaticMeshComponent->AddImpulseAtLocation(GetVelocity() * 20.0f, GetActorLocation());
		}
	}
}

float AEnemy::ApplyDamage(const int32 DamageAmount)
{
	Health -= DamageAmount;

	StaticMeshComponent->SetSimulatePhysics(true);

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

// Copyright ChairNation, 2019

#include "Public/Enemy.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "HovercraftGangWarsPawn.h"
#include "EnemyBullet.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// Static mesh component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(FName("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	StaticMeshComponent->SetCollisionProfileName(FName("Pawn"));

	Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/Game/TwinStickCPP/BradsFolder/Material/M_Gangster_body.M_Gangster_body'")));
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
	bCanFire = true;
}

void AEnemy::SetSpeed(const float InSpeed)
{
	Speed = InSpeed;
}

void AEnemy::SetBulletSpeed(const int32 InSpeed)
{
	BulletSpeed = InSpeed;
}

void AEnemy::SetFirerate(float InFirerate)
{
	FireRate = InFirerate;
}

void AEnemy::SetDamage(int32 InDamage)
{
	DamagePerShot = InDamage;
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

			if (Hit.GetActor() && Hit.GetActor()->IsA(AHovercraftGangWarsPawn::StaticClass()))
			{
				StaticMeshComponent->SetSimulatePhysics(true);
				StaticMeshComponent->AddImpulseAtLocation(GetVelocity() * 20.0f, GetActorLocation());
			}
		}
	}

	FireShot(Direction);
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

void AEnemy::FireShot(const FVector FireDirection)
{
	if (bCanFire)
	{
		const FRotator FireRotation = FireDirection.Rotation();
		// Spawn projectile at an offset from this pawn
		const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

		UWorld* const World = GetWorld();
		if (World)
		{
			// spawn the projectile
			const auto Projectile = World->SpawnActor<AEnemyBullet>(SpawnLocation, FireRotation);
			Projectile->Damage = DamagePerShot;
			Projectile->SetSpeed(BulletSpeed);
		}

		bCanFire = false;
		World->GetTimerManager().SetTimer(Timer_ShotExpiry, this, &AEnemy::ShotTimerExpired, FireRate);
	}
}

void AEnemy::ShotTimerExpired()
{
	bCanFire = true;
}
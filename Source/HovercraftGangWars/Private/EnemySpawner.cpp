// Copyright ChairNation, 2019

#include "Public/EnemySpawner.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Texture2D.h"
#include "DrawDebugHelpers.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	// Scene Component
	SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("Scene Component"));
	RootComponent = SceneComponent;

	// Billboard Component
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(FName("Billboard Component"));
	BillboardComponent->SetupAttachment(SceneComponent);
	BillboardSprite = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("Texture2D'/Engine/EditorResources/EmptyActor.EmptyActor'")));
	BillboardComponent->SetSprite(BillboardSprite);
	BillboardComponent->SetRelativeLocation(FVector(0.0f));
}

void AEnemySpawner::StopSpawning()
{
	GetWorld()->GetTimerManager().ClearTimer(Timer);
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	StartSpawner();
}

void AEnemySpawner::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	DrawDebugCircle(GetWorld(), GetActorLocation(), SpawnRadius, 80, FColor::Red, false, -1, 0, 1, FVector::ForwardVector, FVector::RightVector);
}

bool AEnemySpawner::ShouldTickIfViewportsOnly() const
{
	return true;
}

void AEnemySpawner::StartSpawner()
{
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AEnemySpawner::SpawnEnemy, SpawnDelay, true);
}

void AEnemySpawner::SpawnEnemy()
{
	// Lambda function, to generate a random point on sphere (this is used for spawning boids on the generated point)
	const auto GetRandomPointOnCircle = [&](const FVector Center, const float Radius)
	{
		const float Angle = FMath::FRandRange(0.0f, 2*PI);

		const float X = Center.X + Radius * FMath::Cos(Angle);
		const float Y = Center.Y + Radius * FMath::Sin(Angle);

		return FVector(X, Y, GetActorLocation().Z);
	};

	auto Enemy = GetWorld()->SpawnActor<AEnemy>(EnemyType, GetRandomPointOnCircle(GetActorLocation(), SpawnRadius) + SpawnOffset, GetActorRotation());

	if (Enemy)
	{
		Enemy->SpawnDefaultController();
		Enemy->SetSpeed(EnemySpeed);
		Enemy->SetBulletSpeed(EnemyBulletSpeed);
		Enemy->SetFirerate(EnemyFireRate);
	}

	EnemyCount++;
}
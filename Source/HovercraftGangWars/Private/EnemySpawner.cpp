// Copyright ChairNation, 2019

#include "Public/EnemySpawner.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Texture2D.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	// Scene Component
	SceneComponent = CreateDefaultSubobject<USceneComponent>(FName("Scene Component"));
	SceneComponent = RootComponent;

	// Billboard Component
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>(FName("Billboard Component"));
	BillboardComponent->SetupAttachment(SceneComponent);
	BillboardSprite = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, TEXT("Texture2D'/Engine/EditorResources/EmptyActor.EmptyActor'")));
	BillboardComponent->SetSprite(BillboardSprite);
	BillboardComponent->SetRelativeLocation(FVector(0.0f));
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	StartSpawner();
}

void AEnemySpawner::PostEditMove(const bool bFinished)
{
	Super::PostEditMove(bFinished);

	BillboardComponent->SetRelativeLocation(FVector(0.0f));
}

void AEnemySpawner::StartSpawner()
{
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AEnemySpawner::SpawnEnemy, SpawnDelay, true);
}

void AEnemySpawner::SpawnEnemy()
{
	GetWorld()->SpawnActor<AActor>(EnemyType, GetActorLocation(), GetActorRotation());
	EnemyCount++;
}
// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "HovercraftGangWarsPawn.h"
#include "HovercraftGangWarsProjectile.h"
#include "TimerManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/StaticMesh.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "PlayerBullet.h"

const FName AHovercraftGangWarsPawn::MoveForwardBinding("MoveForward");
const FName AHovercraftGangWarsPawn::MoveRightBinding("MoveRight");
const FName AHovercraftGangWarsPawn::FireForwardBinding("FireForward");
const FName AHovercraftGangWarsPawn::FireRightBinding("FireRight");

AHovercraftGangWarsPawn::AHovercraftGangWarsPawn()
{	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ShipMesh(TEXT("StaticMesh'/Game/TwinStickCPP/HowardsFolder/HoverCraft/HoverCraftWhole.HoverCraftWhole'"));
	// Create the mesh component
	ShipMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	RootComponent = ShipMeshComponent;
	ShipMeshComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	ShipMeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	ShipMeshComponent->SetStaticMesh(ShipMesh.Object);
	
	// Cache our sound effect
	static ConstructorHelpers::FObjectFinder<USoundBase> FireAudio(TEXT("/Game/TwinStick/Audio/TwinStickFire.TwinStickFire"));
	FireSound = FireAudio.Object;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when ship does
	CameraBoom->TargetArmLength = 2200.f;
	CameraBoom->RelativeRotation = FRotator(-80.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;	// Camera does not rotate relative to arm

	// Movement
	MoveSpeed = 1000.0f;
	// Weapon
	GunOffset = FVector(90.f, 0.f, 0.f);
	BulletSpeed = 2000.0f;
	FireRate = 0.1f;
	bCanFire = true;
}

void AHovercraftGangWarsPawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	// set up gameplay key bindings
	PlayerInputComponent->BindAxis(MoveForwardBinding);
	PlayerInputComponent->BindAxis(MoveRightBinding);
	PlayerInputComponent->BindAxis(FireForwardBinding);
	PlayerInputComponent->BindAxis(FireRightBinding);
}

void AHovercraftGangWarsPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->bShowMouseCursor = true;
}

void AHovercraftGangWarsPawn::Tick(const float DeltaSeconds)
{
	// Find movement direction
	const float ForwardValue = GetInputAxisValue(MoveForwardBinding);
	const float RightValue = GetInputAxisValue(MoveRightBinding);

	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(ForwardValue, RightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate look direction
	FVector WorldLocation;
	FVector WorldDirection;
	PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);

	const FVector LookDirection = WorldDirection * FVector(1.0f, 1.0f, 0.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * MoveSpeed * DeltaSeconds;

	const FRotator NewRotation = LookDirection.Rotation() + FRotator(0.0f, -90.0f, 0.0f);
	SetActorRotation(NewRotation);

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, NewRotation, true, &Hit);
		
		if (Hit.IsValidBlockingHit())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, NewRotation, true);
		}
	}
	
	// Create fire direction vector
	const FVector FireDirection = FVector(LookDirection.X, LookDirection.Y, 0.f);

	// Try and fire a shot
	FireShot(FireDirection);
}

void AHovercraftGangWarsPawn::FireShot(const FVector FireDirection)
{
	// If it's ok to fire again
	if (bCanFire && PlayerController->IsInputKeyDown(EKeys::LeftMouseButton) || PlayerController->IsInputKeyDown(EKeys::Gamepad_RightTrigger))
	{
		// If we are pressing fire stick in a direction
		if (FireDirection.SizeSquared() > 0.0f)
		{
			const FRotator FireRotation = FireDirection.Rotation();
			// Spawn projectile at an offset from this pawn
			const FVector SpawnLocation = GetActorLocation() + FireRotation.RotateVector(GunOffset);

			UWorld* const World = GetWorld();
			if (World)
			{
				// spawn the projectile
				const auto Projectile = World->SpawnActor<APlayerBullet>(SpawnLocation, FireRotation);
				Projectile->Damage = DamagePerShot;
				Projectile->SetSpeed(BulletSpeed);
			}

			bCanFire = false;
			World->GetTimerManager().SetTimer(TimerHandle_ShotTimerExpired, this, &AHovercraftGangWarsPawn::ShotTimerExpired, FireRate);

			// try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
			}

			bCanFire = false;
		}
	}
}

void AHovercraftGangWarsPawn::ShotTimerExpired()
{
	bCanFire = true;
}


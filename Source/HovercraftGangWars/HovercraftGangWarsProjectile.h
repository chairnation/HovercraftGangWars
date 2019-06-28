// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "HovercraftGangWarsProjectile.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS(config=Game)
class AHovercraftGangWarsProjectile : public ABullet
{
	GENERATED_BODY()

public:
	AHovercraftGangWarsProjectile();

};


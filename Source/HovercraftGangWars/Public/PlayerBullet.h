// Copyright ChairNation, 2019

#pragma once

#include "Bullet.h"
#include "PlayerBullet.generated.h"

/**
 * A bullet object that the player will use
 */
UCLASS()
class HOVERCRAFTGANGWARS_API APlayerBullet : public ABullet
{
	GENERATED_BODY()
	
public:
	APlayerBullet();
	
protected:
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};

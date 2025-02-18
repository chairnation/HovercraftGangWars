// Copyright ChairNation, 2019

#pragma once

#include "Bullet.h"
#include "EnemyBullet.generated.h"

/**
 * A bullet object that the enemy will use
 */
UCLASS()
class HOVERCRAFTGANGWARS_API AEnemyBullet : public ABullet
{
	GENERATED_BODY()
	
public:
	AEnemyBullet();
	
protected:
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};

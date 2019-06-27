// Copyright ChairNation, 2019

#pragma once

#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS(HideCategories = ("Rendering", "Input", "Actor", "LOD", "Cooking"))
class HOVERCRAFTGANGWARS_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly)
		USceneComponent* SceneComponent;

	UPROPERTY(EditInstanceOnly)
		UStaticMeshComponent* StaticMeshComponent;

	UStaticMesh* StaticMesh;
};

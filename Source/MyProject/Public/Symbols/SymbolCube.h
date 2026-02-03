// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SymbolCube.generated.h"

/*
* cube which will be spawned by location
*/

UCLASS()
class MYPROJECT_API ASymbolCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASymbolCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const FVector& GetSize() const { return Size; }

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Cube = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultRoot = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector Size; // will be calculated on start and used for offset;
};

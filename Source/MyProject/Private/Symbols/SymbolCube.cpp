// Fill out your copyright notice in the Description page of Project Settings.


#include "Symbols/SymbolCube.h"

// Sets default values
ASymbolCube::ASymbolCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// In Constructor
	DefaultRoot = CreateDefaultSubobject<USceneComponent>("DefaultRoot");
	SetRootComponent(DefaultRoot);
	
	Cube = CreateDefaultSubobject<UStaticMeshComponent>("Cube");
	Cube->SetupAttachment(DefaultRoot);
	Cube->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
}

// Called when the game starts or when spawned
void ASymbolCube::BeginPlay()
{
	Super::BeginPlay();
	Cube->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	if (TObjectPtr<UStaticMesh> staticMesh = Cube->GetStaticMesh())
	{
		Size = staticMesh->GetBounds().BoxExtent;
	}
}

// Called every frame
void ASymbolCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


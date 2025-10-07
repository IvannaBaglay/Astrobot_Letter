// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphabetSymbol.h"

// Sets default values
AAlphabetSymbol::AAlphabetSymbol()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

AAlphabetSymbol::~AAlphabetSymbol()
{
}

// Called when the game starts or when spawned
void AAlphabetSymbol::BeginPlay()
{
    Super::BeginPlay();
    
    FVector location = this->GetActorLocation();
}

// Called every frame
void AAlphabetSymbol::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
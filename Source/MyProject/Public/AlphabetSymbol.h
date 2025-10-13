// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMeshActor.h"
#include "AlphabetSymbol.generated.h"

UCLASS()
class MYPROJECT_API AAlphabetSymbol : public AStaticMeshActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AAlphabetSymbol();
    ~AAlphabetSymbol();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    const FVector GetMeshSize() const;
    float GetLength() const;

public:

private:
    UPROPERTY()
    int32 Symbol;
    FVector FullSize;


    float LifeTime = 0; // how long object exist 
};

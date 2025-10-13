// Fill out your copyright notice in the Description page of Project Settings.

#include "AlphabetSymbol.h"

// Sets default values
AAlphabetSymbol::AAlphabetSymbol()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    //PrimaryActorTick.bCanEverTick = true;
}

AAlphabetSymbol::~AAlphabetSymbol()
{
}

// Called when the game starts or when spawned
void AAlphabetSymbol::BeginPlay()
{

    //Super::BeginPlay();
    
    //FVector location = this->GetActorLocation();
}

// Called every frame
void AAlphabetSymbol::Tick(float DeltaTime)
{
    //Super::Tick(DeltaTime);
}


PRAGMA_DISABLE_OPTIMIZATION

const FVector AAlphabetSymbol::GetMeshSize() const
{
    UStaticMeshComponent* staticMesh = GetStaticMeshComponent();

    if (!staticMesh)
    {
        static FVector zero = FVector::ZeroVector;
        return zero;
    }

    TArray<UStaticMeshComponent*> StaticMeshComponents;
    this->GetComponents<UStaticMeshComponent>(StaticMeshComponents);

    FBox CombinedBounds(ForceInit);
    for (UStaticMeshComponent* MeshComp : StaticMeshComponents)
    {
        if (MeshComp && MeshComp->IsVisible())
        {
            // You can use a check like this to ignore invisible or non-existent meshes.
            UStaticMesh* Mesh = MeshComp->GetStaticMesh();
            if (Mesh)
            {
                // Get the local bounds of the static mesh
                FBoxSphereBounds LocalBounds = Mesh->GetBounds();
                CombinedBounds += LocalBounds.GetBox();
            }
        }
    }

    const  FVector& Size = CombinedBounds.GetSize();


    //FBoxSphereBounds bounds = staticMesh->GetStaticMesh()->GetBounds();
    //
    //const FVector& Size = staticMesh->GetStaticMesh()->GetBounds().GetBox().GetSize();
    //FVector BoxExtent = bounds.BoxExtent;
    //FVector Origin = bounds.Origin;

    return Size;
}

float AAlphabetSymbol::GetLength() const
{
    GetComponentsBoundingBox(); // ??

    TArray<UActorComponent*> scenes;

    scenes = GetComponentsByTag(USceneComponent::StaticClass(), "Point1");

    USceneComponent* Point1 = nullptr;
    USceneComponent* Point2 = nullptr;

    if (!scenes.IsEmpty())
    {
        Point1 = Cast<USceneComponent>(scenes[0]);
    }

    scenes = GetComponentsByTag(USceneComponent::StaticClass(), "Point2");

    if (!scenes.IsEmpty())
    {
        Point2 = Cast<USceneComponent>(scenes[0]);
    }

    if (!IsValid(Point1)) return 0.0f;
    if (!IsValid(Point2)) return 0.0f;
    return FMath::Abs(Point1->GetRelativeLocation().X - Point2->GetRelativeLocation().X);
}


PRAGMA_ENABLE_OPTIMIZATION
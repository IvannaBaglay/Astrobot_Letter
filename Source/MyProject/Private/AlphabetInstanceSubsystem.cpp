// Fill out your copyright notice in the Description page of Project Settings.


#include "AlphabetInstanceSubsystem.h"
#include "AlphabetSymbol.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#include "Symbols/SymbolCube.h"

// Rework to spawn symbol by single cubes 
// 

UAlphabetInstanceSubsystem::UAlphabetInstanceSubsystem()
{
}

void UAlphabetInstanceSubsystem::RemoveSymbol(AActor* symbol)
{
}

int32 UAlphabetInstanceSubsystem::GetSymbolsSize() const
{
    return int32();
}

void UAlphabetInstanceSubsystem::AddAlphabet(int32 symbol)
{
}

PRAGMA_DISABLE_OPTIMIZATION

void UAlphabetInstanceSubsystem::SpawnSentence(FString sentence, const FVector startLocation, FRotator rotation, FVector forward)
{
    FVector offset = FVector::Zero();

    for (int32 symbol : sentence)
    {
        TObjectPtr<ASymbolCube> spawnedCube = SpawmSymbolByCubes(symbol, startLocation + offset, rotation);
        if (spawnedCube)
        {
            MoveOffset(spawnedCube->GetSize(), offset, forward);
        }
        //AAlphabetSymbol* spawnedSymbol = SpawmSymbol(symbol, startLocation + offset, rotation);
        //if (spawnedSymbol)
        //{
        //    FVector symbolStaticSize = GetStaticMeshSize(spawnedSymbol);
        //    float symbolLenght = spawnedSymbol->GetLength();
        //    UE_LOG(LogTemp, Log, TEXT("symbol Lenght:%f\n"), symbolLenght);
        //    MoveOffset(FVector(symbolLenght, 0.f, 0.f), offset, forward);
        //}
    }
}

void UAlphabetInstanceSubsystem::MoveOffset(FVector symbolSize, FVector& offset, FVector forward)
{
    forward.Normalize();

    //float symbolWidth = GetProjectedWidth(Symbol, forward);
    UE_LOG(LogTemp, Log, TEXT("symbol X:%f Y:%f, Z:%f\n"), symbolSize.X, symbolSize.Y, symbolSize.Z);
    
    float spacing = symbolSize.X + 10.f; // padding
    offset += forward * spacing;

    // TODO: Add debug to see next spawning point
}

float UAlphabetInstanceSubsystem::GetProjectedWidth(const AActor* actor, const FVector& forward)
{
    FVector Fwd = forward.GetSafeNormal();

    //// Get local-aligned box extents (not world AABB)
    //FBoxSphereBounds Bounds = actor->GetComponentsBoundingBox();
    //FVector Half = Bounds.BoxExtent;
    //
    //// Build local axes from actor’s rotation
    //const FTransform& T = actor->GetActorTransform();
    //FVector XAxis = T.GetUnitAxis(EAxis::X) * Half.X;
    //FVector YAxis = T.GetUnitAxis(EAxis::Y) * Half.Y;
    //FVector ZAxis = T.GetUnitAxis(EAxis::Z) * Half.Z;
    //
    //// Projection = how much each contributes along forward
    //float projection =
    //    FMath::Abs(FVector::DotProduct(XAxis, Fwd)) +
    //    FMath::Abs(FVector::DotProduct(YAxis, Fwd)) +
    //    FMath::Abs(FVector::DotProduct(ZAxis, Fwd));
    //
    //UE_LOG(LogTemp, Log, TEXT("Projection symbol: %f\n"), projection);
    //
    //return projection * 2.f; // full width

    FBoxSphereBounds Bounds = actor->GetComponentsBoundingBox();
    DrawDebugBox(GetWorld(), actor->GetActorLocation(), Bounds.BoxExtent, FColor::Cyan, true, -1.f, 0, 1.f);



    return Bounds.BoxExtent.X * 2.f; // full width
}

FVector UAlphabetInstanceSubsystem::GetStaticMeshSize(const AAlphabetSymbol* Actor)
{
    if (!Actor)
        return FVector::ZeroVector;

    // Find the StaticMeshComponent in the actor
    /*const UStaticMeshComponent* MeshComp = Actor->FindComponentByClass<UStaticMeshComponent>();
    if (!MeshComp)
        return FVector::ZeroVector;*/
    
    // Get the static mesh asset

    //if (const UChildActorComponent* ChildComp = Actor->FindComponentByClass<UChildActorComponent>())
    //{
    //    if (AActor* ChildActor = ChildComp->GetChildActor())
    //    {
    //        if (const UStaticMeshComponent* MeshComp = ChildActor->FindComponentByClass<UStaticMeshComponent>())
    //        {
    //            // use MeshComp->GetStaticMesh()
    //        }
    //    }
    //}

    //const UStaticMesh* Mesh = MeshComp->GetStaticMesh();
    //if (!Mesh)
    //    return FVector::ZeroVector;

    //// Get bounds in *local space* (independent of rotation)
    //const FBoxSphereBounds LocalBounds = Mesh->GetBounds();

    //// Full size (not half extents)
    //return LocalBounds.BoxExtent * 2.f;

    return Actor->GetMeshSize();
}

void UAlphabetInstanceSubsystem::SpawnCube(const FVector location, FRotator rotation)
{
    TObjectPtr<UWorld> world = GetWorld();
    if (!world)
    {
        UE_LOG(LogTemp, Error, TEXT("AlphabetInstanceSubsystem: No valid world!"));

        return nullptr;
    }

    FActorSpawnParameters spawnParameters;
    spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    FVector SpawnLocation = location;
    FRotator SpawnRotation = rotation;

    TObjectPtr<ASymbolCube> NewSymbol = world->SpawnActor<ASymbolCube>(SymbolCubeBlueprint, SpawnLocation, SpawnRotation, spawnParameters);
    if (NewSymbol)
    {
        UE_LOG(LogTemp, Log, TEXT("AlphabetInstanceSubsystem: Successfully spawned symbol: %c"), (TCHAR)symbol);
        return NewSymbol;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AlphabetInstanceSubsystem: Failed to spawn symbol: %c"), (TCHAR)symbol);
        return nullptr;
    }

    return nullptr;
}

void UAlphabetInstanceSubsystem::LoadCubeSymbol()
{
    FString Path = FString::Printf(TEXT("/Game/Blueprint/Symbol/BP_SymbolCube.BP_SymbolCube_C"));// ObjectPath

    if (UClass* LoadedClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *Path)))
    {
        if (LoadedClass->IsChildOf(ASymbolCube::StaticClass()))
        {
            SymbolCubeBlueprint = LoadedClass;
        }
    }
}

PRAGMA_ENABLE_OPTIMIZATION


FVector UAlphabetInstanceSubsystem::GetSymbolSize(const int32 symbol)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("YakuzaManagerSubsystem: No valid world!"));
        return FVector(0.f, 0.f, 0.f);
    }


    const TSubclassOf<AAlphabetSymbol>* Result = AlphabetMap.Find((TCHAR)symbol);
    if (Result == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("AlphabetInstanceSubsystem: No valid Blueprint class found for symbol: %c"), (TCHAR)symbol);
        return FVector(0.f, 0.f, 0.f);
    }

    //Result->Get()->Actor
    return FVector(100.f, 100.f, 100.f);
}


FVector UAlphabetInstanceSubsystem::GetActorDimensions(const AAlphabetSymbol* const symbol)
{
    FVector Origin;
    FVector BoxExtent; // Half-size of the bounding box
    //float SphereRadius;

    // Get the bounding box of the Actor, including all its components
    symbol->GetActorBounds(true, Origin, BoxExtent);

    FBoxSphereBounds Bounds = symbol->GetComponentsBoundingBox();

    // BoxExtent represents half of the dimensions, so multiply by 2 to get full size
    FVector ActorSize = Bounds.BoxExtent * 2.0f;

    UE_LOG(LogTemp, Warning, TEXT("Actor Size: X=%.2f, Y=%.2f, Z=%.2f"), ActorSize.X, ActorSize.Y, ActorSize.Z);

    return ActorSize;
}

AAlphabetSymbol* UAlphabetInstanceSubsystem::SpawmSymbol(const int32 symbol, const FVector location, FRotator rotation)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("AlphabetInstanceSubsystem: No valid world!"));

        return nullptr;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // Define spawn transform (modify as needed)
    FVector SpawnLocation = location; // Example: spawn at (0, 0, 0)
    FRotator SpawnRotation = rotation; // Example: no rotation

    // Look up the TSubclassOf in the AlphabetMap
    const TSubclassOf<AAlphabetSymbol>* Result = AlphabetMap.Find((TCHAR)symbol);
    if (Result == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("AlphabetInstanceSubsystem: No valid Blueprint class found for symbol: %c"), (TCHAR)symbol);

        return nullptr;
    }

    AAlphabetSymbol* NewSymbol = World->SpawnActor<AAlphabetSymbol>(*Result, SpawnLocation, SpawnRotation, SpawnParams);
    if (NewSymbol)
    {
        UE_LOG(LogTemp, Log, TEXT("AlphabetInstanceSubsystem: Successfully spawned symbol: %c"), (TCHAR)symbol);
        return NewSymbol;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AlphabetInstanceSubsystem: Failed to spawn symbol: %c"), (TCHAR)symbol);
        return nullptr;
    }
}

ASymbolCube* UAlphabetInstanceSubsystem::SpawmSymbolByCubes(const int32 symbol, const FVector location, FRotator rotation)
{
    // Write a system to spawn symbol by cubes

    /*
    * Input data: symbol char, location (start location to spawn a new symbol), rotation
    * Output data: spawned symbol, offset to move next symbol
    * 
    * Symbol representation by 1 and 0 in 4x4 grid
    * 0110
    * 1001
    * 1111
    * 1001
    * 
    * Case I 
    * 1110
    * 0100
    * 0100
    * 1110
    * 
    */

    SpawnCube(location, rotation);
}



void UAlphabetInstanceSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
    Super::OnWorldBeginPlay(InWorld);
    //const FString albet{ "ABCDEFGHIJKLMOPRSTUVWXYZ" };


    //LoadAlphabet(albet);
    LoadCubeSymbol();
    // Load Single Cube Blueprint
}


void UAlphabetInstanceSubsystem::LoadAlphabet(const FString& alphabet)
{
    TSubclassOf<AAlphabetSymbol> blueprintClass = nullptr;
    for (const TCHAR symbol : alphabet)
    {

        if (blueprintClass = FindBlueprint(symbol))
        {
            AlphabetMap.FindOrAdd(symbol, blueprintClass);
        }
    }
    UE_LOG(LogTemp, Log, TEXT("Alphabet Loaded size %d"), AlphabetMap.Num());

    // Debug 
    for (const auto keyValue : AlphabetMap)
    {
        FString className;
        keyValue.Value->GetName(className);
        TCHAR key = static_cast<TCHAR>(keyValue.Key);
        UE_LOG(LogTemp, Log, TEXT("%c - %s"), key, *className);
    }
}

TSubclassOf<AAlphabetSymbol> UAlphabetInstanceSubsystem::FindBlueprint(const int32 symbol)
{
    TSubclassOf<AAlphabetSymbol> BlueprintClass = nullptr;

    FString Path = FString::Printf(TEXT("/Game/BP_Symbol_%c.BP_Symbol_%c_C"), (TCHAR)symbol, (TCHAR)symbol);

    if (UClass* LoadedClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *Path)))
    {
        if (LoadedClass->IsChildOf(AAlphabetSymbol::StaticClass()))
        {
            BlueprintClass = LoadedClass;
        }
    }

    return BlueprintClass;
}


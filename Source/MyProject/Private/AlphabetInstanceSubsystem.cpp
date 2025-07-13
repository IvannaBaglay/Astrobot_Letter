// Fill out your copyright notice in the Description page of Project Settings.


#include "AlphabetInstanceSubsystem.h"
#include "AlphabetSymbol.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

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

void UAlphabetInstanceSubsystem::SpawnSentence(FString sentence, const FVector startLocation, FRotator rotation, FVector forward)
{
    FVector offset = FVector::Zero();
    for (int32 symbol : sentence)
    {
        FVector spawnedSize = SpawmSymbol(symbol, startLocation + offset, rotation);
        MoveOffset(spawnedSize, offset, forward);
    }
}

void UAlphabetInstanceSubsystem::MoveOffset(const FVector spawnedSize, FVector& offset, FVector forward)
{
    // Can I use symbol size of will be better have one const offset?
    // Need to add 
    // Find symbol size
    FVector symbolSize = forward * (spawnedSize + FVector(10.f, 10.f, 10.f));

    offset += symbolSize;
}


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

    //GetActorDimensions

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

    // BoxExtent represents half of the dimensions, so multiply by 2 to get full size
    FVector ActorSize = BoxExtent * 2.0f;

    UE_LOG(LogTemp, Warning, TEXT("Actor Size: X=%.2f, Y=%.2f, Z=%.2f"), ActorSize.X, ActorSize.Y, ActorSize.Z);

    return ActorSize;
}

FVector UAlphabetInstanceSubsystem::SpawmSymbol(const int32 symbol,const FVector location, FRotator rotation)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("YakuzaManagerSubsystem: No valid world!"));

        return FVector::ZeroVector;
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

        return FVector::ZeroVector;
    }

    AAlphabetSymbol* NewSymbol = World->SpawnActor<AAlphabetSymbol>(*Result, SpawnLocation, SpawnRotation, SpawnParams);
    if (NewSymbol)
    {
        UE_LOG(LogTemp, Log, TEXT("AlphabetInstanceSubsystem: Successfully spawned symbol: %c"), (TCHAR)symbol);
        return GetActorDimensions(NewSymbol);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("AlphabetInstanceSubsystem: Failed to spawn symbol: %c"), (TCHAR)symbol);
        return FVector::ZeroVector;
    }
}

void UAlphabetInstanceSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
    //const std::string albet{ "abcdefghijklmnopqrstuvwxyz" };
    const std::string albet{ "AB" };


    LoadAlphabet(albet);
}


void UAlphabetInstanceSubsystem::LoadAlphabet(const std::string& alphabet)
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
    //(TEXT("/Game/YakuzaImplementation/YakuzaAI_BP.YakuzaAI_BP_C"));

    FString Path = FString::Printf(TEXT("/Game/BP_Symbol_%c.BP_Symbol_%c_C"), (TCHAR)symbol, (TCHAR)symbol);
    //FString Path = FString::Printf(TEXT("/Game/BP_Symbol_A.BP_Symbol_A_C"));

    //static ConstructorHelpers::FClassFinder<AAlphabetSymbol> alphabetSymbol(*Path);

    if (UClass* LoadedClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), nullptr, *Path)))
    {
        if (LoadedClass->IsChildOf(AAlphabetSymbol::StaticClass()))
        {
            BlueprintClass = LoadedClass;
        }
    }

    return BlueprintClass;
}


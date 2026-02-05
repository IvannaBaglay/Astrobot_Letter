// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include<map>

#include "AlphabetInstanceSubsystem.generated.h"

class AAlphabetSymbol;
class ASymbolCube;

UCLASS()
class MYPROJECT_API UAlphabetInstanceSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:

     UAlphabetInstanceSubsystem();
     
     void OnWorldBeginPlay(UWorld& InWorld) override;
     
     UFUNCTION(BlueprintCallable, Category = "Alphabet")
     void RemoveSymbol(AActor* symbol);
     
     UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Alphabet")
     int32 GetSymbolsSize() const;
     
     UFUNCTION()
     void AddAlphabet(int32 symbol); // TODO: how to create fabric?>?
     UFUNCTION()
     AAlphabetSymbol* SpawmSymbol(const int32 symbol,const FVector location, FRotator rotation); // call it in trigger area to spawn symbol
     bool SpawmSymbolByCubes(const int32 symbol,const FVector location, FRotator rotation, FVector forward); // call it in trigger area to spawn symbol
     UFUNCTION()
     void SpawnSentence(FString sentence, const FVector startLocation, FRotator rotation, FVector forward);

private:
    // To be depricated
    void LoadAlphabet(const FString& alphabet); // check blueprints
    TSubclassOf<AAlphabetSymbol> FindBlueprint(const int32 symbol);
    void MoveOffset(FVector symbolSize, FVector& offset, FVector forward);
    FVector GetSymbolSize(const int32 symbol);
    FVector GetActorDimensions(const AAlphabetSymbol* const symbol);
    float GetProjectedWidth(const AActor* actor, const FVector& forward);
    FVector GetStaticMeshSize(const AAlphabetSymbol* Actor);

private:
    // New system to spawn cube
    TObjectPtr<ASymbolCube> SpawnCube(const FVector location, FRotator rotation);
    void LoadCubeSymbol();

public: 
    //UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
    UPROPERTY()
    TMap<uint32, TSubclassOf<AAlphabetSymbol>> AlphabetMap; // std::map<char, Symbol>
    
    UPROPERTY()
    TArray<AActor*> Symbols;

    TSubclassOf<ASymbolCube> SymbolCubeBlueprint;
    
    // String language // to choose which folder will be loaded. for future
};
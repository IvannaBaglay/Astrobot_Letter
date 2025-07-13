// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include<map>

#include "AlphabetInstanceSubsystem.generated.h"

class AAlphabetSymbol;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UAlphabetInstanceSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	// Begin USubsystem
	//virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//virtual void Deinitialize() override;
	// End USubsystem

	UAlphabetInstanceSubsystem();

	void OnWorldBeginPlay(UWorld& InWorld) override;

	UFUNCTION(BlueprintCallable, Category = "Alphabet")
	void RemoveSymbol(AActor* symbol);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Alphabet")
	int32 GetSymbolsSize() const;

	UFUNCTION()
	void AddAlphabet(int32 symbol); // TODO: how to create fabric?>?
	UFUNCTION()
	FVector SpawmSymbol(const int32 symbol,const FVector location, FRotator rotation); // call it in trigger area to spawn symbol
	UFUNCTION()
	void SpawnSentence(FString sentence, const FVector startLocation, FRotator rotation, FVector forward);

private:
	void LoadAlphabet(const std::string& alphabet); // check blueprints
	TSubclassOf<AAlphabetSymbol> FindBlueprint(const int32 symbol);
	void MoveOffset(const FVector spawnedSize, FVector& offset, FVector forward);
	FVector GetSymbolSize(const int32 symbol);
	FVector GetActorDimensions(const AAlphabetSymbol* const symbol);

public: 
	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	UPROPERTY()
	TMap<uint32, TSubclassOf<AAlphabetSymbol>> AlphabetMap; // std::map<char, Symbol>

	UPROPERTY()
	TArray<AActor*> Symbols;

	// String language // to choose which folder will be loaded. for future
};

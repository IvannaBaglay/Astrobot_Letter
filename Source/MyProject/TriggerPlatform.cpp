// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerPlatform.h"
// include draw debu helpers header file
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "AlphabetSymbol.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

#include "AlphabetInstanceSubsystem.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text), fstring))

// Sets default values
ATriggerPlatform::ATriggerPlatform()
{
    

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
    // Set up the trigger box
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    TriggerBox->SetupAttachment(RootComponent);
    TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerPlatform::OnOverlapComponentBegin);

    //Register Events
	OnActorBeginOverlap.AddDynamic(this, &ATriggerPlatform::OnOverlapActorBegin);
	OnActorEndOverlap.AddDynamic(this, &ATriggerPlatform::OnOverlapEnd);

    // Set up the spawn zone
    SpawnZone = CreateDefaultSubobject<UArrowComponent>(TEXT("SpawnZone"));
    SpawnZone->SetupAttachment(RootComponent);
    // Set a default offset above the trigger (e.g., 100 units up)
    SpawnZone->SetWorldLocation(this->GetActorLocation());
    SpawnZone->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

    //// Set up the spawn zone
    //SpawnZoneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnZoneComponent"));
    //SpawnZoneComponent->SetupAttachment(RootComponent);
    //// Set a default offset above the trigger (e.g., 100 units up)
    //SpawnZoneComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

}

// Called when the game starts or when spawned
void ATriggerPlatform::BeginPlay()
{
	Super::BeginPlay();
    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, 999, 0, 5);
}

// Called every frame
void ATriggerPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATriggerPlatform::OnOverlapComponentBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // check if Actors do not equal nullptr and that 
    if (OtherActor && OtherActor != this)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, TEXT("Overlap Begin"));
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Overlapped Actor = %s"), *OverlappedComp->GetName()));
        }
    }

    if (SpawnZone == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnZone is nullptr"));
        return;
    }

    FTransform SpawnTransform = SpawnZone->GetComponentTransform();

    UAlphabetInstanceSubsystem* Subsystem = GetWorld()->GetSubsystem<UAlphabetInstanceSubsystem>();
    if (Subsystem)
    {
        UE_LOG(LogTemp, Log, TEXT("TriggerZone: Spawned symbol %s"), *Str);
        UE_LOG(LogTemp, Log, TEXT("TriggerZone: Spawned symbol at %s"), *SpawnTransform.GetLocation().ToString());
        FVector position = SpawnTransform.GetLocation();
        FRotator rotation = SpawnTransform.Rotator();
        FVector forward = SpawnZone->GetForwardVector();
        Subsystem->SpawnSentence(Str, position, rotation, forward);
    }
}

void ATriggerPlatform::OnOverlapActorBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
    //// check if Actors do not equal nullptr and that 
    //if (OtherActor && OtherActor != this)
    //{
    //    if (GEngine)
    //    {
    //        GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, TEXT("Overlap Begin"));
    //        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Overlapped Actor = %s"), *OverlappedActor->GetName()));
    //    }
    //}
    //
    //
    //FTransform SpawnTransform = SpawnZone->GetComponentTransform();
    //
    //UAlphabetInstanceSubsystem* Subsystem = GetWorld()->GetSubsystem<UAlphabetInstanceSubsystem>();
    //if (Subsystem)
    //{
    //    UE_LOG(LogTemp, Log, TEXT("TriggerZone: Spawned symbol %s"), *Str);
    //    UE_LOG(LogTemp, Log, TEXT("TriggerZone: Spawned symbol at %s"), *SpawnTransform.GetLocation().ToString());
    //    FVector position = SpawnTransform.GetLocation();
    //    Subsystem->SpawnSentence(Str, position);
    //}
}

void ATriggerPlatform::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
    if (OtherActor && OtherActor != this)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, TEXT("Overlap Ended"));
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("%s has left the Trigger Box"), *OtherActor->GetName()));
        }
    }
}


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
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "AlphabetInstanceSubsystem.h"
#include "DrawDebugHelpers.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT(text), fstring))

// Sets default values
ATriggerPlatform::ATriggerPlatform()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>("Root");
    SetRootComponent(Root);

    // In Constructor
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
    Mesh->SetupAttachment(GetRootComponent());

    TriggerBoxComponent = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
    TriggerBoxComponent->SetupAttachment(GetRootComponent());

    TriggerBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); 
    TriggerBoxComponent->SetCollisionObjectType(ECC_WorldDynamic);
    TriggerBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    SpawnPointComponent = CreateDefaultSubobject<USphereComponent>("SpawnPoint");
    SpawnPointComponent->SetupAttachment(GetRootComponent());

    ArrowComponent = CreateDefaultSubobject<UArrowComponent>("ArrowForward");
    ArrowComponent->SetupAttachment(SpawnPointComponent);
    
    TriggerBoxComponent->SetCollisionProfileName(TEXT("Trigger"));
    TriggerBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATriggerPlatform::OnOverlapComponentBegin);

}

// Called when the game starts or when spawned
void ATriggerPlatform::BeginPlay()
{
    Super::BeginPlay();


    FVector debugTransformLocation = SpawnPointComponent->GetComponentTransform().GetLocation();
    FVector debugTransformForward = SpawnPointComponent->GetForwardVector();
    DrawDebugLine(GetWorld(), debugTransformLocation, debugTransformLocation + debugTransformForward * 100.f, FColor::Orange, true, -1.f, 0, 3.f);


    //Register Events
    OnActorBeginOverlap.AddDynamic(this, &ATriggerPlatform::OnOverlapActorBegin);
    OnActorEndOverlap.AddDynamic(this, &ATriggerPlatform::OnOverlapEnd);
}

// Called every frame
void ATriggerPlatform::Tick(float DeltaTime)
{
    // Super::Tick(DeltaTime);
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

    if (SpawnPointComponent == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnZone is nullptr"));
        return;
    }

    FTransform SpawnTransform = SpawnPointComponent->GetComponentTransform();

    UAlphabetInstanceSubsystem* Subsystem = GetWorld()->GetSubsystem<UAlphabetInstanceSubsystem>();
    if (Subsystem)
    {
        UE_LOG(LogTemp, Log, TEXT("TriggerZone: Spawned symbol %s"), *Str);
        UE_LOG(LogTemp, Log, TEXT("TriggerZone: Spawned symbol at %s"), *SpawnTransform.GetLocation().ToString());
        FVector position = SpawnTransform.GetLocation();
        FRotator rotation = SpawnTransform.Rotator();
        FVector forward = SpawnPointComponent->GetForwardVector();
        FVector forwardFromRotation = UKismetMathLibrary::GetForwardVector(rotation);
        Subsystem->SpawnSentence(Str, position, rotation, forward);
    }   
}

void ATriggerPlatform::OnOverlapActorBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
    if (OtherActor && OtherActor != this)
    {
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green, TEXT("Overlap Actor Begin"));
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("%s has entered the Trigger Box"), *OtherActor->GetName()));
        }
    }
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


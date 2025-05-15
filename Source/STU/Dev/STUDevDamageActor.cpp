// Shoot Them Up Game, All Right Reserved.

#include "STUDevDamageActor.h"
//с помощью него можно рисовать дебаг
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogDevDamageActor, All, All);


// Sets default values
ASTUDevDamageActor::ASTUDevDamageActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //подключение xyz сцены
    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    SetRootComponent(SceneComponent);

    //смещение локации вверх на 100
    SceneComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
}

// Called when the game starts or when spawned
void ASTUDevDamageActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ASTUDevDamageActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 24, SphereColor);

    //получение урона 
    UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), Radius, nullptr, {}, this, nullptr, DoFullDamage);

    //UE_LOG(LogDevDamageActor, Display, TEXT("I hit damage: %f"), Damage);

}

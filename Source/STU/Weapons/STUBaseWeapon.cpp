// Shoot Them Up Game, All Right Reserved.

#include "STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "STU/Character/STUBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "STU/Dev/STUFireDamageType.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponMesh);
}

void ASTUBaseWeapon::StartFire()
{
    MakeShot();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTUBaseWeapon::MakeShot, TimeBetweenShot, true);
}

void ASTUBaseWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

//выстрел
void ASTUBaseWeapon::MakeShot()
{
    if (!GetWorld()) return;

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd)) return;

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit)
    {
        //вызов фукнции нанесения урона
        MakeDamage(HitResult);

        //рисуем линию
        //persistance - отрисовка один раз
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 5.0f, 0, 3.0f);

        //рисование сферы в точки попадания
        DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);

        //вывод кости в которую попали
        UE_LOG(LogBaseWeapon, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());

        //----------
        //AActor* HitActor = HitResult.GetActor();

        //if (!HitActor) return;
        //UE_LOG(LogBaseWeapon, Display, TEXT("Actor: %s"), *HitActor->GetName());
        ////UE_LOG(LogBaseWeapon, Display, TEXT("Actor: %s"), *GetNameSafe(HitResult.GetActor()));

        //if (HitActor->IsA<ASTUBaseCharacter>())
        //{
        //    if (DamageTypeClass) return;

        //    UE_LOG(LogBaseWeapon, Display, TEXT("EnemyActor is find!"));
        //    UGameplayStatics::ApplyDamage(HitActor, 10.0f, nullptr, this, DamageTypeClass);
        //}
    }
    else
    {
        //рисуем линию
        //persistance - отрисовка один раз
        DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 5.0f, 0, 3.0f);
    }
}

//функция возвращает контроллер
APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player) return nullptr;

    return Player->GetController<APlayerController>();
}

//получение ViewPoint player
bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto Controller = GetPlayerController();
    if (!Controller) return false;

    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

//получение данные позиции сокета
FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

//получение данных для trace
bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    //начальное положение это положение камеры
    TraceStart = ViewLocation;

    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);

    //берем forward vector из сокета или начала
    //направление вектора в какую сторону стрельба идет
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);


    //для точки конца берем начало + направление вектора с домноженная дистанцией (1500 ед)
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

//функция трейса
void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!GetWorld()) return;

    //акторы для игнора
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    //возвращает первый объект с которым будет пересечение
    //записывает данные в HitResult
    //блокает для всех видных объектов
    GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

void ASTUBaseWeapon::MakeDamage(FHitResult& HitResult)
{
    const auto DamagedActor = HitResult.GetActor();
    if (!DamagedActor) return;

    DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}
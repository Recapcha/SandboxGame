// Shoot Them Up Game, All Right Reserved.

#include "STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "STU/Weapons/Components/STUWeaponFXComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRifleWeapon, All, All);

ASTURifleWeapon::ASTURifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponComponent");
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponFXComponent);
}

void ASTURifleWeapon::StartFire()
{
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShot, true);
    MakeShot();
}

void ASTURifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
}

//выстрел
void ASTURifleWeapon::MakeShot()
{
    //UE_LOG(LogTemp, Display, TEXT("Make shot"));

    if (!GetWorld() || IsAmmoEmpty())
    {
        StopFire();
        return;
    }

    FVector TraceStart, TraceEnd;
    if (!GetTraceData(TraceStart, TraceEnd))
    {
        StopFire();
        return;
    }

    FHitResult HitResult;
    MakeHit(HitResult, TraceStart, TraceEnd);

    if (HitResult.bBlockingHit)
    {
        //вызов фукнции нанесения урона
        MakeDamage(HitResult);

        //рисуем линию
        //persistance - отрисовка один раз
        //DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 5.0f, 0, 3.0f);

        //рисование сферы в точки попадания
        //DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);

        //вывод кости в которую попали
        //UE_LOG(LogRifleWeapon, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());

        WeaponFXComponent->PlayImpactFX(HitResult);
    }
    else
    {
        //рисуем линию
        //persistance - отрисовка один раз
        //DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 5.0f, 0, 3.0f);
    }

    DecreaseAmmo();
}

//получение данных для trace
bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
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

void ASTURifleWeapon::MakeDamage(FHitResult& HitResult)
{
    const auto DamagedActor = HitResult.GetActor();
    if (!DamagedActor) return;

    DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}
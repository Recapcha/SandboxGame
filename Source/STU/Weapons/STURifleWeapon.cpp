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

//�������
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
        //����� ������� ��������� �����
        MakeDamage(HitResult);

        //������ �����
        //persistance - ��������� ���� ���
        //DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.ImpactPoint, FColor::Red, false, 5.0f, 0, 3.0f);

        //��������� ����� � ����� ���������
        //DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);

        //����� ����� � ������� ������
        //UE_LOG(LogRifleWeapon, Display, TEXT("Bone: %s"), *HitResult.BoneName.ToString());

        WeaponFXComponent->PlayImpactFX(HitResult);
    }
    else
    {
        //������ �����
        //persistance - ��������� ���� ���
        //DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), TraceEnd, FColor::Red, false, 5.0f, 0, 3.0f);
    }

    DecreaseAmmo();
}

//��������� ������ ��� trace
bool ASTURifleWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    //��������� ��������� ��� ��������� ������
    TraceStart = ViewLocation;

    const auto HalfRad = FMath::DegreesToRadians(BulletSpread);

    //����� forward vector �� ������ ��� ������
    //����������� ������� � ����� ������� �������� ����
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);

    //��� ����� ����� ����� ������ + ����������� ������� � ����������� ���������� (1500 ��)
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

void ASTURifleWeapon::MakeDamage(FHitResult& HitResult)
{
    const auto DamagedActor = HitResult.GetActor();
    if (!DamagedActor) return;

    DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}
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
    //��������� �������� ���������������� ���������
    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn't be less or zero"));
    checkf(DefaultAmmo.Clips > 0, TEXT("Clips count couldn't be less or zero"));
    CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::StartFire()
{
}

void ASTUBaseWeapon::StopFire()
{
}

void ASTUBaseWeapon::MakeShot()
{
}

//������� ���������� ����������
APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    const auto Player = Cast<ACharacter>(GetOwner());
    if (!Player) return nullptr;

    return Player->GetController<APlayerController>();
}

//��������� ViewPoint player
bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
    const auto Controller = GetPlayerController();
    if (!Controller) return false;

    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
    return true;
}

//��������� ������ ������� ������
FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}

//��������� ������ ��� trace
bool ASTUBaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    //��������� ��������� ��� ��������� ������
    TraceStart = ViewLocation;

    //����� forward vector �� ������ ��� ������
    //����������� ������� � ����� ������� �������� ����
    const FVector ShootDirection = ViewRotation.Vector();

    //��� ����� ����� ����� ������ + ����������� ������� � ����������� ���������� (1500 ��)
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
    return true;
}

//������� ������
void ASTUBaseWeapon::MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    if (!GetWorld()) return;

    //������ ��� ������
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    //������������ ��������� � phys mat
    CollisionParams.bReturnPhysicalMaterial = true;

    //���������� ������ ������ � ������� ����� �����������
    //���������� ������ � HitResult
    //������� ��� ���� ������ ��������
    GetWorld()->LineTraceSingleByChannel       //
        (                                      //
            HitResult,                         //
            TraceStart,                        //
            TraceEnd,                          //
            ECollisionChannel::ECC_Visibility, //
            CollisionParams                    //
        );
}

//���������� �������� � ��������
void ASTUBaseWeapon::DecreaseAmmo()
{
    if (CurrentAmmo.Bullets == 0)
    {
        UE_LOG(LogBaseWeapon, Warning, TEXT("Clip is empty"));
        return;
    }

    CurrentAmmo.Bullets--;
    //LogAmmo();

    if (IsClipEmpty() && !IsAmmoEmpty())
    {
        StopFire();
        OnClipEmpty.Broadcast(this);
    }
}

//���� �� ������ ����� �������
bool ASTUBaseWeapon::IsAmmoEmpty() const
{
    return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}

//������ �� �������
bool ASTUBaseWeapon::IsClipEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
    return CurrentAmmo.Clips == DefaultAmmo.Clips && //
           CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}

//����������� ������
void ASTUBaseWeapon::ChangeClip()
{
    if (!CurrentAmmo.Infinite)
    {
        if (CurrentAmmo.Clips == 0)
        {
            UE_LOG(LogBaseWeapon, Warning, TEXT("No more clips"));
            return;
        }
        CurrentAmmo.Clips--;
    }
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    UE_LOG(LogBaseWeapon, Display, TEXT("-----Change Clip -----"));
}

bool ASTUBaseWeapon::CanReload() const
{
    return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}

//����������� ������ ��� ����������
bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
    if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0) return false;

    if (IsAmmoEmpty())
    {
        UE_LOG(LogBaseWeapon, Display, TEXT("Ammo was empty!"))
        //CurrentAmmo.Clips = 0
        CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1);
        OnClipEmpty.Broadcast(this);
    }
    else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
    {
        const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount;
        if (DefaultAmmo.Clips - NextClipsAmount >= 0)
        {
            CurrentAmmo.Clips = NextClipsAmount;
            UE_LOG(LogBaseWeapon, Display, TEXT("Clips was added!"))
        }
        else
        {
            CurrentAmmo.Clips = DefaultAmmo.Clips;
            CurrentAmmo.Bullets = DefaultAmmo.Bullets;
            UE_LOG(LogBaseWeapon, Display, TEXT("Ammo was full now!"))
        }
    }
    else
    {
        CurrentAmmo.Bullets = DefaultAmmo.Bullets;
        UE_LOG(LogBaseWeapon, Display, TEXT("Bullets ware added!"))
    }

    return true;
}

//������� ���������� � �����������
void ASTUBaseWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);
    UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo)
}
// Shoot Them Up Game, All Right Reserved.

#include "STUWeaponComponent.h"
#include "STU/Weapons/STUBaseWeapon.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(WeaponComponentLog, All, All);

USTUWeaponComponent::USTUWeaponComponent()
{

    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    SpawnWeapon();
}

void USTUWeaponComponent::SpawnWeapon()
{
    //если указатель на мир игры не нулевой
    if (!GetWorld()) return;
    //UE_LOG(WeaponComponentLog, Display, TEXT("World is find!"));

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;
    //UE_LOG(WeaponComponentLog, Display, TEXT("Character is find!"));

    //спавн оружия
    CurrentWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
    if (!CurrentWeapon) return;
    //UE_LOG(WeaponComponentLog, Display, TEXT("CurrentWeapon is find!"));

    //аттач к мешу, руке
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
    //UE_LOG(WeaponComponentLog, Display, TEXT("Weapon is attached!"));

    CurrentWeapon->SetOwner(Character);
}

void USTUWeaponComponent::StartFire()
{
    if (!CurrentWeapon) return;

    //если есть оружие, то вызов из ASTUBaseWeapon Fire
    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
    if (!CurrentWeapon) return;
    CurrentWeapon->StopFire();
}
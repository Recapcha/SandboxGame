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
    //���� ��������� �� ��� ���� �� �������
    if (!GetWorld()) return;
    //UE_LOG(WeaponComponentLog, Display, TEXT("World is find!"));

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;
    //UE_LOG(WeaponComponentLog, Display, TEXT("Character is find!"));

    //����� ������
    CurrentWeapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
    if (!CurrentWeapon) return;
    //UE_LOG(WeaponComponentLog, Display, TEXT("CurrentWeapon is find!"));

    //����� � ����, ����
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
    //UE_LOG(WeaponComponentLog, Display, TEXT("Weapon is attached!"));

    CurrentWeapon->SetOwner(Character);
}

void USTUWeaponComponent::StartFire()
{
    if (!CurrentWeapon) return;

    //���� ���� ������, �� ����� �� ASTUBaseWeapon Fire
    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
    if (!CurrentWeapon) return;
    CurrentWeapon->StopFire();
}
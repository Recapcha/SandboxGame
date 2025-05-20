// Shoot Them Up Game, All Right Reserved.

#include "STUWeaponComponent.h"
#include "STU/Weapons/STUBaseWeapon.h"
#include "GameFramework/Character.h"
#include "STU/Animations/STUEquipFinishAnimNotify.h"
#include "STU/Animations/STUReloadFinishAnimNotify.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

USTUWeaponComponent::USTUWeaponComponent()
{

    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentWeaponIndex = 0;
    InitAnimations();
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

//������� ��� �����������
void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;
    for (auto Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::SpawnWeapons()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || !GetWorld()) return;

    for (auto OneWeaponData : WeaponData)
    {
        //����� ������
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon) continue;

        Weapon->SetOwner(Character);
        Weapons.Add(Weapon);

        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

//������������ ������
void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
    {
        UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
        return;
    }

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }

    CurrentWeapon = Weapons[WeaponIndex];
    //CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
    //������ ��� ����� ������ � ��������� ������ ��������, ����������� ��� ���
    //� CurrentWeaponData ����� ��������� ��������� �� ��������� ���������
    //FinDByPredicate ��� �������������� ������, ������� ���������� true/false
    //� ���� ������� �������������� ������ ��� �������� []
    //�� ������������� ��������� ������� ��������, ������� ����� �����������
    //� ������� �������� �������
    //� � ������ ������ ���������, ������� �������� ��������� �������
    // ��������� � ������� CurrentWeapon
    // �� ��� �������� ��������� �� ������ ���������, ���� ��� �� nullptr
    //
    // [&] �������� ������ ������� ����������, �� ���� �� ����� ���������� � ���������
    // CurrentWeapon, � �������� ������� Data ����� ��� �������� ������� WeaponData
    //
    // � ������� �������� ������� ����� ��������� ������ ������ � ��� ������ ����������,
    // ��� �������� ��������� �� ������� �������
    //
    // ��������� CurrentReloadAnimMontage �� �������� ����������� �� ��������� ���������
    // CurrentWeaponData ���� �� ����, �� ���������� � ����, ���� ����, �� nullptr
    //
    const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) //
        {                                                                                  //
            return Data.WeaponClass == CurrentWeapon->GetClass();
        });
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
    EquipAnimInProgress = true;
    PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if (!Weapon || !SceneComponent) return;

    //����� � ����, ����
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::StartFire()
{
    if (!CanFire()) return;

    //���� ���� ������, �� ����� �� ASTUBaseWeapon Fire
    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
    if (!CurrentWeapon) return;
    CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon()
{
    if (!CanEquip()) return;

    //��� �� ���������� �� ����� �� ������� �������
    //����� �� �� ������ ����� �������, �� ���� ���� �������� �������� ����� ����� ����� �������
    //�� ����� ����� 0
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    Character->PlayAnimMontage(Animation);
}

//����� ��
void USTUWeaponComponent::InitAnimations()
{
    //����� ������
    //����� �� �������� ����� ������
    auto EquipFinishNotify = FindNotifyByClass<USTUEquipFinishAnimNotify>(EquipAnimMontage);
    if (EquipFinishNotify)
    {
        //������������� �� ������� notify, ��� ��� ������� ��������� ������� OnEquipFinished
        EquipFinishNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }

    //�����������
    //����� �� ������� �� �������� ��� �����������
    for (auto OneWeaponData : WeaponData)
    {
        auto ReloadFinishNotify = FindNotifyByClass<USTUReloadFinishAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if (!ReloadFinishNotify) continue;

        //���� ����� ������������� �� ������� notify � ��� ��� ������� ��������� ������� OnReloadFinished
        ReloadFinishNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
    }
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || MeshComponent != Character->GetMesh()) return;

    EquipAnimInProgress = false;
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || MeshComponent != Character->GetMesh()) return;

    ReloadAnimInProgress = false;
}

bool USTUWeaponComponent::CanFire() const
{
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

//������������ EquipAnimInProgress, �������� ����� �� �� ������ ������ ������
//�� �� ����� �� ����� ��� �����
bool USTUWeaponComponent::CanEquip() const
{
    return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanReload() const
{
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

//�����������
void USTUWeaponComponent::Reload()
{
    if (!CanReload()) return;
    ReloadAnimInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
}
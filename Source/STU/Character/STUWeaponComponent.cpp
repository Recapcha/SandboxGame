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

//функция при уничтожении
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
        //спавн оружия
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon) continue;

        Weapon->SetOwner(Character);
        Weapons.Add(Weapon);

        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

//экипирование оружея
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
    //каждый раз будет искать с структуре нужную анимацию, подходяющую для нас
    //в CurrentWeaponData будет храниться указатель на найденную структуру
    //FinDByPredicate это функциональный объект, который возвращает true/false
    //в свою очередь функциональный объект это оператор []
    //мы воспользуемся анонимной лямбдой функцией, которая будет применяться
    //к каждому элементу массива
    //и в случае класса структуры, которая является элементом массива
    // совпадает с классом CurrentWeapon
    // то нам вернется указатель на данную структуру, если нет то nullptr
    //
    // [&] означает захват внешних переменных, то есть мы можем обратиться к указателю
    // CurrentWeapon, а параметр функции Data имеет тип элемента массива WeaponData
    //
    // К каждому элементу массива будет применина данная функци и при первом совпадении,
    // нам вернется указатель на элемент массива
    //
    // установим CurrentReloadAnimMontage на анимацию перезарядки из найденной структуры
    // CurrentWeaponData если не нуль, то обращаемся к полю, если нуль, то nullptr
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

    //аттач к мешу, руке
    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::StartFire()
{
    if (!CanFire()) return;

    //если есть оружие, то вызов из ASTUBaseWeapon Fire
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

    //что бы переменная не вышла за приделы массива
    //берем ее по модулю длины массива, то есть если значение счетчика будет равно длине массива
    //то будет равно 0
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character) return;

    Character->PlayAnimMontage(Animation);
}

//поиск по
void USTUWeaponComponent::InitAnimations()
{
    //смена оружия
    //поиск по анимации смены оружия
    auto EquipFinishNotify = FindNotifyByClass<USTUEquipFinishAnimNotify>(EquipAnimMontage);
    if (EquipFinishNotify)
    {
        //подписываемся на делегат notify, при его проходе запускаем функцию OnEquipFinished
        EquipFinishNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }

    //перезарядка
    //поиск по массиву из анимация для перезарядки
    for (auto OneWeaponData : WeaponData)
    {
        auto ReloadFinishNotify = FindNotifyByClass<USTUReloadFinishAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if (!ReloadFinishNotify) continue;

        //если нашли подписывается на делегат notify и при его проходе запускаем функцию OnReloadFinished
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

//возвращается EquipAnimInProgress, значение можем ли мы сейчас менять оружие
//мы не можем во время уже смены
bool USTUWeaponComponent::CanEquip() const
{
    return !EquipAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanReload() const
{
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}

//Перезарядка
void USTUWeaponComponent::Reload()
{
    if (!CanReload()) return;
    ReloadAnimInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
}
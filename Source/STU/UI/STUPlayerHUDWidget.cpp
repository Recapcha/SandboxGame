// Shoot Them Up Game, All Right Reserved.

#include "STUPlayerHUDWidget.h"
#include "STU/Character/STUHealthComponent.h"
#include "STU/Character/STUWeaponComponent.h"
#include "STU/Weapons/STUBaseWeapon.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    //указатель на игрока
    //берет павн, который в данный момент управляется
    const auto Player = GetOwningPlayerPawn();
    if (!Player) return 0.0f;

    //референс на компонент здровья
    const auto Component = Player->GetComponentByClass(USTUHealthComponent::StaticClass());
    //так GetComponentByClass приводит к UActorComponent, поэтому с помощью Cast мы приводим к HealthComponent
    const auto HealthComponent = Cast<USTUHealthComponent>(Component);
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}

USTUWeaponComponent* USTUPlayerHUDWidget::GetWeaponComponent() const
{
    //указатель на игрока
    //берет павн, который в данный момент управляется
    const auto Player = GetOwningPlayerPawn();
    if (!Player) return nullptr;

    //референс на компонент оружия
    const auto Component = Player->GetComponentByClass(USTUWeaponComponent::StaticClass());
    const auto WeaponComponent = Cast<USTUWeaponComponent>(Component);
    return WeaponComponent;
}

bool USTUPlayerHUDWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    const auto WeaponComponent = GetWeaponComponent();
    if (!WeaponComponent) return false;

    return WeaponComponent->GetWeaponUIData(UIData);
}

bool USTUPlayerHUDWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    const auto WeaponComponent = GetWeaponComponent();
    if (!WeaponComponent) return false;

    return WeaponComponent->GetWeaponAmmoData(AmmoData);
}

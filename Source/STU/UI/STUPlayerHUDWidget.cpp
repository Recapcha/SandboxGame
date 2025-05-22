// Shoot Them Up Game, All Right Reserved.

#include "STUPlayerHUDWidget.h"
#include "STU/Character/STUHealthComponent.h"
#include "STU/Character/STUWeaponComponent.h"
#include "STU/Weapons/STUBaseWeapon.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    //��������� �� ������
    //����� ����, ������� � ������ ������ �����������
    const auto Player = GetOwningPlayerPawn();
    if (!Player) return 0.0f;

    //�������� �� ��������� �������
    const auto Component = Player->GetComponentByClass(USTUHealthComponent::StaticClass());
    //��� GetComponentByClass �������� � UActorComponent, ������� � ������� Cast �� �������� � HealthComponent
    const auto HealthComponent = Cast<USTUHealthComponent>(Component);
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}

USTUWeaponComponent* USTUPlayerHUDWidget::GetWeaponComponent() const
{
    //��������� �� ������
    //����� ����, ������� � ������ ������ �����������
    const auto Player = GetOwningPlayerPawn();
    if (!Player) return nullptr;

    //�������� �� ��������� ������
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

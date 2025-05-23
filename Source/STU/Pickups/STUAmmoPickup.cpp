// Shoot Them Up Game, All Right Reserved.

#include "STUAmmoPickup.h"
#include "STU/Character/STUHealthComponent.h"
#include "STU/Character/STUWeaponComponent.h"
#include "STU/Core/STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
    const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);
    if (!HealthComponent || HealthComponent->IsDead()) return false;

    const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(PlayerPawn);
    if (!WeaponComponent) return false;

    //UE_LOG(LogAmmoPickup, Display, TEXT("Ammo was taken"));
    return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}
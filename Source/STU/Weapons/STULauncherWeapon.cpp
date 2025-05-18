// Shoot Them Up Game, All Right Reserved.


#include "STULauncherWeapon.h"
#include "STUProjectile.h"
#include "Kismet/GameplayStatics.h"

void ASTULauncherWeapon::StartFire() 
{
    MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    auto Projectile = UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileClass, SpawnTransform);

    //set projectile params

    UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
}

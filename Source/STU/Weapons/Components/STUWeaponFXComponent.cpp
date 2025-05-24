// Shoot Them Up Game, All Right Reserved.

#include "STU/Weapons/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"

USTUWeaponFXComponent::USTUWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
    auto ImpactData = DefaultImpactData;

    if (Hit.PhysMaterial.IsValid())
    {
        //возвращает указатель на материал
        const auto PhysMat = Hit.PhysMaterial.Get();
        if (ImpactDataMap.Contains(PhysMat))
        {
            ImpactData = ImpactDataMap[PhysMat];
        }
    }

    //niagara
    UNiagaraFunctionLibrary::SpawnSystemAtLocation //
        (
            GetWorld(),                 //
            ImpactData.NiagaraEffect,   //
            Hit.ImpactPoint,            //
            Hit.ImpactNormal.Rotation() //
        );

    //decal

    //UGameplayStatics::SpawnDecalAttached
    auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation //
        (                                                        //
            GetWorld(),                                          //
            ImpactData.DecalData.Material,                       //
            ImpactData.DecalData.Size,                           //
            Hit.ImpactPoint,                                     //
            Hit.ImpactNormal.Rotation()                          //
        );
    if (DecalComponent)
    {
        DecalComponent->SetFadeOut               //
            (                                    //
                ImpactData.DecalData.LifeTime,   //
                ImpactData.DecalData.FadeOutTime //
            );
    }
}

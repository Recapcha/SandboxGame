// Shoot Them Up Game, All Right Reserved.

#include "STUEquipFinishAnimNotify.h"

void USTUEquipFinishAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    OnNotified.Broadcast(MeshComp);
    Super::Notify(MeshComp, Animation);
}
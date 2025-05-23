// Shoot Them Up Game, All Right Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUBasePickup.h"
#include "STUHealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class STU_API ASTUHealthPickup : public ASTUBasePickup
{
    GENERATED_BODY()

private:
    virtual bool GivePickupTo(APawn* PlayerPawn) override;
};

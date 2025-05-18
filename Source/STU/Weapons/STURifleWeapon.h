// Shoot Them Up Game, All Right Reserved.

#pragma once

#include "CoreMinimal.h"
#include "STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

UCLASS()
class STU_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    virtual void StartFire() override;
    virtual void StopFire() override;
    virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

protected:
    virtual void MakeShot() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TimeBetweenShot = 0.1f;

    //угол конуса, для разброса стрельбы из оружия
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BulletSpread = 1.5f;

private:
    FTimerHandle ShotTimerHandle;
};

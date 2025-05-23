// Shoot Them Up Game, All Right Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "STUAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnNotifiedSingature, USkeletalMeshComponent*);

UCLASS()
class STU_API USTUAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
		public:
      virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

      FOnNotifiedSingature OnNotified;
};

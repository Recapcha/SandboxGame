// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SandboxPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GAMEC_API ASandboxPlayerController : public APlayerController
{
	GENERATED_BODY()
	

protected:
	//переопределение класса управления
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:
	//Массив со всеми pawn по которым мы можем переключаться
	UPROPERTY()
	TArray<AActor*> Pawns;

	//инднекс текущего индекса pawn 
	int32 CurrentPawnIndex = 0;

	void ChangePawn();
};

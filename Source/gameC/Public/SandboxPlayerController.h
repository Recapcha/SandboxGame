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
	//��������������� ������ ����������
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

private:
	//������ �� ����� pawn �� ������� �� ����� �������������
	UPROPERTY()
	TArray<AActor*> Pawns;

	//������� �������� ������� pawn 
	int32 CurrentPawnIndex = 0;

	void ChangePawn();
};

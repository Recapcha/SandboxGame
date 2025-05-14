// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SandboxPawn.generated.h"

class UCameraComponent;

UCLASS()
class GAMEC_API ASandboxPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASandboxPawn();

	//создание переменной, корневой компонент
	//Содержит в себе только трансформацию в мире, к нему можно при атачить другие компоненты 
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneComponent;

	//добавление статик меша
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	//Скорость
	UPROPERTY(EditAnywhere)
	float Velocity = 300.0f;

	//виртуальные функции, получения контроля над pawn
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	//Вектор скорости pawn 
	//по умолчанию он нулевой
	FVector VelocityVector = FVector::ZeroVector;

	//Функции движения вперед назад, принимают параметр
	void MoveForward(float Amount);
	void MoveRight(float Amount);
};

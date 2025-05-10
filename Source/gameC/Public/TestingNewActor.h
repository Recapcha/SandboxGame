// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "TestingNewActor.generated.h"

UCLASS()
class GAMEC_API ATestingNewActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ATestingNewActor();

    //Добавление статик компонета статик меша к актору
    //Принимает по указателю на BaseMesh
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* BaseMesh;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    //чтобы указать что мы хотим видеть эти переменные в эдиторе класса
    //мы могли изменять их значение, как глазик для перменной мы указываем
    //так же тип int32 гарантирует безопасность комплияции под различные виды платформ

    //можем указать категорию

    //спецификаторы доступа
    //можно изменять везде как внутри так и снаружи
    UPROPERTY(EditAnywhere, Category = "Weapon")
    int32 WeaponsNum = 4;

    //можно изменять только внутри
    UPROPERTY(EditDefaultsOnly, Category = "Stat")
    int32 KillsNum = 7;

    //можно изменять только снаружи
    UPROPERTY(EditInstanceOnly, Category = "Health")
    float Health = 35.531;

    //можно изменять везде как внутри так и снаружи
    UPROPERTY(EditAnywhere, Category = "Health")
    bool IsDead = false;

    //виден везде, но не доступен для редактирования
    UPROPERTY(VisibleAnywhere, Category = "Health")
    bool HasWeapon = true;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:
    void printTypes();
    void printStringTypes();
};

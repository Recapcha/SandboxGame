// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestingNewActor.h"
#include "GeometryHubTestingActor.generated.h"

//настройки для изменения данных для каждого спавн элемента
USTRUCT(BlueprintType)
struct FGeometryPayloadTestingActor
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere)
    TSubclassOf<ATestingNewActor> GeometryClass;

    UPROPERTY(EditAnywhere)
    FGeometrydataTestingActor Data;

    UPROPERTY(EditAnywhere)
    FTransform InitialTransform;
};

UCLASS()
class STU_API AGeometryHubTestingActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AGeometryHubTestingActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    //TSubclassOf отфильтрует все классы движка и property можно будет
    //установить только сам класс или который от него наследуется
    //именно GeometryClass выбранный тут будет спавниться
    UPROPERTY(EditAnywhere)
    TSubclassOf<ATestingNewActor> GeometryClass;

    UPROPERTY(EditAnywhere)
    UClass* Class;

    UPROPERTY(EditAnywhere)
    ATestingNewActor* GeometryObject;

    //Тип массив объектов 
    //Принимает тип объектов в массиве 
    UPROPERTY(EditAnywhere)
    TArray<FGeometryPayloadTestingActor> GeometryPayloads;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:
    ATestingNewActor* NonePropertyActor;

    UPROPERTY()
    ATestingNewActor* PropertyActor;


    void SpawnActors1();
    void SpawnActors2();
    void SpawnActors3();
    void SpawnActors4();

    //Функция на срабатывание делегатов
    //Функции которые мы добавляем к динамическим дилегатам, должны быть помечены UFUNCTION
    UFUNCTION()
    void OnColorChanged(const FLinearColor& Color, const FString& Name);

    void OnTimerFinished(AActor* Actor);
};

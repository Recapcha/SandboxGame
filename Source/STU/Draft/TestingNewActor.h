// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "TestingNewActor.generated.h"

//объявление делегата
//Все делегаты назавния объявляются с большой буквы F
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnColorChangedTestingNewActor, const FLinearColor&, Color, const FString&, Name);

//так как этот делегат только в с++, мы можем не указывать имя актора в блюпринтах
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimerFinishedTestingNewActor, AActor*);

//Класс ENUM отвечающий за тип движения
//UENUM это макрос который расширяет возможности enum и позволяет выводить его в едитор
//Все enum должны начинать с заглавной буквы E , EMovementType
UENUM(BlueprintType)
enum class EMovementTypeTestingActor : uint8
{
    SIN,
    STATIC
};

//Все типы структур начинаются с F
//структура тот же класс, но все поля public
//структура при наследовании имеет все поля как public
//можно объединять переменные для их удобной группировки
//
//BlueprintReadWrite, позволяет получать доступ к переменные на blueprint graph
USTRUCT(BlueprintType)
struct FGeometrydataTestingActor
{
    GENERATED_USTRUCT_BODY()

    //Две переменные, отвечающие за амплитуду и частоту колебаний для логики движения объектов
    UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Movement")
    float Amplitude = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Frequency = 2.0f;

    //Создание переменной MoveType типа enum MovementType
    //который содержит переходные значия STATIC, SIN
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    EMovementTypeTestingActor MoveType = EMovementTypeTestingActor::STATIC;

    //Цвет меша
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
    FLinearColor Color = FLinearColor::Black;

    //Таймер 
    UPROPERTY(EditAnywhere, Category = "Design")
    float TimerRate = 3.0f;
};

UCLASS()
class STU_API ATestingNewActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ATestingNewActor();

    //Добавление статик компонета статик меша к актору
    //Принимает по указателю на BaseMesh
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* BaseMesh;

    //Функция для изменения GeometryData 
    void SetGeometryData(const FGeometrydataTestingActor& Data) { GeometryData = Data; }

    //позволяет вызвать функцию из блюпринт графа 
    UFUNCTION(BlueprintCallable)
    FGeometrydataTestingActor GetGeometryData() const {return GeometryData;}

    //создание делегатов
    //так же что можно было подписаться на него в блюпринтах
    UPROPERTY(BlueprintAssignable)
    FOnColorChangedTestingNewActor OnColorChanged;

    FOnTimerFinishedTestingNewActor OnTimerFinished;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    //Евент, если изменяется что то в едиторе то запускается этот евент
    //В данном случае к моему цвету применяется другой цвет
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    //функция которая срабатывает при создании 
    //Аналог Construction Script 
    virtual void OnConstruction(const FTransform& Transform) override;

    //виртуальная функция конца 
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    //чтобы указать что мы хотим видеть эти переменные в эдиторе класса
    //мы могли изменять их значение, как глазик для перменной мы указываем
    //так же тип int32 гарантирует безопасность комплияции под различные виды платформ

    //ЭТО ВСЕ СОЗДАННЫЕ ПЕРЕМЕННЫЕ, КОТОРЫЕ ДОСТУПНЫ ДЛЯ ДЕТЕЙ ЭТОГО КЛАССА

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

    //переменная хранящая в себе структуру
    //В ней хранятся для вывода другие переменные
    UPROPERTY(EditAnywhere,BlueprintReadWrite,  Category = "GeometryData")
    FGeometrydataTestingActor GeometryData;

public:
    // Called every frame
    virtual void
    Tick(float DeltaTime) override;

private:
    //закрытые переменные
    //Кешшированная переменная, которая содержит начальное положение актора
    FVector InitialLocation;

    //Создание перменной класса таймера
    FTimerHandle TimerHandle;

    //количетсво раз сколько сработать до конца 
    const int32 MaxTimerCount = 5;

    //счетчик срабатывания 
    int32 TimerCount = 0;

    void PrintTypes();
    void PrintStringTypes();
    void PrintTransform();
    void HandleMovement();
    void SetColor(const FLinearColor& Color);
    void OnTimerFired();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "TestingNewActor.generated.h"

//���������� ��������
//��� �������� �������� ����������� � ������� ����� F
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnColorChangedTestingNewActor, const FLinearColor&, Color, const FString&, Name);

//��� ��� ���� ������� ������ � �++, �� ����� �� ��������� ��� ������ � ����������
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimerFinishedTestingNewActor, AActor*);

//����� ENUM ���������� �� ��� ��������
//UENUM ��� ������ ������� ��������� ����������� enum � ��������� �������� ��� � ������
//��� enum ������ �������� � ��������� ����� E , EMovementType
UENUM(BlueprintType)
enum class EMovementTypeTestingActor : uint8
{
    SIN,
    STATIC
};

//��� ���� �������� ���������� � F
//��������� ��� �� �����, �� ��� ���� public
//��������� ��� ������������ ����� ��� ���� ��� public
//����� ���������� ���������� ��� �� ������� �����������
//
//BlueprintReadWrite, ��������� �������� ������ � ���������� �� blueprint graph
USTRUCT(BlueprintType)
struct FGeometrydataTestingActor
{
    GENERATED_USTRUCT_BODY()

    //��� ����������, ���������� �� ��������� � ������� ��������� ��� ������ �������� ��������
    UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Movement")
    float Amplitude = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Frequency = 2.0f;

    //�������� ���������� MoveType ���� enum MovementType
    //������� �������� ���������� ������ STATIC, SIN
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    EMovementTypeTestingActor MoveType = EMovementTypeTestingActor::STATIC;

    //���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Design")
    FLinearColor Color = FLinearColor::Black;

    //������ 
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

    //���������� ������ ��������� ������ ���� � ������
    //��������� �� ��������� �� BaseMesh
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* BaseMesh;

    //������� ��� ��������� GeometryData 
    void SetGeometryData(const FGeometrydataTestingActor& Data) { GeometryData = Data; }

    //��������� ������� ������� �� �������� ����� 
    UFUNCTION(BlueprintCallable)
    FGeometrydataTestingActor GetGeometryData() const {return GeometryData;}

    //�������� ���������
    //��� �� ��� ����� ���� ����������� �� ���� � ����������
    UPROPERTY(BlueprintAssignable)
    FOnColorChangedTestingNewActor OnColorChanged;

    FOnTimerFinishedTestingNewActor OnTimerFinished;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    //�����, ���� ���������� ��� �� � ������� �� ����������� ���� �����
    //� ������ ������ � ����� ����� ����������� ������ ����
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

    //������� ������� ����������� ��� �������� 
    //������ Construction Script 
    virtual void OnConstruction(const FTransform& Transform) override;

    //����������� ������� ����� 
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    //����� ������� ��� �� ����� ������ ��� ���������� � ������� ������
    //�� ����� �������� �� ��������, ��� ������ ��� ��������� �� ���������
    //��� �� ��� int32 ����������� ������������ ���������� ��� ��������� ���� ��������

    //��� ��� ��������� ����������, ������� �������� ��� ����� ����� ������

    //����� ������� ���������

    //������������� �������
    //����� �������� ����� ��� ������ ��� � �������
    UPROPERTY(EditAnywhere, Category = "Weapon")
    int32 WeaponsNum = 4;

    //����� �������� ������ ������
    UPROPERTY(EditDefaultsOnly, Category = "Stat")
    int32 KillsNum = 7;

    //����� �������� ������ �������
    UPROPERTY(EditInstanceOnly, Category = "Health")
    float Health = 35.531;

    //����� �������� ����� ��� ������ ��� � �������
    UPROPERTY(EditAnywhere, Category = "Health")
    bool IsDead = false;

    //����� �����, �� �� �������� ��� ��������������
    UPROPERTY(VisibleAnywhere, Category = "Health")
    bool HasWeapon = true;

    //���������� �������� � ���� ���������
    //� ��� �������� ��� ������ ������ ����������
    UPROPERTY(EditAnywhere,BlueprintReadWrite,  Category = "GeometryData")
    FGeometrydataTestingActor GeometryData;

public:
    // Called every frame
    virtual void
    Tick(float DeltaTime) override;

private:
    //�������� ����������
    //������������� ����������, ������� �������� ��������� ��������� ������
    FVector InitialLocation;

    //�������� ��������� ������ �������
    FTimerHandle TimerHandle;

    //���������� ��� ������� ��������� �� ����� 
    const int32 MaxTimerCount = 5;

    //������� ������������ 
    int32 TimerCount = 0;

    void PrintTypes();
    void PrintStringTypes();
    void PrintTransform();
    void HandleMovement();
    void SetColor(const FLinearColor& Color);
    void OnTimerFired();
};

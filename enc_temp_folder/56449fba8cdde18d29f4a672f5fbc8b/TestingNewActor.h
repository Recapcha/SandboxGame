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

    //���������� ������ ��������� ������ ���� � ������
    //��������� �� ��������� �� BaseMesh
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* BaseMesh;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    //����� ������� ��� �� ����� ������ ��� ���������� � ������� ������
    //�� ����� �������� �� ��������, ��� ������ ��� ��������� �� ���������
    //��� �� ��� int32 ����������� ������������ ���������� ��� ��������� ���� ��������

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

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:
    void printTypes();
    void printStringTypes();
};

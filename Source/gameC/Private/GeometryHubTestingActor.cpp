// Fill out your copyright notice in the Description page of Project Settings.

#include "GeometryHubTestingActor.h"
#include "Engine/World.h"

// Sets default values
AGeometryHubTestingActor::AGeometryHubTestingActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AGeometryHubTestingActor::BeginPlay()
{
    Super::BeginPlay();

    //��������� �� ��� ����, � ��� ��������� ������� ������
    //GetWorld() - ���������� ��������� �� ���������� ������ ���� ����
    UWorld* World = GetWorld();
    if (World)
    {
        for (int32 i = 0; i < 10; i++)
        {
            //������������� ������
            //�������, ���������, ������
            const FTransform GeometryTransform = FTransform(FRotator::ZeroRotator, FVector(0.0f, 300.0f * i, 300.0f));

            //����� �������
            //� ���������� ������ �� ����� �������� ������ ������
            //�������, ������� � ��� ������
            //World->SpawnActor(GeometryClass);

            //������� ���������� �����, ��� ����� ����� ����� ��������� �� ������ ������ ����
            //�� ���� �� ���� ��������� ���� �� AActor � ATestingNewActor
            //��� �� �������� ������������� ������
            ATestingNewActor* Geometry = World->SpawnActor<ATestingNewActor>(GeometryClass, GeometryTransform);

            //�������� �� ����������, ����� �� �� ������ ����� ������ Geometry. ����� ����� ������
            if (Geometry)
            {
                FGeometrydataTestingActor Data;
                Data.MoveType = FMath::RandBool() ? EMovementTypeTestingActor::STATIC : EMovementTypeTestingActor::SIN;
                Geometry->SetGeometryData(Data);
            }
        }

        //��� 10 ���������
        for (int32 i = 0; i < 10; i++)
        {
            //��������� ��������� �� ���������
            const FTransform GeometryTransform = FTransform(FRotator::ZeroRotator, FVector(0.0f, 300.0f * i, 700.0f));

            //SpawnActorDeferred
            //���� ����� ���������� ��� ��� ��� �������� ������, ����� ������ ������ �����������,
            //� ����� ���� �� ����� ������
            ATestingNewActor* Geometry = World->SpawnActorDeferred<ATestingNewActor>(GeometryClass, GeometryTransform);

            if (Geometry)
            {
                FGeometrydataTestingActor Data;
                Data.Color = FLinearColor::MakeRandomColor();
                Geometry->SetGeometryData(Data);

                //������ ������ ����� ����
                Geometry->FinishSpawning(GeometryTransform);
            }
        }

        //��� ���� �������� ����������� � ������� GeometryPayloads
        //��������� ���������� ���������� (const FGeometryPayloadTestingActor Payload) �����
        //�� ��������� GeometryPayloads (���������), �� ������� ����� ����������� ��������

        for (const FGeometryPayloadTestingActor Payload : GeometryPayloads)
        {
            //World->SpawnActorDeferred<ATestingNewActor>(...)
            //
            //World : ��� ��������� �� ������� ������� ���(UWorld*).
            //������ ���������� ����� GetWorld().
            //��� �������� �� ���������� ����� �������� �� ������.
            //
            //SpawnActorDeferred : ��� ����������� ������� ��� ������(��������) ������,
            //������� ����������� ��������� ����� ��� �������������.
            //� ������� �� SpawnActor, SpawnActorDeferred �� �������� ����� BeginPlay()
            //� ������ ������� �������������, ��������� � ������ ���������� ������ � ����.
            //
            //<ATestingNewActor> : ��� ��������� ��������, ����������� ��������� ��� ������������� ��������.
            //�� ����, �� �������, ��� ����� ���������� ������, ������� �����(��� ����� ��������� �)
            //����� ATestingNewActor, � �� ����� �������� ��������� ������ ����� ����.
            //
            //Payload.GeometryClass : ��� ��������, ���������� ����� ������, ������� ����� �������.
            //�� ������ ���� ���� TSubclassOf<AActor>(��� ����� �����������, �������� TSubclassOf<ATestingNewActor>).
            //��� UClass * ������� ��� ������ � Payload.��� ��������� �������� ������ ������ �������,
            //���� Payload.GeometryClass ����� ��������� ������ ������.
            //
            //Payload.InitialTransform : ��� ��������, ���������� ��������� �������������(���������, �������, �������)
            //��� ������������ ������.��� FTransform.ATestingNewActor* Geometry = ... :
            //��������� SpawnActorDeferred ������������� ��������� Geometry ���� ATestingNewActor*.

            //����� ����� SpawnActorDeferred ?
            //�� ������������,����� ���� ����� ��������� ������(���������� ��� ��������,
            //������� ����� - �� ������������� ������� �������������) �� ����,
            //��� �� ��������� �������� � ���� � ������ ���� ������� (�� ������ BeginPlay()).
            //���� �� �� ����������� ������� SpawnActor,
            //�� BeginPlay() ��� �� ������ ����� ����� ������������,
            //� � ���� �� ���� �� ����������� ��� - �� ��������� ����� ����.

            ATestingNewActor* Geometry = World->SpawnActorDeferred<ATestingNewActor>(Payload.GeometryClass, Payload.InitialTransform);

            if (Geometry)
            {
                //�� �������� � ���� ����� �����-�� ������ �� ����� ��������� Payload.
                Geometry->SetGeometryData(Payload.Data);

                //������ �����, ����� SpawnActorDeferred � FinishSpawning,
                //�� ������ ��������� �������� ������� � ������ ������� ��� ��������� ������ ������.
                //�� ���� ����� BeginPlay() ��� �� ��� ������.

                //��� ��� ������:
                //��������� ������� ������������� ������.
                //������������ ������ � ����.�������� PostActorCreated()(���� �������������).
                //�������� ExecuteConstruction()(��� ������� OnConstruction() � C++ � Construction Script � Blueprint).
                //�������� PostInitializeComponents().
                //�������� BeginPlay().
                //����� ������ ���� ������� ����� ��������� ��������� ������ � �������� � ����.
                //
                //Payload.InitialTransform: � FinishSpawning ����� ���������� �������������.
                Geometry->FinishSpawning(Payload.InitialTransform);
            }
        }
    }
}

// Called every frame
void AGeometryHubTestingActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

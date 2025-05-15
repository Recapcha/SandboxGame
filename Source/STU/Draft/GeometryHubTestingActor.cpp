// Fill out your copyright notice in the Description page of Project Settings.

#include "GeometryHubTestingActor.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogHubActor, All, All);

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

    //SpawnActors1();
    //SpawnActors2();
    //SpawnActors3();
    SpawnActors4();
}

// Called every frame
void AGeometryHubTestingActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //is valid �������� is pending kill, ����� �� �������� �������
    UE_LOG(LogHubActor, Warning, TEXT("property pointer %i, is valid %i"), PropertyActor != nullptr, IsValid(PropertyActor));
    UE_LOG(LogHubActor, Error, TEXT("none property pointer %i, is valid %i"), NonePropertyActor != nullptr, IsValid(PropertyActor));
}

void AGeometryHubTestingActor::SpawnActors1()
{
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
    }
}

void AGeometryHubTestingActor::SpawnActors2()
{
    //��������� �� ��� ����, � ��� ��������� ������� ������
    //GetWorld() - ���������� ��������� �� ���������� ������ ���� ����
    UWorld* World = GetWorld();
    if (World)
    {
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
    }
}

void AGeometryHubTestingActor::SpawnActors3()
{
    //��������� �� ��� ����, � ��� ��������� ������� ������
    //GetWorld() - ���������� ��������� �� ���������� ������ ���� ����
    UWorld* World = GetWorld();
    if (World)
    {

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

                Geometry->OnColorChanged.AddDynamic(this, &AGeometryHubTestingActor::OnColorChanged);
                Geometry->OnTimerFinished.AddUObject(this, &AGeometryHubTestingActor::OnTimerFinished);

                //Payload.InitialTransform: � FinishSpawning ����� ���������� �������������.
                Geometry->FinishSpawning(Payload.InitialTransform);
            }
        }
    }
}

void AGeometryHubTestingActor::SpawnActors4()
{
    if (!GetWorld()) return;

    FTransform GeometryTransform = FTransform(FRotator::ZeroRotator, FVector(700.0f, 300.0f, 300.0f));
    NonePropertyActor = GetWorld()->SpawnActor<ATestingNewActor>(GeometryClass, GeometryTransform);
    
    GeometryTransform = FTransform(FRotator::ZeroRotator, FVector(700.0f, 700.0f, 300.0f));
    PropertyActor = GetWorld()->SpawnActor<ATestingNewActor>(GeometryClass, GeometryTransform);
}

//������� ��� ������������ ���������
void AGeometryHubTestingActor::OnColorChanged(const FLinearColor& Color, const FString& Name)
{
    UE_LOG(LogHubActor, Warning, TEXT("Actor name: %s Color: %s"), *Name, *Color.ToString());
}

void AGeometryHubTestingActor::OnTimerFinished(AActor* Actor)
{
    if (!Actor) return;
    UE_LOG(LogHubActor, Error, TEXT("Timer finished: %s"), *Actor->GetName());

    //�����������, ����������� ��������� �� ������� ����� �� ��������� �� ����� ����������
    ATestingNewActor* Geometry = Cast<ATestingNewActor>(Actor);
    if (!Geometry) return;

    UE_LOG(LogHubActor, Error, TEXT("Cast success, amplitude %f"), Geometry->GetGeometryData().Amplitude);

    //�������� ������ �� �����
    Geometry->Destroy();

    //����� 2 ��� ��������� destroy
    //Geometry->SetLifeSpan(2.0f);
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingNewActor.h"
#include "Engine/Engine.h"

DEFINE_LOG_CATEGORY_STATIC(LogForTestingNewActor, All, All);

// Sets default values
ATestingNewActor::ATestingNewActor()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //������������ ���������� BaseMesh ���� UStaticMeshComponent �� ��������� �������, ������� ��������� ��� (������)
    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");

    //������� root ��� ������
    //��� �������, ������� ��������� � ���� ����������, ������ ������� ������� �� �����
    SetRootComponent(BaseMesh);
}

// Called when the game starts or when spawned
void ATestingNewActor::BeginPlay()
{
    Super::BeginPlay();

    //������� ���������� ���� FTransform � ����������� ���� ������������ �������� ������� GetActorTransform()
    FTransform Transform = GetActorTransform();

    //������������� �������� XYZ ����������
    FVector Location = Transform.GetLocation();
    FRotator Rotator = Transform.Rotator();
    FVector Scale = Transform.GetScale3D();

    //�������� ������ 
    UE_LOG(LogForTestingNewActor, Warning, TEXT("Actor name is %s"), *GetName());

    //Transform
    UE_LOG(LogForTestingNewActor, Warning, TEXT("Transform %s"), *Transform.ToString());

    //XYZ
    UE_LOG(LogForTestingNewActor, Warning, TEXT("Location %s"), *Location.ToString());
    UE_LOG(LogForTestingNewActor, Warning, TEXT("Rotator %s"), *Rotator.ToString());
    UE_LOG(LogForTestingNewActor, Warning, TEXT("Scale %s"), *Scale.ToString());

    //����� ������������� � ��������� ����� Transform 
    UE_LOG(LogForTestingNewActor, Error, TEXT("Human transform %s"), *Transform.ToHumanReadableString());

    //printStringTypes();
    //printTypes();
}

// Called every frame
void ATestingNewActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ATestingNewActor::printTypes()
{
    //����� ����������� ��������� �����������
    UE_LOG(LogForTestingNewActor, Display, TEXT("info"));

    //UE_LOG(LogTemp, Display, TEXT("Actor name is "));

    ////GetName - ���������� ������� � Actor, �������� �� �������� ������������, ����� �� �� ���������
    //UE_LOG(LogForTestingNewActor, Warning, TEXT("Actor name %s"), *GetName());

    ////LogTemp ��� ������� ��������� �����������
    //UE_LOG(LogTemp, Display, TEXT("Hello Unreal!"));
    //UE_LOG(LogTemp, Warning, TEXT("Hello Unreal!"));
    //UE_LOG(LogTemp, Error, TEXT("Hello Unreal!"));

    //���������� ��������� � protected .h ������������ ����, ������� � ��� ��������
    //int WeaponsNum = 4;
    //int KillsNum = 7;
    //float Health = 35.531;
    //bool IsDead = false;
    //bool HasWeapon = true;

    //UE_LOG(LogTemp, Display, TEXT("WeaponsNum "), WeaponsNum);
    //UE_LOG(LogTemp, Display, TEXT("KillsNum "), KillsNum);
    //UE_LOG(LogTemp, Display, TEXT("Health "), Health);
    //UE_LOG(LogTemp, Display, TEXT("Health "), Health);
    //UE_LOG(LogTemp, Display, TEXT("IsDead "), IsDead);
    //UE_LOG(LogTemp, Display, TEXT("HasWeapon "), static_cast<int>(HasWeapon));

    //UE_LOG(LogTemp, Display, TEXT("------------------------------"));

    //������������ �������, ��� ��� ����� �� ���������
    UE_LOG(LogForTestingNewActor, Display, TEXT("WeaponsNum %d "), WeaponsNum);                //int
    UE_LOG(LogForTestingNewActor, Display, TEXT("KillsNum %i"), KillsNum);                     //int
    UE_LOG(LogForTestingNewActor, Display, TEXT("Health %f"), Health);                         //float
    UE_LOG(LogForTestingNewActor, Display, TEXT("Health %.2f"), Health);                       //float � ������������ ����� .
    UE_LOG(LogForTestingNewActor, Display, TEXT("IsDead %d"), IsDead);                         //bool
    UE_LOG(LogForTestingNewActor, Display, TEXT("HasWeapon %d"), static_cast<int>(HasWeapon)); //bool ����������� � int

    UE_LOG(LogTemp, Warning, TEXT("Testing Log"));
}

void ATestingNewActor::printStringTypes()
{
    FString Name = "JohnConnor";
    UE_LOG(LogForTestingNewActor, Warning, TEXT("Name: %s"), *Name);
    UE_LOG(LogTemp, Warning, TEXT("Hello Unreal!"));

    //int WeaponsNum = 4;
    //float Health = 34.1535633;
    //bool IsDead = false;

    //�������������� � ���� FString ������� ��������
    FString WeaponsNumStr = "Weapons num = " + FString::FromInt(WeaponsNum);
    FString HealthStr = "Health = " + FString::SanitizeFloat(Health);
    FString IsDeadStr = "Is dead = " + FString(IsDead ? "true" : "false");

    //%s - ������������ ������ �����
    //����� �� ��������� �� ����������, ������� ������ ��������������� � FString �����
    FString Stat = FString::Printf(TEXT(" \n == All Stat == \n %s \n %s \n %s"), *WeaponsNumStr, *HealthStr, *IsDeadStr);
    UE_LOG(LogForTestingNewActor, Warning, TEXT("%s"), *Stat);

    //����� ��������� �� �����
    //-1 � ��������� ��� ����� ������������� ��������� �� �����,
    //���� ��������� ��������� � ����������� ������� �������� �� ��������� �� �����
    //3.0f �����
    //���� � ���, ��������� �������� ��� ���������. ��� �� � ��������� ������� ���������� �������� ������
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Name);
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Stat, true, FVector2D(1.5f, 1.5f));
}

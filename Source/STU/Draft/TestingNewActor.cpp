// Fill out your copyright notice in the Description page of Project Settings.

#include "TestingNewActor.h"
#include "Engine/Engine.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "TimerManager.h"

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

    //��� ������� ��������� ������� ������
    //��� �� ����� ������� ����� Transform.GetLocation()
    //����������� ��� �������� ������� ����
    InitialLocation = GetActorLocation();

    //printStringTypes();
    //printTypes();
    //SetColor(GeometryData.Color);

    //����� ������� ��������� ������� � ��������� ��������
    //���������� ������� OnTimerFired ��� ������� ������� �������� �� ����� ������� 
    //true - ������ ��������� ���� ��� � ����������� 
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ATestingNewActor::OnTimerFired, GeometryData.TimerRate, true);

}

// Called every frame
void ATestingNewActor::Tick(float DeltaTime)
{
    //������ � Tick
    Super::Tick(DeltaTime);

    HandleMovement();
}

void ATestingNewActor::PrintTypes()
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

void ATestingNewActor::PrintStringTypes()
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
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Name);
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Stat, true, FVector2D(1.5f, 1.5f));
    }
}

void ATestingNewActor::PrintTransform()
{
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
}

void ATestingNewActor::HandleMovement()
{
    //UE_LOG(LogForTestingNewActor, Warning, TEXT("HandleMovement"));

    switch (GeometryData.MoveType)
    {
    case EMovementTypeTestingActor::SIN:
    {
        //� ���������� CurrentLocation, �������� �������. ���� FVector �����������
        FVector CurrentLocation = GetActorLocation();
        if (GetWorld())
        {
            float Time = GetWorld()->GetTimeSeconds();
            CurrentLocation.Z = InitialLocation.Z + GeometryData.Amplitude * FMath::Sin(GeometryData.Frequency * Time);
            SetActorLocation(CurrentLocation);
        }
    }
    break;
    case EMovementTypeTestingActor::STATIC:
        break;
    default:
        break;
    }
}

void ATestingNewActor::SetColor(const FLinearColor& Color)
{
    if (BaseMesh)
    {
        //�������� ���������
        //� ���������
        UMaterialInstanceDynamic* DynMaterial = BaseMesh->CreateAndSetMaterialInstanceDynamic(0);
        if (DynMaterial)
        {
            //� ��������� ���������� ���� � ��� ��������� Color
            //��� ������ ���� �������� �� ������, ���� �������� ��������� �� ��� ������� ��� ����
            //�������� ���������� �������� �� �����, �� ��������� ������ �� ���������
            //FLinearColor - 32 ������ ����, FColor - 8���
            DynMaterial->SetVectorParameterValue("Color", Color);
        }
    }
}

void ATestingNewActor::OnTimerFired()
{
    if (++TimerCount <= MaxTimerCount)
    {
        const FLinearColor NewColor = FLinearColor::MakeRandomColor();

        //TimerCount ���������� �� ��������, ������ ��� ������������ %i ������� �������� ���� int.
        //*NewColor.ToString() ���������� ��� ���������, ������ ��� ������������ % s ������� ��������� �� ������ ���� const TCHAR *.
        UE_LOG(LogForTestingNewActor, Display, TEXT("TimerCount: %i, Color to set  up: %s"), TimerCount, *NewColor.ToString());
        SetColor(NewColor);
        
        //����� ��������, ������� "� ��������!"
        //����� ��� ���� ��� �� �������� ��������� ��� ��� �������� ������� 
        //�������� ������ �� ���� 
        OnColorChanged.Broadcast(NewColor, GetName());
    }
    else
    {
        UE_LOG(LogForTestingNewActor, Display, TEXT("Timer has been stopped!"));

        //��������� �������, ����� ������ ����������
        GetWorldTimerManager().ClearTimer(TimerHandle);

        //����� ������� ��������, ����� ������ �������� ������
        OnTimerFinished.Broadcast(this);
    }
}

void ATestingNewActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    //SetColor(GeometryData.Color = FLinearColor::Black);
}

void ATestingNewActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UE_LOG(LogForTestingNewActor, Error, TEXT("Actor has been destroyed"));
    Super::EndPlay(EndPlayReason);
}

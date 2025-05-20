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

    //Присваивание переменной BaseMesh типа UStaticMeshComponent из шаблонной функции, которое принимает имя (легкое)
    BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>("BaseMesh");

    //Создает root для актора
    //Это функция, которая принимает в себя переменную, объект который возьмет за центр
    SetRootComponent(BaseMesh);
}

// Called when the game starts or when spawned
void ATestingNewActor::BeginPlay()
{
    Super::BeginPlay();

    //доп функция получения локации актора
    //так же можно сделать через Transform.GetLocation()
    //Присваиваем при создании запуске игры
    InitialLocation = GetActorLocation();

    //printStringTypes();
    //printTypes();
    //SetColor(GeometryData.Color);

    //вызов функции установки таймера с передачей значений
    //переданная функция OnTimerFired это функция которая работает во время таймера 
    //true - таймер срабатает один раз и остановится 
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ATestingNewActor::OnTimerFired, GeometryData.TimerRate, true);

}

// Called every frame
void ATestingNewActor::Tick(float DeltaTime)
{
    //Работа в Tick
    Super::Tick(DeltaTime);

    HandleMovement();
}

void ATestingNewActor::PrintTypes()
{
    //вывод собственной категории логирования
    UE_LOG(LogForTestingNewActor, Display, TEXT("info"));

    //UE_LOG(LogTemp, Display, TEXT("Actor name is "));

    ////GetName - встроенная функция у Actor, которого мы являемся наследниками, берем ее по указателю
    //UE_LOG(LogForTestingNewActor, Warning, TEXT("Actor name %s"), *GetName());

    ////LogTemp это готовая категория логирования
    //UE_LOG(LogTemp, Display, TEXT("Hello Unreal!"));
    //UE_LOG(LogTemp, Warning, TEXT("Hello Unreal!"));
    //UE_LOG(LogTemp, Error, TEXT("Hello Unreal!"));

    //Переменные добавлены в protected .h заголовочный файл, поэтому и так доступны
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

    //Спецификатор доступа, без них числа не выведутся
    UE_LOG(LogForTestingNewActor, Display, TEXT("WeaponsNum %d "), WeaponsNum);                //int
    UE_LOG(LogForTestingNewActor, Display, TEXT("KillsNum %i"), KillsNum);                     //int
    UE_LOG(LogForTestingNewActor, Display, TEXT("Health %f"), Health);                         //float
    UE_LOG(LogForTestingNewActor, Display, TEXT("Health %.2f"), Health);                       //float с ограничением после .
    UE_LOG(LogForTestingNewActor, Display, TEXT("IsDead %d"), IsDead);                         //bool
    UE_LOG(LogForTestingNewActor, Display, TEXT("HasWeapon %d"), static_cast<int>(HasWeapon)); //bool конвертация в int

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

    //преобразование к типу FString булевых значений
    FString WeaponsNumStr = "Weapons num = " + FString::FromInt(WeaponsNum);
    FString HealthStr = "Health = " + FString::SanitizeFloat(Health);
    FString IsDeadStr = "Is dead = " + FString(IsDead ? "true" : "false");

    //%s - модификаторы вывода чисел
    //вывод по указателю на переменные, которые хранят преобразованных в FString числа
    FString Stat = FString::Printf(TEXT(" \n == All Stat == \n %s \n %s \n %s"), *WeaponsNumStr, *HealthStr, *IsDeadStr);
    UE_LOG(LogForTestingNewActor, Warning, TEXT("%s"), *Stat);

    //вывод сообщения на экран
    //-1 у сообщения нет ключа гарантировано выведется на экран,
    //ключ позволяет сообщения с одинаковыми ключами повторно не выводится на экран
    //3.0f время
    //цвет и имя, следующии остаются без изменения. Так же в последнем имеется возможнсть поменять размер
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, Name);
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, Stat, true, FVector2D(1.5f, 1.5f));
    }
}

void ATestingNewActor::PrintTransform()
{
    //создаем переменную типа FTransform и присваиваем туда возвращаемое значение функции GetActorTransform()
    FTransform Transform = GetActorTransform();

    //Присваиваение основных XYZ переменных
    FVector Location = Transform.GetLocation();
    FRotator Rotator = Transform.Rotator();
    FVector Scale = Transform.GetScale3D();

    //название актора
    UE_LOG(LogForTestingNewActor, Warning, TEXT("Actor name is %s"), *GetName());

    //Transform
    UE_LOG(LogForTestingNewActor, Warning, TEXT("Transform %s"), *Transform.ToString());

    //XYZ
    UE_LOG(LogForTestingNewActor, Warning, TEXT("Location %s"), *Location.ToString());
    UE_LOG(LogForTestingNewActor, Warning, TEXT("Rotator %s"), *Rotator.ToString());
    UE_LOG(LogForTestingNewActor, Warning, TEXT("Scale %s"), *Scale.ToString());

    //Более информативный и подробный показ Transform
    UE_LOG(LogForTestingNewActor, Error, TEXT("Human transform %s"), *Transform.ToHumanReadableString());
}

void ATestingNewActor::HandleMovement()
{
    //UE_LOG(LogForTestingNewActor, Warning, TEXT("HandleMovement"));

    switch (GeometryData.MoveType)
    {
    case EMovementTypeTestingActor::SIN:
    {
        //В переменную CurrentLocation, текующей позиции. типа FVector присваиваем
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
        //Создание материала
        //В указатель
        UMaterialInstanceDynamic* DynMaterial = BaseMesh->CreateAndSetMaterialInstanceDynamic(0);
        if (DynMaterial)
        {
            //У материала выставляем цвет в его параметре Color
            //при старте цвет изменить на желтый, если параметр совпадает он сам изменит ему цвет
            //выбирать конкретный материал не нужно, он реагирует только на параметры
            //FLinearColor - 32 битный цвет, FColor - 8бит
            DynMaterial->SetVectorParameterValue("Color", Color);
        }
    }
}

void ATestingNewActor::OnTimerFired()
{
    if (++TimerCount <= MaxTimerCount)
    {
        const FLinearColor NewColor = FLinearColor::MakeRandomColor();

        //TimerCount передается по значению, потому что спецификатор %i требует значение типа int.
        //*NewColor.ToString() передается как указатель, потому что спецификатор % s требует указатель на строку типа const TCHAR *.
        UE_LOG(LogForTestingNewActor, Display, TEXT("TimerCount: %i, Color to set  up: %s"), TimerCount, *NewColor.ToString());
        SetColor(NewColor);
        
        //Вызов делегата, говорит "Я Сработал!"
        //Когда где либо кто то получает сообщение что тут сработал делегат 
        //начинает логика из него 
        OnColorChanged.Broadcast(NewColor, GetName());
    }
    else
    {
        UE_LOG(LogForTestingNewActor, Display, TEXT("Timer has been stopped!"));

        //остановка таймера, какой таймер остановить
        GetWorldTimerManager().ClearTimer(TimerHandle);

        //Вызов второго делегата, когда таймер закончил работу
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

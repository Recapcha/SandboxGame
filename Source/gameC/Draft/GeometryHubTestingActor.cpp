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

    //is valid вызывает is pending kill, когда мы вызываем дестрой
    UE_LOG(LogHubActor, Warning, TEXT("property pointer %i, is valid %i"), PropertyActor != nullptr, IsValid(PropertyActor));
    UE_LOG(LogHubActor, Error, TEXT("none property pointer %i, is valid %i"), NonePropertyActor != nullptr, IsValid(PropertyActor));
}

void AGeometryHubTestingActor::SpawnActors1()
{
    //указатель на мир игры, в нем находится функция спавна
    //GetWorld() - возвращает указатель на глобальный объект мира игры
    UWorld* World = GetWorld();
    if (World)
    {
        for (int32 i = 0; i < 10; i++)
        {
            //трансформация актора
            //поворот, положение, размер
            const FTransform GeometryTransform = FTransform(FRotator::ZeroRotator, FVector(0.0f, 300.0f * i, 300.0f));

            //спавн объекта
            //в параметрах спавна он может получать объект спавна
            //локацию, ротацию и доп данные
            //World->SpawnActor(GeometryClass);

            //создаем приведении типов, что могли сразу иметь указатель на объект нашего типа
            //то есть не надо приводить каст от AActor к ATestingNewActor
            //так же передаем трансформацию актора
            ATestingNewActor* Geometry = World->SpawnActor<ATestingNewActor>(GeometryClass, GeometryTransform);

            //проверка на валидность, можем ли мы вообще взять объект Geometry. иначе будет ошибка
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
    //указатель на мир игры, в нем находится функция спавна
    //GetWorld() - возвращает указатель на глобальный объект мира игры
    UWorld* World = GetWorld();
    if (World)
    {
        //для 10 элементов
        for (int32 i = 0; i < 10; i++)
        {
            //настройки положения по умолчанию
            const FTransform GeometryTransform = FTransform(FRotator::ZeroRotator, FVector(0.0f, 300.0f * i, 700.0f));

            //SpawnActorDeferred
            //этот спавн отличается тем что при создании актора, будет вызван только констурктор,
            //а бегин плей не будет вызван
            ATestingNewActor* Geometry = World->SpawnActorDeferred<ATestingNewActor>(GeometryClass, GeometryTransform);

            if (Geometry)
            {
                FGeometrydataTestingActor Data;
                Data.Color = FLinearColor::MakeRandomColor();
                Geometry->SetGeometryData(Data);

                //ручной запуск бегин плей
                Geometry->FinishSpawning(GeometryTransform);
            }
        }
    }
}

void AGeometryHubTestingActor::SpawnActors3()
{
    //указатель на мир игры, в нем находится функция спавна
    //GetWorld() - возвращает указатель на глобальный объект мира игры
    UWorld* World = GetWorld();
    if (World)
    {

        //для всех объектов находящихся в массиве GeometryPayloads
        //разделяет объявление переменной (const FGeometryPayloadTestingActor Payload) цикла
        //от коллекции GeometryPayloads (диапазона), по которой будет происходить итерация

        for (const FGeometryPayloadTestingActor Payload : GeometryPayloads)
        {
            //World->SpawnActorDeferred<ATestingNewActor>(...)
            //
            //World : Это указатель на текущий игровой мир(UWorld*).
            //Обычно получается через GetWorld().
            //Мир отвечает за управление всеми акторами на уровне.
            //
            //SpawnActorDeferred : Это специальная функция для спавна(создания) актора,
            //которая откладывает некоторые этапы его инициализации.
            //В отличие от SpawnActor, SpawnActorDeferred не вызывает сразу BeginPlay()
            //и другие функции инициализации, связанные с полным появлением актора в мире.
            //
            //<ATestingNewActor> : Это шаблонный параметр, указывающий ожидаемый тип возвращаемого значения.
            //То есть, мы говорим, что хотим заспавнить актора, который будет(или будет совместим с)
            //типом ATestingNewActor, и мы хотим получить указатель именно этого типа.
            //
            //Payload.GeometryClass : Это аргумент, передающий класс актора, который нужно создать.
            //Он должен быть типа TSubclassOf<AActor>(или более конкретного, например TSubclassOf<ATestingNewActor>).
            //Это UClass * который был выбран в Payload.Это позволяет спавнить акторы разных классов,
            //если Payload.GeometryClass может содержать разные классы.
            //
            //Payload.InitialTransform : Это аргумент, передающий начальную трансформацию(положение, поворот, масштаб)
            //для создаваемого актора.Тип FTransform.ATestingNewActor* Geometry = ... :
            //Результат SpawnActorDeferred присваивается указателю Geometry типа ATestingNewActor*.

            //Зачем нужен SpawnActorDeferred ?
            //Он используется,когда тебе нужно настроить актора(установить его свойства,
            //вызвать какие - то специфические функции инициализации) до того,
            //как он полностью появится в мире и начнет свою “жизнь” (до вызова BeginPlay()).
            //Если бы ты использовал обычный SpawnActor,
            //то BeginPlay() был бы вызван сразу после конструктора,
            //и у тебя не было бы возможности что - то настроить перед этим.

            ATestingNewActor* Geometry = World->SpawnActorDeferred<ATestingNewActor>(Payload.GeometryClass, Payload.InitialTransform);

            if (Geometry)
            {
                //Мы передаем в этот метод какие-то данные из нашей структуры Payload.
                Geometry->SetGeometryData(Payload.Data);

                //Именно здесь, между SpawnActorDeferred и FinishSpawning,
                //ты можешь безопасно вызывать сеттеры и другие функции для настройки своего актора.
                //На этом этапе BeginPlay() еще не был вызван.

                //Что она делает:
                //Завершает процесс инициализации актора.
                //Регистрирует актора в мире.Вызывает PostActorCreated()(если переопределен).
                //Вызывает ExecuteConstruction()(что вызовет OnConstruction() в C++ и Construction Script в Blueprint).
                //Вызывает PostInitializeComponents().
                //Вызывает BeginPlay().
                //После вызова этой функции актор считается полностью “живым” и активным в мире.

                Geometry->OnColorChanged.AddDynamic(this, &AGeometryHubTestingActor::OnColorChanged);
                Geometry->OnTimerFinished.AddUObject(this, &AGeometryHubTestingActor::OnTimerFinished);

                //Payload.InitialTransform: В FinishSpawning также передается трансформация.
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

//функция при срабатывании делегатов
void AGeometryHubTestingActor::OnColorChanged(const FLinearColor& Color, const FString& Name)
{
    UE_LOG(LogHubActor, Warning, TEXT("Actor name: %s Color: %s"), *Name, *Color.ToString());
}

void AGeometryHubTestingActor::OnTimerFinished(AActor* Actor)
{
    if (!Actor) return;
    UE_LOG(LogHubActor, Error, TEXT("Timer finished: %s"), *Actor->GetName());

    //Даункастинг, преобразует указатель на базовый класс на указатель на класс наследника
    ATestingNewActor* Geometry = Cast<ATestingNewActor>(Actor);
    if (!Geometry) return;

    UE_LOG(LogHubActor, Error, TEXT("Cast success, amplitude %f"), Geometry->GetGeometryData().Amplitude);

    //удаление актора со сцены
    Geometry->Destroy();

    //через 2 сек вызовется destroy
    //Geometry->SetLifeSpan(2.0f);
}

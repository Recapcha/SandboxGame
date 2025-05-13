// Fill out your copyright notice in the Description page of Project Settings.


#include "SandboxPawn.h"
//объявление заголовочный файлов для использования 
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"

//собственная категория логирования для pawn 
DEFINE_LOG_CATEGORY_STATIC(LogSandboxPawn, All, All)

// Sets default values
ASandboxPawn::ASandboxPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Определение корневого элемента, применение к нему логики
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

	//добавление статик меш компонента  класс 
	//атачим к сцене, базовой трансформации сцены
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	
	//поменять трансформацию относительно родительского компонента 
	//StaticMeshComponent->SetRelativeLocation();

	//добавление камеры
	//атачим к сцене, базовой трансформации сцены
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ASandboxPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASandboxPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//на функцию tick мы изменяем положение нашего pawn в зависимости от вектора скорости
	//если вектор 0, то вычисления можно не делать
	//if (!VelocityVector.IsZero() &&IsControlled) проверки на то управляется ли кем то или нет 
	if (!VelocityVector.IsZero())
	{
		//локальная константа, в которой хранится новое положения для pawn
		//вычисляется она по формулен равномерного прямолинейного движения 
		//r = r0 + vt
		const FVector NewLocation = GetActorLocation() + Velocity * DeltaTime * VelocityVector;
		SetActorLocation(NewLocation);

		//второй способ вместо &&IsControlled, останавливать объект. 
		//обнулять вектор скорости
		VelocityVector = FVector::ZeroVector;
	}
}

// Called to bind functionality to input
//бинд функций для управления
void ASandboxPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//проверка на нулевой указатель
	if (PlayerInputComponent)
	{
		//"бинд маппинга" "указатель на объект" "ссылка на функцию, которые мы вызываем во время работы"
		PlayerInputComponent->BindAxis("MoveForward", this, &ASandboxPawn::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &ASandboxPawn::MoveRight);
	}
}

void ASandboxPawn::MoveForward(float Amount)
{
	//UE_LOG(LogSandboxPawn, Display, TEXT("Move forward: %f"), Amount)
	//изменение вектора скорости в зависимости от значения
	//значения будут -1, 0, 1
	VelocityVector.X = Amount;
}

void ASandboxPawn::MoveRight(float Amount)
{
	//UE_LOG(LogSandboxPawn, Display, TEXT("Move right: %f"), Amount)
	VelocityVector.Y = Amount;
}

//Получение контроля над pawn
void ASandboxPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//проверка на нулевой указатель
	if (!NewController) return;
	UE_LOG(LogSandboxPawn, Error, TEXT("%s possessed %s"), *GetName(), *NewController->GetName());
}

void ASandboxPawn::UnPossessed()
{
	Super::UnPossessed();

	UE_LOG(LogSandboxPawn, Warning, TEXT("%s unpossessed"), *GetName());
}

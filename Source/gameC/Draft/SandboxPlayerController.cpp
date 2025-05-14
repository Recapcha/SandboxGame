// Fill out your copyright notice in the Description page of Project Settings.


#include "SandboxPlayerController.h"
#include "Components/InputComponent.h"
//доп класс для UGameplaySatics
#include "Kismet/GameplayStatics.h"
#include "SandboxPawn.h"

DEFINE_LOG_CATEGORY_STATIC(LogSandboxPlayerController, All, All)

//переопределенная функция при нажатии клавиши
void ASandboxPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//проверка на нулевой указатель
	if (InputComponent)
	{
		//запуск действий при нажатии клавиши 
		InputComponent->BindAction("ChangePawn", IE_Pressed, this, &ASandboxPlayerController::ChangePawn);
	}
}

void ASandboxPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//получение всех акторов со сцены ASandboxPawn
	//может получить всех акторов в мире ASandboxPawn 
	//или получить доступ к акторам у SandboxPlayerController передав туда ключевое слово this
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASandboxPawn::StaticClass(), Pawns);
}

//функция изменения pawn
void ASandboxPlayerController::ChangePawn()
{
	//количество элементов в массиве больше 1 
	if (Pawns.Num() <= 1) return;

	//изменение pawn на одного из тех, кто находится на сцене
	//указатель на pawn из массива
	//каждый раз будет получать новый pawn, меняя индекс массива
	ASandboxPawn* CurrentPawn = Cast<ASandboxPawn>(Pawns[CurrentPawnIndex]);
	
	//прибавление для перехода на след pawn
	CurrentPawnIndex = (CurrentPawnIndex + 1) % Pawns.Num();
	if (!CurrentPawn) return;

	UE_LOG(LogSandboxPlayerController, Error, TEXT("Change player pawn"));
	Possess(CurrentPawn);
}

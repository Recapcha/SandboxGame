// Fill out your copyright notice in the Description page of Project Settings.


#include "SandboxPlayerController.h"
#include "Components/InputComponent.h"
//��� ����� ��� UGameplaySatics
#include "Kismet/GameplayStatics.h"
#include "SandboxPawn.h"

DEFINE_LOG_CATEGORY_STATIC(LogSandboxPlayerController, All, All)

//���������������� ������� ��� ������� �������
void ASandboxPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//�������� �� ������� ���������
	if (InputComponent)
	{
		//������ �������� ��� ������� ������� 
		InputComponent->BindAction("ChangePawn", IE_Pressed, this, &ASandboxPlayerController::ChangePawn);
	}
}

void ASandboxPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//��������� ���� ������� �� ����� ASandboxPawn
	//����� �������� ���� ������� � ���� ASandboxPawn 
	//��� �������� ������ � ������� � SandboxPlayerController ������� ���� �������� ����� this
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASandboxPawn::StaticClass(), Pawns);
}

//������� ��������� pawn
void ASandboxPlayerController::ChangePawn()
{
	//���������� ��������� � ������� ������ 1 
	if (Pawns.Num() <= 1) return;

	//��������� pawn �� ������ �� ���, ��� ��������� �� �����
	//��������� �� pawn �� �������
	//������ ��� ����� �������� ����� pawn, ����� ������ �������
	ASandboxPawn* CurrentPawn = Cast<ASandboxPawn>(Pawns[CurrentPawnIndex]);
	
	//����������� ��� �������� �� ���� pawn
	CurrentPawnIndex = (CurrentPawnIndex + 1) % Pawns.Num();
	if (!CurrentPawn) return;

	UE_LOG(LogSandboxPlayerController, Error, TEXT("Change player pawn"));
	Possess(CurrentPawn);
}

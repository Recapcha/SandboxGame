// Copyright Epic Games, Inc. All Rights Reserved.


#include "gameCGameModeBase.h"
//����������� ������ pawn 
#include "SandboxPawn.h"
#include "SandboxPlayerController.h"

AgameCGameModeBase::AgameCGameModeBase()
{
	//�������������� ������ ������ 
	DefaultPawnClass = ASandboxPawn::StaticClass();
	PlayerControllerClass = ASandboxPlayerController::StaticClass();
}
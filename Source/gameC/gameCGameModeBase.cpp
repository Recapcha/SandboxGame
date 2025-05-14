// Copyright Epic Games, Inc. All Rights Reserved.


#include "gameCGameModeBase.h"
//����������� ������ pawn 
#include "Gamec/Draft/SandboxPawn.h"
#include "Gamec/Draft/SandboxPlayerController.h"

AgameCGameModeBase::AgameCGameModeBase()
{
	//�������������� ������ ������ 
	DefaultPawnClass = ASandboxPawn::StaticClass();
	PlayerControllerClass = ASandboxPlayerController::StaticClass();
}
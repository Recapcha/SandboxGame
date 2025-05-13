// Copyright Epic Games, Inc. All Rights Reserved.


#include "gameCGameModeBase.h"
//подключение класса pawn 
#include "SandboxPawn.h"
#include "SandboxPlayerController.h"

AgameCGameModeBase::AgameCGameModeBase()
{
	//переопределяем дефолт классы 
	DefaultPawnClass = ASandboxPawn::StaticClass();
	PlayerControllerClass = ASandboxPlayerController::StaticClass();
}
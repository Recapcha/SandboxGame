// Copyright Epic Games, Inc. All Rights Reserved.


#include "gameCGameModeBase.h"
//подключение класса pawn 
#include "Gamec/Draft/SandboxPawn.h"
#include "Gamec/Draft/SandboxPlayerController.h"

AgameCGameModeBase::AgameCGameModeBase()
{
	//переопределяем дефолт классы 
	DefaultPawnClass = ASandboxPawn::StaticClass();
	PlayerControllerClass = ASandboxPlayerController::StaticClass();
}
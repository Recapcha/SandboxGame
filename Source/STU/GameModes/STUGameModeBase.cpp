// Shoot Them Up Game, All Right Reserved.

#include "STUGameModeBase.h"
#include "STU/Character/STUBaseCharacter.h"
#include "STU/Player/STUPlayerController.h"
#include "STU/UI/STUGameHUD.h"

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
}
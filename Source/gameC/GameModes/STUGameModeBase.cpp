// Shoot Them Up Game, All Right Reserved.

#include "STUGameModeBase.h"
#include "Gamec/Character/STUBaseCharacter.h"
#include "Gamec/Player/STUPlayerController.h"

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
}
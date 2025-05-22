// Shoot Them Up Game, All Right Reserved.

#include "STUGameHUD.h"
#include "Engine/Canvas.h"
#include "Blueprint/UserWidget.h"

//перезапись виртуальной функции
void ASTUGameHUD::DrawHUD()
{
    //вызов родительской функции
    Super::DrawHUD();

    //DrawCrossHair();
}

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();
    
    //подгрузка widget во вьюпорт
    auto PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass);
    if (PlayerHUDWidget)
    {
        PlayerHUDWidget->AddToViewport();
    }
}

void ASTUGameHUD::DrawCrossHair()
{
    //Определение центра
    const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

    //ширини и длина, цвет
    const float HalfLineSize = 10.0f;
    const float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Green;

    //функция которая умеет рисовать линии
    //горизонтальная линия
    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);

    //вертикальная линия
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}

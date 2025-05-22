#pragma once

//шаблонная функция, которая возвращает указатель на тип компонента pawn 
class STUUtils
{
public:
    template <typename T>
    static T* GetSTUPlayerComponent(APawn* PlayerPawn)
    {
        if (!PlayerPawn) return nullptr;

        //референс на компонент оружия
        const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
        return Cast<T>(Component);
    }
};
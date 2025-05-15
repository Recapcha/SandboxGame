// Shoot Them Up Game, All Right Reserved.

#include "STUHealthComponent.h"
#include "GameFramework/Actor.h"
//#include "STU/Dev/STUFireDamageType.h"
//#include "STU/Dev/STUIceDamageType.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent()
{
    //tick не нужен
    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    Health = MaxHealth;
    //первый раз устанавливаются жизни, поэтому выводит этот делегат
    //что у нас изменилось количество хп 
    OnHealthChanged.Broadcast(Health);

    //подписываемся на делегат OnTakeAnyDamage
    //и вызываем функцию при срабатывании OnTakeAnyDamage
    //ComponentOwner возвращает указатель функции на компонент
    //если он не нулевой, можем подписаться на делегат данного актора
    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
    }
}

//эта функция вызывается каждый раз когда мы получаем любой урон, так как подписаны на делегат OnTakeAnyDamage
//а он вызывается внутри анриловской функции TakeDamage
void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    //UE_LOG(LogHealthComponent, Display, TEXT("Damage: %f"), Damage);

    //Если урон меньше или равен нулю ИЛИ персонаж уже мертв, то прекратить выполнение этой функции и вернуться
    //пока урон есть или персонаж жив, наносить ему урон в размере  
    //сообщить что получен урон
    if (Damage <= 0.0f || IsDead()) return;

    Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);

    //оповещаем всех клиентов, что персонаж погиб
    if (IsDead())
    {
        OnDeath.Broadcast();
    }

    //if (DamageType)
    //{
    //    float CurrentDamage = Damage;
    //    //DamageType
    //    //проверяем относиться ли переданный DamageType к STUFireDamageType
    //    //сравнение с soft class
    //    if (DamageType->IsA<USTUFireDamageType>())
    //    {
    //        //уменьшаем здоровье на дамаг
    //        //CurrentDamage = Damage + 50.0f;
    //        CurrentDamage = Damage;
    //        Health -= CurrentDamage;
    //        UE_LOG(LogHealthComponent, Display, TEXT("Fire damage! %f"), CurrentDamage);
    //    }
    //    else if (DamageType->IsA<USTUIceDamageType>())
    //    {
    //        CurrentDamage = Damage;
    //        Health -= CurrentDamage;
    //        UE_LOG(LogHealthComponent, Display, TEXT("Ice damage! %f"), CurrentDamage);
    //    }
    //    else
    //    {
    //        //во всех остальных случаях, но лучше ввести новый тип, который будет отвечать за другой любой урон
    //        //так как DamageType на входе не nullptr всегда
    //        //Health -= Damage;
    //        //UE_LOG(LogHealthComponent, Display, TEXT("Damage!!! %f"), Damage);
    //    }
    //}
}
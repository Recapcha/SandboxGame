// Shoot Them Up Game, All Right Reserved.

#include "STUHealthComponent.h"
#include "GameFramework/Actor.h"

#include "Engine/World.h"
#include "TimerManager.h"
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

    SetHealth(MaxHealth);

    //подписываемся на делегат OnTakeAnyDamage
    //и вызываем функцию при срабатывании OnTakeAnyDamage
    //ComponentOwner возвращает указатель функции на компонент
    //если он не нулевой, можем подписаться на делегат данного актора
    //
    //Референсы на мир и персонажа
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

    if (Damage <= 0.0f || IsDead() || !GetWorld()) return;
    SetHealth(Health - Damage);
    GetWorld()->GetTimerManager().ClearTimer(TimerHeal);

    //оповещаем всех клиентов, что персонаж погиб
    if (IsDead())
    {
        OnDeath.Broadcast();
    }
    else if (AutoHeal)
    {
        GetWorld()->GetTimerManager().SetTimer(TimerHeal, this, &USTUHealthComponent::OnTimerHeal, HealUpdateTime, true, HealDelay);
    }
}

void USTUHealthComponent::OnTimerHeal()
{
    SetHealth(Health + HealModifier);

    if (FMath::IsNearlyEqual(Health, MaxHealth) && GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(TimerHeal);
    }
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
    Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);
}

// Shoot Them Up Game, All Right Reserved.

#include "STUHealthComponent.h"
#include "GameFramework/Actor.h"

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
    //UE_LOG(BaseCharacterLog, Display, TEXT("Damage: %f"), Damage);
    //уменьшаем здоровье на дамаг
    Health -= Damage;
}
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
    //tick �� �����
    PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    SetHealth(MaxHealth);

    //������������� �� ������� OnTakeAnyDamage
    //� �������� ������� ��� ������������ OnTakeAnyDamage
    //ComponentOwner ���������� ��������� ������� �� ���������
    //���� �� �� �������, ����� ����������� �� ������� ������� ������
    //
    //��������� �� ��� � ���������
    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
    }
}

//��� ������� ���������� ������ ��� ����� �� �������� ����� ����, ��� ��� ��������� �� ������� OnTakeAnyDamage
//� �� ���������� ������ ����������� ������� TakeDamage
void USTUHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    //UE_LOG(LogHealthComponent, Display, TEXT("Damage: %f"), Damage);

    //���� ���� ������ ��� ����� ���� ��� �������� ��� �����, �� ���������� ���������� ���� ������� � ���������
    //���� ���� ���� ��� �������� ���, �������� ��� ���� � �������
    //�������� ��� ������� ����

    if (Damage <= 0.0f || IsDead() || !GetWorld()) return;
    SetHealth(Health - Damage);
    GetWorld()->GetTimerManager().ClearTimer(TimerHeal);

    //��������� ���� ��������, ��� �������� �����
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

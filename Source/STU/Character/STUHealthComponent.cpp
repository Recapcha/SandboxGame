// Shoot Them Up Game, All Right Reserved.

#include "STUHealthComponent.h"
#include "GameFramework/Actor.h"

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

    Health = MaxHealth;

    //������������� �� ������� OnTakeAnyDamage
    //� �������� ������� ��� ������������ OnTakeAnyDamage
    //ComponentOwner ���������� ��������� ������� �� ���������
    //���� �� �� �������, ����� ����������� �� ������� ������� ������
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
    //��������� �������� �� �����
    Health -= Damage;

    UE_LOG(LogHealthComponent, Display, TEXT("Damage: %f"), Damage);
}
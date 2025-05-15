// Shoot Them Up Game, All Right Reserved.

#include "STUHealthComponent.h"
#include "GameFramework/Actor.h"

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
    //UE_LOG(BaseCharacterLog, Display, TEXT("Damage: %f"), Damage);
    //��������� �������� �� �����
    Health -= Damage;
}
// Shoot Them Up Game, All Right Reserved.

#include "STUHealthComponent.h"
#include "GameFramework/Actor.h"
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

    Health = MaxHealth;
    //������ ��� ��������������� �����, ������� ������� ���� �������
    //��� � ��� ���������� ���������� �� 
    OnHealthChanged.Broadcast(Health);

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
    //UE_LOG(LogHealthComponent, Display, TEXT("Damage: %f"), Damage);

    //���� ���� ������ ��� ����� ���� ��� �������� ��� �����, �� ���������� ���������� ���� ������� � ���������
    //���� ���� ���� ��� �������� ���, �������� ��� ���� � �������  
    //�������� ��� ������� ����
    if (Damage <= 0.0f || IsDead()) return;

    Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(Health);

    //��������� ���� ��������, ��� �������� �����
    if (IsDead())
    {
        OnDeath.Broadcast();
    }

    //if (DamageType)
    //{
    //    float CurrentDamage = Damage;
    //    //DamageType
    //    //��������� ���������� �� ���������� DamageType � STUFireDamageType
    //    //��������� � soft class
    //    if (DamageType->IsA<USTUFireDamageType>())
    //    {
    //        //��������� �������� �� �����
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
    //        //�� ���� ��������� �������, �� ����� ������ ����� ���, ������� ����� �������� �� ������ ����� ����
    //        //��� ��� DamageType �� ����� �� nullptr ������
    //        //Health -= Damage;
    //        //UE_LOG(LogHealthComponent, Display, TEXT("Damage!!! %f"), Damage);
    //    }
    //}
}
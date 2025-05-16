// Shoot Them Up Game, All Right Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUHealthComponent.generated.h"

//������� �� ������ ���������
DECLARE_MULTICAST_DELEGATE(FOnDeath);
//�������, ����� �������� ����� ���������
//� ������� ���� ����� ������ ������ � ����, �� �������� ��������
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class STU_API USTUHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUHealthComponent();

    //������� ������������ �������� health
    float GetHealth() const { return Health; }

    UFUNCTION(BlueprintCallable)
    bool IsDead() const { return FMath::IsNearlyZero(Health); }

    FOnDeath OnDeath;
    FOnHealthChanged OnHealthChanged;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "0", ClampMax = "1000.0"))
    float MaxHealth = 100.0f;

    //�������� �� ���������� ��������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
    bool AutoHeal = true;

    //������� ����������� ��������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealUpdateTime = 0.3f;

    //�������� ����� ����������� ��������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealDelay = 3.0f;

    //������� �� ���� ��� ������������ ��������
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (EditCondition = "AutoHeal"))
    float HealModifier = 1.0f;

    virtual void BeginPlay() override;

private:
    float Health = 0.0f;

    //������ Heal
    FTimerHandle TimerHeal;

    //����� ��������� � �������� OnTakeAnyDamage, �� ����� ��� ��������
    //�����, ������� ������� ����
    //����
    //��� �����
    //����� ���������� ����� ����
    //����� ������� ����� ����
    UFUNCTION()
    void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    void OnTimerHeal();
    void SetHealth(float NewHealth);
};

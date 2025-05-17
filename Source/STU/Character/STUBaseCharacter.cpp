// Shoot Them Up Game, All Right Reserved.

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "STU_CharacterMovementComponent.h"
#include "STU/Character/STUWeaponComponent.h"
#include "STUHealthComponent.h"
#include "STUBaseCharacter.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"


DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All);

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTU_CharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);
    SpringArmComponent->TargetArmLength = 600.0f;

    //attach � ��������� ����������
    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    //����� ������ ������ �� ����������� �� ������������� � ��� �������
    HealthTextComponent->SetOwnerNoSee(true);

    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    //�������� �� 0 ����������, �������� ������ � ������� ������
    //�� ����������, ���� �� ���������
    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());

    OnHealthChanged(HealthComponent->GetHealth());

    //������������� � ������ �� ��������� ��������� �� ���������
    //����� ������� ��� ��������� ��������� ������ ��������� � ��������, ����� ������� OnDeath
    //AddUObject ����� ����� ������ ��� �� ���������� � ��������, ������ � C++
    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);

    //������� �� ��������� �����
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

    //��������� ����� �� �������
    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //��������� �����
    //��� ��� �����, � FDamageEvent ���� <class UDamageType>, � ��� ����� ������� ��� �����
    //�� ���� ����� ������� ����������� ����� � ���� ���������� ��������� �����
    //FRadialDamageParams ��������� ����� �� �������
    //����� ��������� �� Controller, ��� ����� �����, ����� ������ �� ����� �������
    //���� ������� ����, ���� this
    //TakeDamage(0.1f, FDamageEvent{}, Controller, this);
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    check(PlayerInputComponent);
    check(WeaponComponent);


    //����� ������� � ����������� �� ������� ������� � ��� ��� ����� ���������� this
    PlayerInputComponent->BindAxis("MoveForward", this, &ASTUBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASTUBaseCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ASTUBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ASTUBaseCharacter::AddControllerYawInput);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUBaseCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUBaseCharacter::OnStopRunning);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USTUWeaponComponent::Fire);

}

void ASTUBaseCharacter::MoveForward(float Amount)
{
    IsMovingForward = Amount > 0.0f;
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ASTUBaseCharacter::MoveRight(float Amount)
{
    if (Amount == 0.0f) return;
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASTUBaseCharacter::OnStartRunning()
{
    WantsToRun = true;
}
void ASTUBaseCharacter::OnStopRunning()
{
    WantsToRun = false;
}

bool ASTUBaseCharacter::IsRunning() const
{
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

float ASTUBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero()) return 0.0f;
    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ASTUBaseCharacter::OnDeath()
{
    UE_LOG(BaseCharacterLog, Display, TEXT("Player %s is dead"), *GetName());

    //��������� ��������
    PlayAnimMontage(DeathAnimMontage);

    //��������� ����� ��������
    GetCharacterMovement()->DisableMovement();

    //����������� ���������, ����� 5 ���
    SetLifeSpan(5.0f);

    //��� ������ ������������ �� ����������� ����� �����������
    //���� ����� �� ���� controller - ctrl +f ChangeState - f12 -BeginSpectatingState
    //����� ������� �� ��� ����������� ������� UnPosses() � ��������� ������ pawn
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void ASTUBaseCharacter::OnHealthChanged(float Health)
{
    //����� ������ ��� ���������� �������
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    //�������� ������� �������� �������
    //�������� ������� ����� ����� ����� �� ��� �������� ������������ �� Z
    //��� ��� �������� �� Z ����� ������������� ��� �������, �� ������� �� -1
    const auto FallVelocityZ = -GetCharacterMovement()->Velocity.Z;
    UE_LOG(BaseCharacterLog, Display, TEXT("On landed: %f"), FallVelocityZ);

    if (FallVelocityZ < LandedDamageVelocity.X) return;

    //��������� ����� � ����������� �� ������
    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
    UE_LOG(BaseCharacterLog, Display, TEXT("FinalDamage: %f"), FinalDamage);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}

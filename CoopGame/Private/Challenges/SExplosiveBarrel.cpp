// Fill out your copyright notice in the Description page of Project Settings.


#include "Challenges/SExplosiveBarrel.h"
#include "Components/SHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundCue.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	
    HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealtComp"));
    HealthComp->OnHealthChanged.AddDynamic(this, &ASexplosiveBarrel::OnHealthChanged);

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    MeshComp->SetSimulatePhysics(true);
    MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
    RootComponent = MeshComp;

    RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
    RadialForceComp->SetupAttachment(MeshComp);
    RadialForceComp->Radius = 250;
    RadialForceComp->bImpulseVelChange = true;
    RadialForceComp->bAutoActivate = false;
    RadialForceComp->bIgnoreOwningActor = true; //ignore self

    ExplosionImpulse = 400;

    ExplosionDamage = 80;

    SetReplicates(true);
    SetReplicateMovement(true);

}

void ASExplosiveBarrel::OnHealthChanged(USHealthComponent* OwningHealthComp, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) {

    if (bExploded)
    {

        //nothing to do, already explosion
        return;

    }

    if (Health <= 0.0f)
    {

        bExploded = true; //Explode!
        OnRep_Exploded();

        UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());

        FVector BoostIntensity = FVector::UpVector * ExplosionImpulse;
        MeshComp->AddImpulse(BoostIntensity, NAME_None, true);

        RadialForceComp->FireImpulse();

    }

}

void ASExplosiveBarrel::OnRep_Exploded()
{

    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());

    TArray<AActor*> IgnoredActors;

    UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage, GetActorLocation(), ExplosionImpulse, nullptr, IgnoredActors, this, GetInstigatorController(), true);

    MeshComp->SetMaterial(0, ExplodedMaterial);

}

void ASExplosiveBarrel::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ASExplosiveBarrel, bExploded);

}



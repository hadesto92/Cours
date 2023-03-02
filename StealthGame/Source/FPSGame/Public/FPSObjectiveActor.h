// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSObjectiveActor.generated.h"

class USphereComponent;

UCLASS()
class FPSGAME_API AFPSObjectiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSObjectiveActor();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components") //Tworzenie metadanych (w³aœciwoœci) - nowej kategori - w obiekcie dla MeshComp
	UStaticMeshComponent* MeshComp; //Utworzenie w klasie statycznego komponentu o nazwie MeshComp

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* PickupFX;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PlayEffects(); //Utworzenie nowej funkcji effekt która bêdzie nak³ada³a na obiekt np. dym

public:	

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override; //Daje mo¿liwoœc nadpisania (override) funkcji która bêdzie odpowiedzialna za interkacjê w momencie kiedy gracz wejdzie w kolizjê z obiektem, zmieniamy ju¿ utworzon¹ funkcjê dlatego bêdziemy musieli dodaæ zabezpieczenie w cpp aby stara implementacja te¿ by³a



};

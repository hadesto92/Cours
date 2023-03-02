// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSObjectiveActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FPSCharacter.h"

// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp")); //Tworzy mesh do objektu
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); //Ustawia mesh ¿e nie ma ¿adnej kolizji
	RootComponent = MeshComp; //Przypisuje MeshComp do g³ównej struktury objektu

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp")); //Tworzy sferê kolizji
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //Ustawienie kolizji sfery w trybie zapytania dziêki czemu mo¿emy rozbiæ to na kana³y
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore); //Ustawienie ¿e kolizja i reakcja na wszystkie kana³y zostan¹ zignorowane
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); //Ustawienie rekacji tylko na jeden kana³ dla PAWN oraz aby siê nak³ada³ (Overlap)
	SphereComp->SetupAttachment(MeshComp); //Do³¹cza sferê do MeshComp

	SetReplicates(true);

}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
	
	PlayEffects(); //na pocz¹tu gry ma wywo³aæ siê funkcja efektu 

}

void AFPSObjectiveActor::PlayEffects()
{

	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation()); //Po wywo³aniu funkci ma ona w lokalizacji objektu utworzyæ efekt np. dym

}

void AFPSObjectiveActor::NotifyActorBeginOverlap(AActor* OtherActor)
{

	Super::NotifyActorBeginOverlap(OtherActor); //Poniewa¿ zmieniamy efekt juz utworzonej funkcji jest to upewnienie siê ¿e podstawowa implenetacja wywo³a siê i robimy to za pomoc¹ super aktorów

	PlayEffects();

	if (GetLocalRole() == ROLE_Authority)
	{

		AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(OtherActor); //Utworzenie nowego gracza który ma interakcjê z objektem

		if (MyCharacter) //jeœli zosta³ utworzony nowy gracz nalezy ustawiæ ¿e podnus³ obiekt oraz go zniszczyæ na koniec
		{

			MyCharacter->bIsCarryingObjective = true;

			Destroy();

		}

	}

}


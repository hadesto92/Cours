// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "FPSGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHear);

	GuardState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
	FirstLocation = GetActorLocation();

	OrginalRotation = GetActorRotation();

	if(bPatrol==true)
	{

		if (bGeneratePatrol == true)
		{

			GeneratePointPatrol();

		}
		if (bFirstPointPatrolAtInPutLocation == true && bGeneratePatrol == false)
		{

			FirstPointPatrol();

		}

		PatrolPointMovement();

	}

}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	
	if (SeenPawn == nullptr)
	{

		return;

	}
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

	SetGuardState(EAIState::Alerted);

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), SeenPawn->GetActorLocation());

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{

		FVector Delta = GetActorLocation() - SeenPawn->GetActorLocation();
		float DistanceToPawn = Delta.Size();

		if (DistanceToPawn < DistanceToPawnCheck)
		{

			GM->CompleteMission(SeenPawn, false);

		}
		else
		{

			GetWorld()->GetTimerManager().SetTimer(TimeHandle_EndMission, this, &AFPSAIGuard::ResetLocation, TimeToWaitSEEN);

		}

	}

}

void AFPSAIGuard::OnNoiseHear(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
	{

		return;

	}

	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);
	
	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;

	SetActorRotation(NewLookAt);

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), Location);

	GetWorldTimerManager().SetTimer(TimerHandle_ResetLocation, this, &AFPSAIGuard::ResetLocation, TimeToWaitHEAR, false);

	SetGuardState(EAIState::Suspicious);

}

void AFPSAIGuard::ResetLocation()
{

	SetGuardState(EAIState::Idle);

	if (bPatrol == true)
	{

		PatrolPointMovement();

	}
	else
	{

		UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetController(), FirstLocation);

		FVector Delta = GetActorLocation() - FirstLocation;
		float DistanceToFirstLocation = Delta.Size();

		float wait = (DistanceToFirstLocation/GuardMovementSpeed)+1;

		GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, wait, false);

	}

}

void AFPSAIGuard::ResetOrientation()
{

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("\nGuard is FirstLocation!")));
	SetActorRotation(OrginalRotation);

}

void AFPSAIGuard::OnRep_GuardState()
{

	OnStateChanged(GuardState);

}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{

	if (GuardState == NewState)
	{

		return;

	}

	GuardState = NewState;

	OnRep_GuardState();

}

void AFPSAIGuard::PatrolPointMovement()
{

	if (CurrentPatrolPoint == nullptr || CurrentPatrolPoint == PatrolPoint[PatrolPoint.Num() - 1])
	{

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("\nGuard is first point!")));

		PatrolPointID = 0;

		CurrentPatrolPoint = PatrolPoint[PatrolPointID];

	}
	else
	{

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("\nGuard is next point!")));

		PatrolPointID++;

		CurrentPatrolPoint = PatrolPoint[PatrolPointID];

	}

	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), CurrentPatrolPoint);

}

void AFPSAIGuard::GeneratePointPatrol()
{

	FirstPointPatrol();

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	for (int i = 1; i < MaxPointPatrol; i++)
	{
		
		GetRandomLocation(FirstLocation, RadiusGeneratorPatrolPoint);

		ATargetPoint* NewPoint = GetWorld()->SpawnActor<ATargetPoint>(NewGeneratedLocation, OrginalRotation, SpawnInfo);

		PatrolPoint.Add(NewPoint);

		DrawDebugSphere(GetWorld(), NewGeneratedLocation, 32.0f, 12, FColor::Blue, false, 60.0f);

	}

}

void AFPSAIGuard::FirstPointPatrol()
{

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	ATargetPoint* NewPoint = GetWorld()->SpawnActor<ATargetPoint>(FirstLocation, OrginalRotation, SpawnInfo);

	PatrolPoint.Add(NewPoint);

	DrawDebugSphere(GetWorld(), FirstLocation, 32.0f, 12, FColor::Blue, false, 60.0f);

}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);

	if (CurrentPatrolPoint)
	{

		FVector Delta = GetActorLocation() - CurrentPatrolPoint->GetActorLocation();
		float DistanceToGoal = Delta.Size();

		if (DistanceToGoal < DistanceCheck)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("\nGuard is point!")));
			PatrolPointMovement();
		}

	}

}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuardState);

}


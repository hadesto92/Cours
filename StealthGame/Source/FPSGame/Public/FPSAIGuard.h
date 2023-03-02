// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/TargetPoint.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EAIState : uint8
{

	Idle,

	Suspicious,

	Alerted

};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
		void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
		void OnNoiseHear(APawn* NoiseInstigator, const FVector& Location, float Volume);

	FRotator OrginalRotation;

	FVector FirstLocation;

	FTimerHandle TimerHandle_ResetOrientation;

	FTimerHandle TimerHandle_ResetLocation;

	FTimerHandle TimeHandle_EndMission;

	UFUNCTION()
		void ResetLocation();

	UFUNCTION()
		void ResetOrientation();

	UPROPERTY(ReplicatedUsing=OnRep_GuardState)
		EAIState GuardState;

	UFUNCTION()
		void OnRep_GuardState();

	UFUNCTION()
		void SetGuardState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
		void OnStateChanged(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
		void GetRandomLocation(FVector Origin, float Radius);

	UFUNCTION()
		void PatrolPointMovement();

	UFUNCTION()
		void GeneratePointPatrol();

	UFUNCTION()
		void FirstPointPatrol();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected: 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Patrol")
		bool bPatrol;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Patrol", meta = (EditCOndition = "bPatrol"))
		bool bGeneratePatrol;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Patrol", meta = (EditCOndition = "!bGeneratePatrol"))
		bool bFirstPointPatrolAtInPutLocation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Patrol")
		float DistanceCheck = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Patrol", meta = (EditCondition = "bGeneratePatrol"))
		int MaxPointPatrol = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Patrol", meta = (EditCondition = "bGeneratePatrol"))
		float RadiusGeneratorPatrolPoint = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Patrol", meta = (EditCondition="!bGeneratePatrol"))
		TArray<ATargetPoint*> PatrolPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
		float DistanceToPawnCheck = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Difficult")
		float TimeToWaitSEEN = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Difficult")
		float TimeToWaitHEAR = 5;

	UPROPERTY(BlueprintReadWrite, Category = "Movement")
		float GuardMovementSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "AI")
		FVector NewGeneratedLocation;

	int PatrolPointID = 0;

	bool flag;

	AActor* CurrentPatrolPoint;
};

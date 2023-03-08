// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameMode.h"
#include "TimerManager.h"
#include "Components/SHealthComponent.h"
#include "SGameState.h"
#include "EngineUtils.h"
#include "SPlayerState.h"


ASGameMode::ASGameMode()
{

	TimeBetweenWaves = 10.0f;

	GameStateClass = ASGameState::StaticClass();
	PlayerStateClass = ASplayerState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;

}

void ASGameMode::StartWave()
{

	WaveCount++;

	NrOfBotsToSpawn = 2 * WaveCount;

	GetWorldTimerManager().SetTimer(TimerHandle_BotSpawner, this, &ASGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);

	SetWaveState(EWaveState::WaveInProgress);
}

void ASGameMode::EndWave()
{

	GetWorldTimerManager().ClearTimer(TimerHandle_BotSpawner);

	SetWaveState(EWaveState::WaitingToComplete);

}

void ASGameMode::GameOver()
{

	EndWave();

	//Finish up the match present 'game over' to player

	SetWaveState(EWaveState::GameOver);

}

void ASGameMode::PrepareForNextWave()
{

	GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASGameMode::StartWave, TimeBetweenWaves, false);

	SetWaveState(EWaveState::WaitingToStart);

	RestartDeadPlayers();

}

void ASGameMode::CheckWaveState()
{

	bool bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);

	if (NrOfBotsToSpawn > 0 || bIsPreparingForWave)
	{

		return;

	}

	if (CheckAnyBotAlive() == false)
	{


		SetWaveState(EWaveState::WaveComplete);
		PrepareForNextWave();

	}

}

bool ASGameMode::CheckAnyBotAlive()
{

	if (NrOfBotsToSpawn <= 0)
	{

		for (TActorIterator<APawn> Its(GetWorld()); Its; ++Its)
		{

			APawn* TestPawn = *Its;
			if (TestPawn->ActorHasTag(FName(TEXT("RiderEnemy"))))
			{

				return true;

			}

		}

		return false;

	}
	else
	{

		return true;

	}

}

void ASGameMode::CheckAnyPlayerAlive()
{

	for (TActorIterator<APlayerController> Its(GetWorld()); Its; ++Its)
	{

		APlayerController* PC = Cast<APlayerController>(*Its);
		if (PC && PC->GetPawn())
		{

			APawn* MyPawn = PC->GetPawn();
			USHealthComponent* HealthComp = Cast<USHealthComponent>(MyPawn->GetComponentByClass(USHealthComponent::StaticClass()));
			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f)
			{

				//A player is still alive
				return;

			}

		}

	}

	//No player alive
	GameOver();

}

void ASGameMode::SetWaveState(EWaveState NewState)
{

	ASGameState* GS = GetGameState<ASGameState>();
	if (ensureAlways(GS))
	{

		GS->SetWaveState(NewState);

	}

}

void ASGameMode::RestartDeadPlayers()
{

	for (TActorIterator<APlayerController> Its(GetWorld()); Its; ++Its)
	{

		APlayerController* PC = Cast<APlayerController>(*Its);
		if (PC && PC->GetPawn() == nullptr)
		{

			RestartPlayer(PC);

		}

	}

}

void ASGameMode::StartPlay()
{

	Super::StartPlay();

	PrepareForNextWave();

}

void ASGameMode::Tick(float DeltaSeconds)
{

	Super::Tick(DeltaSeconds);

	CheckWaveState();
	CheckAnyPlayerAlive();

}

void ASGameMode::SpawnBotTimerElapsed()
{

	SpawnNewBot();

	NrOfBotsToSpawn--;
	
	if (NrOfBotsToSpawn <= 0)
	{

		EndWave();

	}

}

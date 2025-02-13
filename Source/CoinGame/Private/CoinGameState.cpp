// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinGameState.h"
#include "Kismet/GameplayStatics.h"
#include "CoinPlayerController.h"
#include "SpawnVolume.h"
#include "CoinGameInstance.h"
#include "CoinItem.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

ACoinGameState::ACoinGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 30.0f; 
	CurrentLevelIndex = 0;
	MaxLevels = 3;
}

void ACoinGameState::BeginPlay()
{
	Super::BeginPlay();

	UpdateHUD();
	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ACoinGameState::UpdateHUD,
		0.1f,
		true
	);
}

int32 ACoinGameState::GetScore() const
{
	return Score;
}

void ACoinGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCoinGameInstance* CoinGameInstance = Cast<UCoinGameInstance>(GameInstance);
		if (CoinGameInstance)
		{
			CoinGameInstance->AddToScore(Amount);
		}
	}
}

void ACoinGameState::StartLevel()
{

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ACoinPlayerController* CoinPlayerController = Cast<ACoinPlayerController>(PlayerController))
		{
			CoinPlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCoinGameInstance* CoinGameInstance = Cast<UCoinGameInstance>(GameInstance);
		if (CoinGameInstance)
		{
			CurrentLevelIndex = CoinGameInstance->CurrentLevelIndex;
		}
	}

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	
	const int32 ItemToSpawn = 40;
	
	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
	
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}
	
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ACoinGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);
}

void ACoinGameState::OnLevelTimeUp()
{
	EndLevel();
}

void ACoinGameState::OnCoinCollected()
{
	CollectedCoinCount++;

    if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
    {
    	EndLevel();
    }
}

void ACoinGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UCoinGameInstance* CoinGameInstance = Cast<UCoinGameInstance>(GameInstance);
		if (CoinGameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			CoinGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	if (CurrentLevelIndex >= MaxLevels - 1)
	{
		OnGameOver();
		return;
	}

	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void ACoinGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ACoinPlayerController* CoinPlayerController = Cast<ACoinPlayerController>(PlayerController))
		{
			CoinPlayerController->SetPause(true);
			CoinPlayerController->ShowMainMenu(true);
		}
	}
}

void ACoinGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		ACoinPlayerController* CoinPlayerController = Cast<ACoinPlayerController>(PlayerController);
		{
			if (UUserWidget* HUDWidget = CoinPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("TimeText"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("ScoreText"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UCoinGameInstance* CoinGameInstance = Cast<UCoinGameInstance>(GameInstance);
						if (CoinGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), CoinGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("LevelText"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}
			}
		}
	}
}
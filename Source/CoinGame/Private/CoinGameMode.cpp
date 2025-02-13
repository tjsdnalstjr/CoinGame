// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinGameMode.h"
#include "CoinPlayerController.h"
#include "CoinGameState.h"
#include "PlayerCharacter.h"

ACoinGameMode::ACoinGameMode()
{
    PlayerControllerClass = ACoinPlayerController::StaticClass();
    DefaultPawnClass = APlayerCharacter::StaticClass();
    GameStateClass = ACoinGameState::StaticClass();
}
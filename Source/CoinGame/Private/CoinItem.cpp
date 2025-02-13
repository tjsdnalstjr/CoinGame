// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinItem.h"
#include "Engine/World.h"
#include "CoinGameState.h"

ACoinItem::ACoinItem()
{

    PointValue = 0;
    ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
    if (Activator && Activator->ActorHasTag("Player"))
    {
        if (UWorld* World = GetWorld())
        {
            if (ACoinGameState* GameState = World->GetGameState<ACoinGameState>())
            {
                GameState->AddScore(PointValue);
            }
        }
        DestroyItem();
    }   
}
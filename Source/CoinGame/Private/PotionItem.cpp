// Fill out your copyright notice in the Description page of Project Settings.


#include "PotionItem.h"
#include "PlayerCharacter.h"
#include "CoinPlayerController.h"


void APotionItem::ActivateItem(AActor* Activator)
{
    if (Activator && Activator->ActorHasTag("Player"))
    {
        float a = FMath::FRandRange(0, 1.0f);

        if (a < 0.5f)
        {
            EffectHeal(Activator);
        }
        else
        {
            //EffectAddSpeed(Activator);
        }

        DestroyItem();
    }
}

void APotionItem::EffectHeal(AActor* Activator)
{
    APlayerCharacter* player = Cast<APlayerCharacter>(Activator);
    player->AddHealth(HealValue);
}

void APotionItem::EffectAddSpeed(AActor* Activator)
{
    APlayerCharacter* player = Cast<APlayerCharacter>(Activator);
    player->SetSprintSpeed(2.2f);

    FTimerDelegate TimerDelegate;
    TimerDelegate.BindUFunction(this, FName("EffectEnd"), Activator);
    GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, TimerDelegate, 3.0f, false);
}

void APotionItem::EffectEnd(AActor* Activator)
{
    APlayerCharacter* player = Cast<APlayerCharacter>(Activator);
    player->SetSprintSpeed(1.7f);
}
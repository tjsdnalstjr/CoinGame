// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "PotionItem.generated.h"

/**
 * 
 */
UCLASS()
class COINGAME_API APotionItem : public ABaseItem
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 HealValue;

	FTimerHandle EffectTimerHandle;

	virtual void ActivateItem(AActor* Activator) override;

	void EffectHeal(AActor* Activator);
	void EffectAddSpeed(AActor* Activator);
	void EffectEnd(AActor* Activator);

};
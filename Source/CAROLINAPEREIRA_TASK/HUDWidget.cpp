// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"

void UHUDWidget::UpdateCollectiblesCollected(int NewAmount)
{
	if (CollectiblesCollectedText)
	{
		CollectiblesCollectedText->SetText(FText::AsNumber(NewAmount));
	}
}

void UHUDWidget::SetTotalAmount(int NewAmount)
{
	if (TotalCollectibles)
	{
		TotalCollectibles->SetText(FText::AsNumber(NewAmount));
	}
}
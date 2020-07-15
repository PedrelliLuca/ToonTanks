// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"

void  APlayerControllerBase::SetPlayerEnabledState(bool bSetPlayerEnabled)
{
    if (bSetPlayerEnabled)
    {
        GetPawn()->EnableInput(this);
        APlayerControllerBase::bShowMouseCursor = true;
    }
    else
    {
        GetPawn()->DisableInput(this);
        APlayerControllerBase::bShowMouseCursor = false;
    }
}
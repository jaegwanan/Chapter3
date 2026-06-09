// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "Player_Soldier.h"
#include "MyGameModeBase.h"

#include "MyPlayerController.h"

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = APlayer_Soldier::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
}

// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "HovercraftGangWarsGameMode.h"
#include "HovercraftGangWarsPawn.h"

AHovercraftGangWarsGameMode::AHovercraftGangWarsGameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = AHovercraftGangWarsPawn::StaticClass();
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Match.generated.h"

class URating;

/**
 *
 */
UCLASS(BlueprintType)
class UEGLICKO_API UMatch : public UObject
{
	GENERATED_BODY()
public:
	UMatch();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Make Match", Keywords = "Glicko Match Make Match"), Category = "UEGlicko|Match")
	static UMatch *MakeMatch(URating *opponent, float score = 0);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Opponent", Keywords = "Glicko Match Get Opponent"), Category = "UEGlicko|Match")
	URating *getOpponent();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Get Score", Keywords = "Glicko Match Get Score"), Category = "UEGlicko|Match")
	float getScore();

private:
	URating *opponent;
	float _score;
};

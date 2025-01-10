// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Match.generated.h"

class URating;

/// @brief Glicko match object.
UCLASS(BlueprintType)
class UEGLICKO_API UMatch : public UObject
{
	GENERATED_BODY()
public:
	/// @brief Class constructor
	UMatch();

	/// @brief Create a new match.
	/// @param opponent Opponent in the match.
	/// @param score The score of the match.
	/// @return match
	/// @ref URating
	UFUNCTION(BlueprintPure, meta = (DisplayName = "Make Match", ReturnDisplayName = "Match", Keywords = "Glicko Match Make Match"), Category = "UE Glicko|Match")
	static UMatch *MakeMatch(URating *opponent, float score = 0);

	/// @brief Get the opponent in the match.
	/// @return opponent rating
	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "Get Opponent", Keywords = "Glicko Match Get Opponent"), Category = "UE Glicko|Match")
	URating *getOpponent();

	/// @brief Get the score of the match. Should be 0 for a loss, 1 for a win, and .5 for a draw.
	/// @return score
	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "Get Score", Keywords = "Glicko Match Get Score"), Category = "UE Glicko|Match")
	float getScore();

private:
	/// @brief The opponent in the match
	URating *opponent;
	/// @brief The score of the match. Should be 0 for a loss, 1 for a win, and .5 for a draw.
	float _score;
};

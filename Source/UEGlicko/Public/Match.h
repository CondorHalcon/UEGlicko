// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Match.generated.h"

class URating;

/// @brief Glicko match object.
USTRUCT(BlueprintType, meta = (HasNativeMake = "UEGlicko.MatchHelperLibrary.MakeMatch"))
struct UEGLICKO_API FMatch
{
	GENERATED_BODY()

public:
	/// @brief The opponent in the match
	UPROPERTY(VisibleAnywhere)
	URating *Opponent;

	/// @brief The score of the match. Should be 0 for a loss, 1 for a win, and .5 for a draw.
	UPROPERTY(VisibleAnywhere)
	float Score;
};

/// @brief
UCLASS()
class UMatchHelperLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/// @brief Make a new match struct
	/// @param opponent Opponent in the match.
	/// @param score The score of the match.
	/// @return match
	/// @return FMatch
	/// @ref URating
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe, ReturnDisplayName = "Match", Keywords = "Glicko Match Make Match"), Category = "UE Glicko|Match")
	static FMatch MakeMatch(URating *opponent, float score = 0);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rating.generated.h"

class UMatch;

/**
 *
 */
UCLASS()
class UEGLICKO_API URating : public UObject
{
	GENERATED_BODY()
public:
	URating();

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Make Rating", Keywords = "Glicko Rating Make Rating Detailed"), Category = "UEGlicko|Rating")
	static URating *MakeRating(double r = 1500, double d = 350, double v = 0.06);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "Make Rating Simple", Keywords = "Glicko Rating Make Rating"), Category = "UEGlicko|Rating")
	static URating *MakeRatingSimple();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Update Matches", Keywords = "Glicko Rating Update Matches"), Category = "UEGlicko|Rating")
	void UpdateMatches(TArray<UMatch *> Matches);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Update Match", Keywords = "Glicko Rating Update Match"), Category = "UEGlicko|Rating")
	void UpdateMatch(UMatch *Match);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Update Pending", Keywords = "Glicko Rating Update"), Category = "UEGlicko|Rating")
	void UpdatePending();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Decay", Keywords = "Glicko Rating Decay"), Category = "UEGlicko|Rating")
	void Decay();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Apply", Keywords = "Glicko Rating Apply"), Category = "UEGlicko|Rating")
	void Apply();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Rating 1", Keywords = "Glicko Rating Get Rating 1"), Category = "UEGlicko|Rating")
	float getRating1();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Deviation 1", Keywords = "Glicko Rating Get Deviation 1"), Category = "UEGlicko|Rating")
	float getDeviation1();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Volatility 1", Keywords = "Glicko Rating Get Volatility 1"), Category = "UEGlicko|Rating")
	float getVolatility1();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Rating 2", Keywords = "Glicko Rating Get Rating 2"), Category = "UEGlicko|Rating")
	float getRating2();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Deviation 2", Keywords = "Glicko Rating Get Deviation 2"), Category = "UEGlicko|Rating")
	float getDeviation2();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Volatility 2", Keywords = "Glicko Rating Get Volatility 2"), Category = "UEGlicko|Rating")
	float getVolatility2();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Glicko 1", Keywords = "Glicko Rating Get Glicko 1"), Category = "UEGlicko|Rating")
	FString getGlicko1();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Get Glicko 2", Keywords = "Glicko Rating Get Glicko 2"), Category = "UEGlicko|Rating")
	FString getGlicko2();

private:
	UPROPERTY(VisibleAnywhere)
	double rating;

	UPROPERTY(VisibleAnywhere)
	double deviation;

	UPROPERTY(VisibleAnywhere)
	double volatility;

	UPROPERTY(VisibleAnywhere)
	float delta;

	UPROPERTY(VisibleAnywhere)
	TArray<UMatch *> matchesPending;

	double ratingPending;
	double deviationPending;
	double volatilityPending;
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Rating.generated.h"

struct FMatch;

/// @brief Glicko rating object.
UCLASS(BlueprintType)
class UEGLICKO_API URating : public UObject
{
	GENERATED_BODY()
public:
	/// @brief Class constructor
	URating();

	/// @brief Creates a new rating with the specified rating, deviation and volatility.
	/// @param r Glicko-1 rating
	/// @param d Glicko-1 deviation
	/// @param v Glicko-2 volatility
	/// @return Rating
	UFUNCTION(BlueprintPure, meta = (ReturnDisplayName = "Rating", Keywords = "Glicko Rating Make Rating Detailed"), Category = "UE Glicko|Rating")
	static URating *MakeRating(double r = 1500, double d = 350, double v = 0.06);

	/// @brief Creates a new rating with the default rating values.
	/// @return Rating
	UFUNCTION(BlueprintPure, meta = (ReturnDisplayName = "Rating", Keywords = "Glicko Rating Make Rating"), Category = "UE Glicko|Rating")
	static URating *MakeRatingSimple();

	/// @brief Updates the rating based on the specified matches.
	/// @param matches Matches to calculate for
	/// @ref FMatch
	/// @see Apply
	/// @see Decay
	/// @see UpdateMatch
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Glicko Rating Update Matches"), Category = "UE Glicko|Rating")
	void UpdateMatches(TArray<FMatch> matches);

	/// @brief Updates the rating based on the specified match.
	/// @param match Match to calculate for
	/// @ref FMatch
	/// @see Apply
	/// @see Decay
	/// @see UpdateMatches
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Glicko Rating Update Match"), Category = "UE Glicko|Rating")
	void UpdateMatch(FMatch match);

	/// @brief Decays the rating deviation if no games were played.
	/// @see Apply
	/// @see UpdateMatch
	/// @see UpdateMatches
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Glicko Rating Decay"), Category = "UE Glicko|Rating")
	void Decay();

	/// @brief Applies the pending rating values to the actual rating values.
	/// @see Decay
	/// @see UpdateMatch
	/// @see UpdateMatches
	UFUNCTION(BlueprintCallable, meta = (Keywords = "Glicko Rating Apply"), Category = "UE Glicko|Rating")
	void Apply();

	/// @brief Get Glicko-1 rating
	/// @return rating
	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "Get Rating 1", Keywords = "Glicko Rating Get Rating 1"), Category = "UE Glicko|Rating")
	double getRating1();

	/// @brief Get Glicko-1 deviation
	/// @return deviation
	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "Get Deviation 1", Keywords = "Glicko Rating Get Deviation 1"), Category = "UE Glicko|Rating")
	double getDeviation1();

	/// @brief Get Glicko-1 delta
	/// @return delta
	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "Get Delta 1", Keywords = "Glicko Rating Get Delta 1"), Category = "UE Glicko|Rating")
	double getDelta1();

	/// @brief Get Glicko-2 rating
	/// @return rating
	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "Get Rating 2", Keywords = "Glicko Rating Get Rating 2"), Category = "UE Glicko|Rating")
	double getRating2();

	/// @brief Get Glicko-2 deviation
	/// @return deviation
	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "Get Deviation 2", Keywords = "Glicko Rating Get Deviation 2"), Category = "UE Glicko|Rating")
	double getDeviation2();

	/// @brief Get Glicko-2 volatility
	/// @return volatility
	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "Get Volatility 2", Keywords = "Glicko Rating Get Volatility 2"), Category = "UE Glicko|Rating")
	double getVolatility2();

	/// @brief Get Glicko-2 delta
	/// @return delta
	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "Get Delta 2", Keywords = "Glicko Rating Get Delta 2"), Category = "UE Glicko|Rating")
	double getDelta2();

	/// @brief Get rating in Glicko-1 format
	/// @return Glicko-1 string rating
	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "Get Glicko 1", Keywords = "Glicko Rating Get Glicko 1"), Category = "UE Glicko|Rating")
	FString getGlicko1();

	/// @brief Get rating in Glicko-2 format
	/// @return Glicko-2 string rating
	UFUNCTION(BlueprintPure, meta = (CompactNodeTitle = "Get Glicko 2", Keywords = "Glicko Rating Get Glicko 2"), Category = "UE Glicko|Rating")
	FString getGlicko2();

private:
	/// @brief The rating; µ (mu)
	UPROPERTY(VisibleAnywhere)
	double Rating;

	/// @brief The rating deviation; φ (phi)
	UPROPERTY(VisibleAnywhere)
	double Deviation;

	/// @brief The rating volatility; σ (sigma)
	UPROPERTY(VisibleAnywhere)
	double Volatility;

	/// @brief The rating delta; Δ (delta)
	UPROPERTY(VisibleAnywhere)
	double Delta;

	/// @brief The pending rating value; µ' (mu prime)
	double RatingPending;
	/// @brief The pending deviation value; φ' (phi prime)
	double DeviationPending;
	/// @brief The pending volatility value; σ' (sigma prime)
	double VolatilityPending;

	/// @brief Computes the value of the g function for a rating
	/// @return
	double G();
	/// @brief Computes the value of the e function in terms of a g function value and another rating
	/// @param g
	/// @param rating
	/// @return
	double E(double g, URating* rating);
	/// @brief Computes the value of the f function in terms of x, delta^2, phi^2,v, a and tau^2.
	/// @param x
	/// @param dS
	/// @param pS
	/// @param v
	/// @param a
	/// @param tS
	/// @return
	static double F(double x, double dS, double pS, double v, double a, double tS);
	/// @brief Performs convergence iteration on the function f
	/// @param d
	/// @param v
	/// @param p
	/// @param s
	/// @return
	static double Convergence(double d, double v, double p, double s);
};

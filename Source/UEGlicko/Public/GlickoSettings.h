// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GlickoSettings.generated.h"

/**
 * 
 */
UCLASS(config=Game, defaultconfig, meta=(DisplayName="UE Glicko"))
class UEGLICKO_API UGlickoSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UGlickoSettings();

	/// @brief Get default GlickoSettings object
	/// @return GlickoSettings
	/// @see GetDefault<UGlickoSettings>()
	UFUNCTION(BlueprintCallable, meta = (ReturnDisplayName = "Glicko", Keywords = "Glicko Settings"), Category = "UE Glicko")
	static const UGlickoSettings* GetGlickoSettings();

	/// @brief The default/initial rating value; µ (mu)
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, meta = (Keywords = "Glicko Default Rating"), Category = "UE Glicko|Default Rating")
	double DefaultRating; // 1500.0;

	/// @brief The default/initial deviation value; φ (phi)
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, meta = (Keywords = "Glicko Default Deviation"), Category = "UE Glicko|Default Rating")
	double DefaultDeviation; // 350.0;

	/// @brief The default/initial volatility value; σ (sigma)
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, meta = (Keywords = "Glicko Default Volatility"), Category = "UE Glicko|Default Rating")
	double DefaultVolatility; // 0.06;

	/// @brief The Glicko-1 to Glicko-2 scale factor
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, meta = (Keywords = "Glicko Scale"), Category = "UE Glicko|Glicko")
	double Scale; // 173.7178;

	/// @brief The system constant; τ (tau)
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, meta = (Keywords = "Glicko System Constant"), Category = "UE Glicko|Glicko")
	double SystemConst; // 0.5;

	/// @brief The convergence constant; ε (epsilon)
	UPROPERTY(config, EditAnywhere, BlueprintReadOnly, meta = (Keywords = "Glicko Convergence"), Category = "UE Glicko|Glicko")
	double Convergence; // 0.000001;
};


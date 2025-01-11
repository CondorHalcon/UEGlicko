// Fill out your copyright notice in the Description page of Project Settings.

#include "Rating.h"
#include "Match.h"
#include "GlickoSettings.h"
#include "Math/UnrealMathUtility.h"

URating::URating()
{
	static const UGlickoSettings* Glicko = UGlickoSettings::GetGlickoSettings();
	this->Rating = 0; // (DefaultRating - DefaultRating) / Scale;
	this->Deviation = Glicko->DefaultDeviation / Glicko->Scale;
	this->Volatility = Glicko->DefaultVolatility;
	this->Delta = 0.0;
	// pending
	this->RatingPending = this->Rating;
	this->DeviationPending = this->Deviation;
	this->VolatilityPending = this->Volatility;
}

URating* URating::MakeRating(double r, double d, double v)
{
	static const UGlickoSettings* Glicko = UGlickoSettings::GetGlickoSettings();
	URating* rating = NewObject<URating>();
	rating->Rating = (r - Glicko->DefaultRating) / Glicko->Scale;
	rating->Deviation = d / Glicko->Scale;
	rating->Volatility = v;
	return rating;
}

URating* URating::MakeRatingSimple()
{
	return NewObject<URating>();
}

#pragma region Changing Rating

void URating::UpdateMatches(TArray<FMatch> matches)
{
	TArray<double> gTable = TArray<double>();
	gTable.Init(0.0, matches.Num());
	TArray<double> eTable = TArray<double>();
	eTable.Init(0.0, matches.Num());
	double invV = 0.0;

	// Compute the g and e values for each opponent and  accumulate the results into the v value
	for (int i = 0; i < matches.Num(); i++) {
		double g = matches[i].Opponent->G();
		double e = matches[i].Opponent->E(g, this);

		gTable[i] = g;
		eTable[i] = e;
		invV += g * g * e * (1.0 - e);
	}

	// Invert the v value
	double v = 1.0 / (invV == 0 ? .000001 : invV);

	// Compute the delta value from the g, e, and v values
 	double dInner = 0.0;
	for (int j = 0; j < matches.Num(); j++) {
		dInner += gTable[j] * (matches[j].Score - eTable[j]);
	}

	// Apply the v value to the delta
	double d = v * dInner;

	// Compute new rating, deviation and volatility values
	VolatilityPending = FMath::Exp(Convergence(d, v, Deviation, Volatility) / 2.0);
	DeviationPending = 1.0 / FMath::Sqrt((1.0 / (Deviation * Deviation + VolatilityPending * VolatilityPending)) + invV);
	RatingPending = Rating + DeviationPending * DeviationPending * dInner;
}

void URating::UpdateMatch(FMatch match)
{
	TArray<FMatch> mArr = TArray<FMatch>();
	mArr.Add(match);
	UpdateMatches(mArr);
}

void URating::Decay()
{
	RatingPending = Rating;
	DeviationPending = FMath::Sqrt(Deviation * Deviation + Volatility * Volatility);
	VolatilityPending = Volatility;
}

void URating::Apply()
{
	Delta = RatingPending - Rating;
	Rating = RatingPending;
	Deviation = DeviationPending;
	Volatility = VolatilityPending;
}

#pragma endregion

double URating::getRating1()
{
	static const UGlickoSettings* Glicko = UGlickoSettings::GetGlickoSettings();
	return (this->Rating * Glicko->Scale) + Glicko->DefaultRating;
}

double URating::getDeviation1()
{
	static const UGlickoSettings* Glicko = UGlickoSettings::GetGlickoSettings();
	return this->Deviation * Glicko->Scale;
}

double URating::getDelta1()
{
	static const UGlickoSettings* Glicko = UGlickoSettings::GetGlickoSettings();
	return this->Delta * Glicko->Scale;
}

double URating::getRating2()
{
	return this->Rating;
}

double URating::getDeviation2()
{
	return this->Deviation;
}

double URating::getVolatility2()
{
	return this->Volatility;
}

double URating::getDelta2()
{
	return this->Delta;
}

FString URating::getGlicko1()
{
	return FString::Printf(TEXT("µ%d±φ%d"), (int)getRating1(), (int)getDeviation1());
}

FString URating::getGlicko2()
{
	return FString::Printf(TEXT("µ%d±φ%d:σ%d"), (int)getRating2(), (int)getDeviation2(), (int)getVolatility2());
}

#pragma region Math Functions

double URating::G()
{
	double scale = Deviation / PI;
	return 1.0 / FMath::Sqrt(1.0 + 3.0 * scale * scale);
}

double URating::E(double g, URating* r)
{
	double exponent = -1.0 * g * (r->Rating - this->Rating);
	return 1.0 / (1.0 + FMath::Exp(exponent));
}

double URating::F(double x, double dS, double pS, double v, double a, double tS)
{
	double eX = FMath::Exp(x);
	double num = eX * (dS - pS - v - eX);
	double den = pS + v + eX;

	return (num / (2.0 * den * den)) - ((x - a) / tS);
}

double URating::Convergence(double d, double v, double p, double s)
{
	static const UGlickoSettings* Glicko = UGlickoSettings::GetGlickoSettings();

	// Initialize function values for iteration procedure
	double dS = d * d;
	double pS = p * p;
	double tS = Glicko->SystemConst * Glicko->SystemConst;
	double a = FMath::Log2(s * s);

	// Select the upper and lower iteration ranges
	double A = a;
	double B;
	double bTest = dS - pS - v;

	if (bTest > 0.0)
	{
		B = FMath::Log2(bTest);
	}
	else
	{
		B = a - Glicko->SystemConst;
		while (F(B, dS, pS, v, a, tS) < 0.0)
		{
			B -= Glicko->SystemConst;
		}
	}

	double fA = F(A, dS, pS, v, a, tS);
	double fB = F(B, dS, pS, v, a, tS);
	while (FMath::Abs(B - A) > Glicko->Convergence)
	{
		double C = A + (A - B) * fA / (fB - fA);
		double fC = F(C, dS, pS, v, a, tS);

		if (fC * fB < 0.0)
		{
			A = B;
			fA = fB;
		}
		else
		{
			fA /= 2.0;
		}

		B = C;
		fB = fC;
	}

	return A;
}

#pragma endregion

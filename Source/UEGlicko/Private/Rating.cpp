// Fill out your copyright notice in the Description page of Project Settings.

#include "Rating.h"
#include "Match.h"
#include "GlickoSettings.h"
#include "Math/UnrealMathUtility.h"

URating::URating()
{
	static const UGlickoSettings* Glicko = UGlickoSettings::GetGlickoSettings();
	this->rating = 0; // (DefaultRating - DefaultRating) / Scale;
	this->deviation = Glicko->DefaultDeviation / Glicko->Scale;
	this->volatility = Glicko->DefaultVolatility;
	this->delta = 0.0;
	// pending
	this->ratingPending = this->rating;
	this->deviationPending = this->deviation;
	this->volatilityPending = this->volatility;
}

URating* URating::MakeRating(double r, double d, double v)
{
	static const UGlickoSettings* Glicko = UGlickoSettings::GetGlickoSettings();
	URating* rating = NewObject<URating>();
	rating->rating = (r - Glicko->DefaultRating) / Glicko->Scale;
	rating->deviation = d / Glicko->Scale;
	rating->volatility = v;
	return rating;
}

URating* URating::MakeRatingSimple()
{
	return NewObject<URating>();
}

#pragma region Changing Rating

void URating::UpdateMatches(TArray<UMatch*> matches)
{
	TArray<double> gTable = TArray<double>();
	gTable.Init(0.0, matches.Num());
	TArray<double> eTable = TArray<double>();
	eTable.Init(0.0, matches.Num());
	double invV = 0.0;

	// Compute the g and e values for each opponent and 
	// accumulate the results into the v value
	for (int i = 0; i < matches.Num(); i++) {
		URating* opponent = matches[i]->getOpponent();
		double g = opponent->G();
		double e = opponent->E(g, this);

		gTable[i] = g;
		eTable[i] = e;
		invV += g * g * e * (1.0 - e);
	}

	// Invert the v value
	double v = 1.0 / (invV == 0 ? .000001 : invV);

	// Compute the delta value from the g, e, and v values
 	double dInner = 0.0;
	for (int j = 0; j < matches.Num(); j++) {
		dInner += gTable[j] * (matches[j]->getScore() - eTable[j]);
	}

	// Apply the v value to the delta
	double d = v * dInner;

	// Compute new rating, deviation and volatility values
	volatilityPending = FMath::Exp(Convergence(d, v, deviation, volatility) / 2.0);
	deviationPending = 1.0 / FMath::Sqrt((1.0 / (deviation * deviation + volatilityPending * volatilityPending)) + invV);
	ratingPending = rating + deviationPending * deviationPending * dInner;
}

void URating::UpdateMatch(UMatch* match)
{
	TArray<UMatch*> mArr = TArray<UMatch*>();
	mArr.Add(match);
	UpdateMatches(mArr);
}

void URating::Decay()
{
	ratingPending = rating;
	deviationPending = FMath::Sqrt(deviation * deviation + volatility * volatility);
	volatilityPending = volatility;
}

void URating::Apply()
{
	delta = ratingPending - rating;
	rating = ratingPending;
	deviation = deviationPending;
	volatility = volatilityPending;
}

#pragma endregion

double URating::getRating1()
{
	static const UGlickoSettings* Glicko = UGlickoSettings::GetGlickoSettings();
	return (this->rating * Glicko->Scale) + Glicko->DefaultRating;
}

double URating::getDeviation1()
{
	static const UGlickoSettings* Glicko = UGlickoSettings::GetGlickoSettings();
	return this->deviation * Glicko->Scale;
}

double URating::getDelta1()
{
	static const UGlickoSettings* Glicko = UGlickoSettings::GetGlickoSettings();
	return this->delta * Glicko->Scale;
}

double URating::getRating2()
{
	return this->rating;
}

double URating::getDeviation2()
{
	return this->deviation;
}

double URating::getVolatility2()
{
	return this->volatility;
}

double URating::getDelta2()
{
	return this->delta;
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
	double scale = deviation / PI;
	return 1.0 / FMath::Sqrt(1.0 + 3.0 * scale * scale);
}

double URating::E(double g, URating* r)
{
	double exponent = -1.0 * g * (r->rating - this->rating);
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

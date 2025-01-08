// Fill out your copyright notice in the Description page of Project Settings.

#include "Rating.h"
#include "Match.h"
#include "Math/UnrealMathUtility.h"

URating::URating()
{
	this->rating = 1500;
	this->deviation = 350;
	this->volatility = 0.06;
}

URating* URating::MakeRating(double r, double d, double v)
{
	URating* rating = NewObject<URating>();
	rating->rating = r;
	rating->deviation = d;
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
	TArray<double> eTable = TArray<double>();
	double invV = 0.0;

	for (int i = 0; i < matches.Num(); i++) {
		URating* opponent = matches[i]->getOpponent();
		double g = opponent->G();
		double e = opponent->E(g, this);

		gTable[i] = g;
		eTable[i] = e;
		invV += g * g * e * (1.0 - e);
	}

	double v = 1.0 / invV;

	double dInner = 0.0;
	for (int j = 0; j < matches.Num(); j++) {
		dInner += gTable[j] * (matches[j]->getScore() - eTable[j]);
	}

	double d = v * dInner;

	ratingPending = FMath::Exp(Convergence(d, v, deviation, volatility) / 2.0);
	deviationPending = 1.0 / FMath::Sqrt((1.0 / (deviation * deviation + ratingPending * ratingPending)) + invV);
	volatilityPending = rating + deviationPending * deviationPending * dInner;
}

void URating::UpdateMatch(UMatch* match)
{
	TArray<UMatch*> mArr;
	mArr.Add(match);
	UpdateMatches(mArr);
}

void URating::UpdatePending()
{
	UpdateMatches(matchesPending);
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
	return (this->rating * URating::Scale) + URating::DefaultRating;
}

double URating::getDeviation1()
{
	return this->deviation * URating::Scale;
}

double URating::getDelta1()
{
	return this->delta * URating::Scale;
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
	return FString::Printf(TEXT("[µ%02d:φ%02d]"), getRating1(), getDeviation1());
}

FString URating::getGlicko2()
{
	return FString::Printf(TEXT("[µ%02d:φ%02d:σ%02d]"), getRating2(), getDeviation2(), getVolatility2());
}

#pragma region Math Functions

double URating::G()
{
	double scale = deviation / PI;
	return 1.0 / FMath::Sqrt(1.0 + 3.0 * scale * scale);
}

double URating::E(double g, URating* r)
{
	double exponent = -1.0 * g * r->rating - this->rating;
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
	double dS = d * d;
	double pS = p * p;
	double tS = URating::SystemConst * URating::SystemConst;
	double a = FMath::Log2(s * s);

	double A = a;
	double B;
	double bTest = dS - pS - v;

	if (bTest > 0.0)
	{
		B = FMath::Log2(bTest);
	}
	else
	{
		B = a - URating::SystemConst;
		while (F(B, dS, pS, v, a, tS) < 0.0)
		{
			B -= URating::SystemConst;
		}
	}

	double fA = F(A, dS, pS, v, a, tS);
	double fB = F(B, dS, pS, v, a, tS);
	while (FMath::Abs(B - A) > URating::ConvergenceD)
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

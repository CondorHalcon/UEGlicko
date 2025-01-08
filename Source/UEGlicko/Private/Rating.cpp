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

void URating::UpdateMatches(TArray<UMatch*> Matches)
{
	TArray<double> gTable = TArray<double>();
	TArray<double> eTable = TArray<double>();
	double invV = 0.0;

	for (int i = 0; i < Matches.Num(); i++) {
		URating* opponent = Matches[i]->getOpponent();
		double g = opponent->G();
		double e = opponent->E(g, this);

		gTable[i] = g;
		eTable[i] = e;
		invV += g * g * e * (1.0 - e);
	}

	double v = 1.0 / invV;

	double dInner = 0.0;
	for (int j = 0; j < Matches.Num(); j++) {
		dInner += gTable[j] * (Matches[j]->getScore() - eTable[j]);
	}

	double d = v * dInner;

	ratingPending = FMath::Exp(Convergence(d, v, deviation, volatility) / 2.0);
	deviationPending = 1.0 / FMath::Sqrt((1.0 / (deviation * deviation + ratingPending * ratingPending)) + invV);
	volatilityPending = rating + deviationPending * deviationPending * dInner;
}

void URating::UpdateMatch(UMatch* Match)
{
}

void URating::UpdatePending()
{
}

void URating::Decay()
{
}

void URating::Apply()
{
}

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

double URating::G()
{
	return 0.0;
}

double URating::E(double g, URating* rating)
{
	return 0.0;
}

double URating::F(double x, double dS, double pS, double v, double a, double tS)
{
	return 0.0;
}

double URating::Convergence(double d, double v, double p, double s)
{
	return 0.0;
}

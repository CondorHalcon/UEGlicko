// Fill out your copyright notice in the Description page of Project Settings.

#include "Rating.h"
#include "Match.h"

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

float URating::getRating1()
{
	return 0.0f;
}

float URating::getDeviation1()
{
	return 0.0f;
}

float URating::getVolatility1()
{
	return 0.0f;
}

float URating::getRating2()
{
	return 0.0f;
}

float URating::getDeviation2()
{
	return 0.0f;
}

float URating::getVolatility2()
{
	return 0.0f;
}

FString URating::getGlicko1()
{
	return FString();
}

FString URating::getGlicko2()
{
	return FString();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GlickoSettings.h"

UGlickoSettings::UGlickoSettings()
{
	this->DefaultRating = 1500.0;
	this->DefaultDeviation = 350.0;
	this->DefaultVolatility = 0.06;
	this->Scale = 173.7178;
	this->SystemConst = 0.5;
	this->Convergence = 0.000001;
}

const UGlickoSettings* UGlickoSettings::GetGlickoSettings()
{
	return GetDefault<UGlickoSettings>();
}

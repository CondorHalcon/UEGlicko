// Fill out your copyright notice in the Description page of Project Settings.

#include "Match.h"
#include "Rating.h"

UMatch::UMatch()
{
    this->opponent = nullptr;
    this->_score = 0;
}

URating* UMatch::getOpponent()
{
    return this->opponent;
}

float UMatch::getScore()
{
    return _score;
}

UMatch* UMatch::MakeMatch(URating* opponent, float score)
{
    UMatch* match = NewObject<UMatch>();
    match->opponent = opponent;
    match->_score = score;
    return match;
}

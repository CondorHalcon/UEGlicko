// Fill out your copyright notice in the Description page of Project Settings.

#include "Match.h"
#include "Rating.h"

FMatch UMatchHelperLibrary::MakeMatch(URating *opponent, float score)
{
    FMatch match;
    match.Opponent = opponent;
    match.Score = score;
    return match;
}

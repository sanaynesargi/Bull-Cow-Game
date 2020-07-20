// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

// error shows, IntelliSense bug
struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;
	void SetupGame();
	void EndGame();
	void ProcessGuess(const FString &input);
	TCHAR IsIsogram(const FString &word);
	FBullCowCount GetBullCows(const FString &Guess) const;
	TArray<FString> GetIsogramArray(const TArray<FString> &Words, int32 &min, int32 &max);
	FString SetMode(const FString &mode) const;

	// Your declarations go below!
	private:
	FString HiddenWord;
	int32 Lives;
	bool bGameOver;
	TArray<FString> IsogramArray;
	int32 min;
	int32 max;
	bool bSetMode = false;
};

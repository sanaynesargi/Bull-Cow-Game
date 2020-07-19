// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    SetupGame();
    PrintLine(TEXT("--> The Hidden Word is: %s"), *HiddenWord); // debug statement
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    // check if game is over
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    } else {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::GetBullCows(const FString &Guess, int32 &BullCount,int32 &CowCount) const
{
    BullCount = 0;
    CowCount = 0;

    // for every index of guess is same as index Hidden, BullCount++
    // if not a bull was it a cow, if yes CowCount++

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            BullCount++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                CowCount++;
            }
            
        }
        
    }
}

void UBullCowCartridge::SetupGame()
{
    // print welcome message
    IsogramArray = GetIsogramArray(HiddenWords);

    HiddenWord = IsogramArray[FMath::FRand() * IsogramArray.Num() - 1]; // set hidden word
    Lives = HiddenWord.Len(); // set lives
    bGameOver = false;
    PrintLine(TEXT("--> Hello, Welcome to Bull Cows!!\n--> Press Enter to Begin.\n--> Press tab to write to the terminal.\n--> Guess the %i letter word.\n--> You have %i lives.\n--> You have %d word possibtiliies. Good Luck!!"), HiddenWord.Len(), Lives, HiddenWords.Num());
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("--> Press Enter for new game"));
}

void UBullCowCartridge::ProcessGuess(const FString &input)
{

    bool bClearWord = false; // variable to keep track of clear screen message
    bool bHelpWord = false; // variable to keep track of help message
    bool bWrongWordType = false; // variable too keep track of whether the user has typed in the wrong type of word, not the right amount of chars

    // handle clear sceen message, messages are cls, l, and clear for linux, mac, and pc users
    if ((input == TEXT("cls")) || (input == TEXT("clear")) || (input == TEXT("l")))
    {
        ClearScreen();
        // PrintLine(TEXT("--> Type in assistance at any time to get the instructions of the game and some useful Tips and Tricks."));
        bClearWord = true;
    } else {
        bClearWord = false;
    }

    // show lives
    // check if word is correct
    if (HiddenWord == input)
    {
        PrintLine(TEXT("--> Correct!! You got the word!! The word was %s"), *HiddenWord);
        EndGame();
        return;
    }

    if (bClearWord)
    {
        PrintLine(TEXT("--> Please Type in the %i letter word."), HiddenWord.Len());
        PrintLine(TEXT("--> You have %d lives remaining"), Lives);
        return;
    }
    // check if word is isogram
    if (IsIsogram(input) == TCHAR('F'))
    {
        // word is not isogram
        PrintLine(TEXT("%s is not an isogram, please try again"), *input);
        bWrongWordType = true;
    }

    // check if word has correct number of chars
    if (HiddenWord.Len() != input.Len() && !bClearWord)
    {
        if (HiddenWord.Len() < input.Len())
        {
            PrintLine(TEXT("--> Incorrect, The word has too many characters.\nThe Hidden Word has %i characters"), HiddenWord.Len());
            // PrintLine(TEXT("--> Please try again"));
            // PrintLine(TEXT("--> You have lost."));
            // EndGame();
        } else {
            PrintLine(TEXT("--> Incorrect, The word doesn't have enough characters.\nThe Hidden Word has %i characters"), HiddenWord.Len());
            // PrintLine(TEXT("--> Please try again"));
            // EndGame();               
        }
        bWrongWordType = true;
    }

    PrintLine(FString::Printf(TEXT("--> Incorrect, The Hidden Word has %i characters, try again."), HiddenWord.Len()));
    // check if word is isogram

    if (Lives - 1 <= 0)
    {
        PrintLine(TEXT("You have failed misearably, and the word wasn't even that hard."));
        PrintLine(TEXT("For your poor soul, I will tell you the word, %s"), *HiddenWord);
        EndGame();
        return;
    }
    // only decrement a life when the word is of the correct type
    if (bWrongWordType)
    {
        PrintLine(TEXT("You have %d lives left"), Lives);                                                                  
        // set the bWrongWordType variable to false for future words
        bWrongWordType = false;
        
    } else {
        int32 Bulls, Cows;
        GetBullCows(input, Bulls, Cows);
        
        PrintLine(TEXT("You have %i bull (s) and %i cow (s)"), Bulls, Cows);

        PrintLine(TEXT("You have lost a life, you have %d lives left :("), --Lives); // decrement a life within the statement
    }
    // check if lives are greater than 0
    // if yes then GuessAgain
    // if no then print fail message, and HiddenWord?
    // press enter to play again
}

TArray<FString> UBullCowCartridge::GetIsogramArray(const TArray<FString> &Words)
{
    TArray<FString> Isograms;

    for (int32 i = 0; i < Words.Num(); i++)
    {
        if (IsIsogram(Words[i]) == TCHAR('T'))
        {
            Isograms.Emplace(Words[i]);
        }
    }

    return Isograms;
}

TCHAR UBullCowCartridge::IsIsogram(const FString &word)
{
    for (int32 i = 0; i < word.Len(); i++)
    {
        for (int32 j = 0; j < word.Len(); j++)
        {
            if (word[i] == word[j] && i != j)
            {
                return TCHAR('F');
            }
         }   }
    return TCHAR('T');
}

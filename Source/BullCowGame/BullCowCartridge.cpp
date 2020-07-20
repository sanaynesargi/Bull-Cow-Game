// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    SetupGame();
    // PrintLine(TEXT("--> The Hidden Word is: %s"), *HiddenWord); // debug statement
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

FBullCowCount UBullCowCartridge::GetBullCows(const FString &Guess) const
{
    // for every index of guess is same as index Hidden, BullCount++
    // if not a bull was it a cow, if yes CowCount++

    int32 BullCount = 0;
    int32 CowCount = 0;

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
                break;
            }
            
        }
        
    }

    FBullCowCount Count = {BullCount, CowCount};
    return Count;
}

void UBullCowCartridge::SetupGame()
{
    bGameOver = false;
    PrintLine(TEXT("--> Hello, Welcome to Bull Cows!!\n--> Press Enter to Begin.\n--> Press tab to write to the terminal.\n"));
    PrintLine(TEXT("Would you like to play easy, medium, or hard mode?"));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("--> Press Enter for new game"));
}

void UBullCowCartridge::ProcessGuess(const FString &input)
{
    if (bSetMode == false)
    {
        FString mode = SetMode(input);
        if (mode == TEXT("easy"))
        {
            min = 4;
            max = 5;
            IsogramArray = GetIsogramArray(HiddenWords, min, max); // fix
            HiddenWord = IsogramArray[FMath::FRand() * IsogramArray.Num() - 1]; // set hidden word
            bSetMode = true;
            Lives = HiddenWord.Len(); // set lives
            PrintLine(TEXT("The word is %d letters long and you have %d lives, Good Luck"), Lives, Lives);
        }
        else if (mode == TEXT("medium"))
        {
            min = 6;
            max = 8;
            IsogramArray = GetIsogramArray(HiddenWords, min, max); // fix
            HiddenWord = IsogramArray[FMath::FRand() * IsogramArray.Num() - 1]; // set hidden word
            bSetMode = true;
            Lives = HiddenWord.Len(); // set lives
            PrintLine(TEXT("The word is %d letters long and you have %d lives, Good Luck"), Lives, Lives);
        }
        else if (mode == TEXT("hard"))
        {
            min = 9;
            max = 10;
            IsogramArray = GetIsogramArray(HiddenWords, min, max); // fix
            HiddenWord = IsogramArray[FMath::FRand() * IsogramArray.Num() - 1]; // set hidden word
            bSetMode = true;
            Lives = HiddenWord.Len(); // set lives
            PrintLine(TEXT("The word is %d letters long and you have %d lives, Good Luck"), Lives, Lives);
        }
        else
        {
            PrintLine(TEXT("Invalid difficulty random difficulty will be chosen"));
            min = 4;
            max = 10;
            IsogramArray = GetIsogramArray(HiddenWords, min, max); // fix
            HiddenWord = IsogramArray[FMath::FRand() * IsogramArray.Num() - 1]; // set hidden word
            Lives = HiddenWord.Len(); // set lives
            bSetMode = true;
        }
        PrintLine(TEXT("Your difficulty is set have fun!!"));
        PrintLine(TEXT("--> Guess the %d letter word"), Lives);
        return;
    }
    if (bSetMode == true)
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
            FBullCowCount BullCows = GetBullCows(input);
            
            PrintLine(TEXT("You have %i bull (s) and %i cow (s)"), BullCows.Bulls, BullCows.Cows);

            PrintLine(TEXT("You have lost a life, you have %d lives left :("), --Lives); // decrement a life within the statement
        }
    }
}

TArray<FString> UBullCowCartridge::GetIsogramArray(const TArray<FString> &Words, int32 &min, int32 &max)
{
    TArray<FString> Isograms;

    for (int32 i = 0; i < Words.Num(); i++)
    {
        if (IsIsogram(Words[i]) == TCHAR('T') && Words[i].Len() >= min && Words[i].Len() <= max)
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

FString UBullCowCartridge::SetMode(const FString &text) const
{
    FString difficulty;

    if (text == TEXT("easy"))
    {
        difficulty = TEXT("easy");
    }

    else if (text == TEXT("medium"))
    {
        difficulty = TEXT("easy");
    }

    else if (text == TEXT("hard"))
    {
        difficulty = TEXT("hard");
    }

    else
    {
        difficulty = TEXT("INVALID");
    }

    return difficulty;
}
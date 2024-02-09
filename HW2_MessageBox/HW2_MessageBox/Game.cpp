#define _CRT_SECURE_NO_WARNINGS
#include <windows.h> 
#include <tchar.h> 

INT WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    bool bIsGame = true;
    while (bIsGame)
    {
        int nMinRange = 1;
        int nMaxRange = 100;
        int nTriesCount = 0;

        MessageBox(0, TEXT("Think of a number between 1 and 100!"), TEXT("Think of a number"), MB_OK | MB_ICONINFORMATION);
        bool bIsFound = false;

        while (!bIsFound)
        {
            int nGuessNum = (nMinRange + nMaxRange) / 2;
            TCHAR szMessage[100];

            _stprintf_s(szMessage, TEXT("Is your number greater than %d?"), nGuessNum);
            int nResult = MessageBox(0, szMessage, TEXT("Think of a number"), MB_YESNO | MB_ICONQUESTION);

            if (nResult == IDYES)
            {
                nMinRange = nGuessNum + 1;
            }
            else
            {
                nMaxRange = nGuessNum - 1;
            }

            if (nMinRange > 100 || nMaxRange < 1)
            {
                MessageBox(0, TEXT("Your number is out of range."), TEXT("Out of range"), MB_OK | MB_ICONERROR);
                bIsFound = true;
            }
            else if (nMinRange > nMaxRange)
            {
                bIsFound = true;
                _stprintf_s(szMessage, TEXT("Your number: %d\nTries: %d"), nMinRange, nTriesCount);
                MessageBox(0, szMessage, TEXT("Number has been guessed"), MB_OK | MB_ICONINFORMATION);
            }

            nTriesCount++;
        }

        int nPlayAgain = MessageBox(0, TEXT("Play again?"), TEXT(""), MB_YESNO | MB_ICONQUESTION);
        if (nPlayAgain != IDYES)
        {
            bIsGame = false;
        }
    }

    return 0;
}

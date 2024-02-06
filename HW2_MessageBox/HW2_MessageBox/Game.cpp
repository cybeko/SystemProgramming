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

        MessageBox(0, TEXT("Загадайте число от 1 до 100!"), TEXT("Загадайте число"), MB_OK | MB_ICONINFORMATION);
        bool bIsFound = false;

        while (!bIsFound)
        {
            int nGuessNum = (nMinRange + nMaxRange) / 2;
            TCHAR szMessage[100];

            _stprintf_s(szMessage, TEXT("Ваше число больше %d?"), nGuessNum);
            int nResult = MessageBox(0, szMessage, TEXT("Загадайте число"), MB_YESNO | MB_ICONQUESTION);

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
                MessageBox(0, TEXT("Ваше число вне диапазона от 1 до 100."), TEXT("Вне диапазона"), MB_OK | MB_ICONERROR);
                bIsFound = true;
            }
            else if (nMinRange > nMaxRange)
            {
                bIsFound = true;
                _stprintf_s(szMessage, TEXT("Ваше число: %d\nКоличество попыток: %d"), nMinRange, nTriesCount);
                MessageBox(0, szMessage, TEXT("Число угадано"), MB_OK | MB_ICONINFORMATION);
            }

            nTriesCount++;
        }

        int nPlayAgain = MessageBox(0, TEXT("Сыграть еще раз?"), TEXT(""), MB_YESNO | MB_ICONQUESTION);
        if (nPlayAgain != IDYES)
        {
            bIsGame = false;
        }
    }

    return 0;
}

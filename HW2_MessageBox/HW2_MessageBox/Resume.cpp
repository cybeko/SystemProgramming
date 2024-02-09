#define _CRT_SECURE_NO_WARNINGS
#include <windows.h> 
#include <tchar.h> 

INT WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    const int nElementsLen = 150;

    TCHAR szResume[][nElementsLen] = {
        TEXT("Resume: \nJane M. Smith"),
        TEXT("Contact Information:\nAddress: 456 Oak Street, City, USA\nPhone: (555) 987 - 6543\nEmail: jane.smith@email.com"),
        TEXT("Education:\nBachelor's Degree in Marketing, Springfield University, USA"),
        TEXT("Additional Information:\nDigital Marketing Certificate from Google\nParticipation in the Marketing Conference \"Global Trends 2023\"")
    };

    const int nSize = sizeof(szResume) / sizeof(szResume[0]);

    TCHAR szTitle[20];
    for (int i = 0;i < nSize; i++)
    {
        _stprintf(szTitle, TEXT("Page#%d"), i + 1);
        MessageBox(0,
            szResume[i],
            szTitle,
            MB_OK);
    }

    int nCount = 0;
    for (int i = 0;i < nSize; i++)
    {
        nCount += lstrlen(szResume[i]);
    }
    nCount /= nSize;

    TCHAR bufCount[300];
    _stprintf(bufCount, TEXT("%d"), nCount);
    MessageBox(0, bufCount, TEXT("Average symbol count:"), MB_OK | MB_ICONINFORMATION);

    return 0;
}
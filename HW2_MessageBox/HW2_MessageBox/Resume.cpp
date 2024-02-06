#define _CRT_SECURE_NO_WARNINGS
#include <windows.h> 
#include <tchar.h> 

INT WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
    const int nElementsLen = 150;

    TCHAR szResume[][nElementsLen] = {
       TEXT("Резюме: \nДжейн М. Смит"),
       TEXT("Контактная информация:\nАдрес: 456 Дубовая улица, Городск, США\nТелефон: (555) 987 - 6543\nЭлектронная почта : jane.smith@email.com"),
       TEXT("Образование:\nСтепень бакалавра по маркетингу, Университет Спрингфилд, США"),
       TEXT("Дополнительная информация:\nСертификат по цифровому маркетингу от Google\nУчастие в конференции по маркетингу \"Мировые Тренды 2023\"")
    };

    const int nSize = sizeof(szResume) / sizeof(szResume[0]);

    TCHAR szTitle[20];
    for (int i = 0;i < nSize; i++)
    {
        _stprintf(szTitle, TEXT("Страница резюме#%d"), i + 1);
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
    MessageBox(0, bufCount, TEXT("Среднее количество символов:"), MB_OK | MB_ICONINFORMATION);

    return 0;
}
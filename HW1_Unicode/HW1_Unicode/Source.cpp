#define _CRT_SECURE_NO_WARNINGS 
#define _UNICODE  

#include <iostream> 
#include <tchar.h> 
#include <Windows.h> 
using namespace std;

//task 1 
void SpaceToTab()
{
	_TCHAR str[] = _TEXT("A B C D E F");
	wcout << str << endl;

	for (size_t i = 0; i < _tcslen(str); i++)
	{
        if (str[i] == _T(' '))
		{
            str[i] = _T('\t');
		}
	}
	wcout << str << endl;
}
//task 2 
void CountSymbols()
{
	_TCHAR str[] = _TEXT("Hello");
	wcout << str << endl;
	cout << "Length of Unicode-string: " << wcslen(str) << endl;
}

//task 3 
void CountWords()
{
    const int SIZE = 64;
    _TCHAR str[SIZE];

    wcout << L"Enter sentence: ";
    wcin.getline(str, SIZE);
    //wcout << str << endl;

    bool inSpace = false;
    int words = 0;

    for (size_t i = 0; i < wcslen(str); i++)
    {
        if (str[i] == ' ')
        {
            inSpace = false;
        }
        else
        {
            if (!inSpace)
            {
                words++;
                inSpace = true;
            }
        }
    }
    wcout << L"Word count: " << words << endl;
}

//task 4
void CountVowelsRU()
{
    const int SIZE = 64;
    _TCHAR str[SIZE];

    setlocale(LC_ALL, "Russian");

    wcout << L"Введите предложение: ";
    wcin.getline(str, SIZE);
    wcout << L"Введённое предложение: " << str << endl;

    int vowelCount = 0;

    for (int i = 0; str[i] < wcslen(str); ++i) 
    {
        wchar_t ch = towlower(str[i]);
        if (ch == L'е')//..
        {
            vowelCount++;
        }
    }

    wcout << L"Количество гласных: " << vowelCount << endl;
}

//task 5
void IsPalindrome()
{
    _TCHAR str[] = _TEXT("Racecar");

    int size = wcslen(str);

    for (int i = 0; i < size / 2; i++)
    {
        if (tolower(str[i]) != tolower(str[size - i - 1]))
        {
            wcout << _TEXT("Not a palindrome");
            return;
        }
    }
    wcout << _TEXT("Palindrome");
}

//task 6
void DeleteSymbolByPosition()
{
    _TCHAR str[] = _TEXT("World");
    wcout << "Before deleting: " << str << endl;

    int pos;
    cin >> pos;

    int size = wcslen(str);

    if (pos >= 0 && pos < size)
    {
        for (int i = pos; i < size - 1; i++)
        {
            str[i] = str[i + 1];
        }

        str[size - 1] = _TEXT('\0');
        wcout << "After deleting: " << str << endl;
    }
    else
    {
        wcout << "Invalid position." << endl;
    }
}

//task 7
void DeleteSymbols()
{
    _TCHAR str[] = _TEXT("Borrasca");
    wcout << "Before deleting: " << str << endl;

    _TCHAR symbol;
    wcout << "Enter symbol: ";
    wcin >> symbol;

    bool isFind = 0;
    int j = 0;
    for (int i = 0; i < wcslen(str); i++) 
    {
        if (str[i] != symbol) 
        {
            str[j++] = str[i];
        }
        else
        {
            isFind = 1;
        }
    }
    if (!isFind)
    {
        wcout << "Symbol not found";
    }
    else
    {
        str[j] = L'\0';
        wcout << "After deleting: " << str << endl;
    }
}

//task 8
void InsertSymbolByPosition()
{
    _TCHAR str[] = _TEXT("Borrasca");
    wcout << "Before deleting: " << str << endl;

    _TCHAR symbol;
    wcout << "Enter symbol: ";
    wcin >> symbol;

    int pos;
    wcout << "Enter position: ";
    wcin >> pos;

    int len = wcslen(str);

    if (pos >= 0 && pos <= len)
    {
        _TCHAR newStr[36];
        int j = 0;

        for (int i = 0; i < pos; i++) 
        {
            newStr[j++] = str[i];
        }
        newStr[j++] = symbol;

        for (int i = pos; i < len; i++) 
        {
            newStr[j++] = str[i];
        }

        newStr[j] = L'\0';
        wcout << "After inserting: " << newStr << endl;
    }
    else
    {
        wcout << "Invalid position." << endl;
    }
}

void main()
{
	//SpaceToTab();
	//CountSymbols();
	//CountWords();
    CountVowelsRU();
    //IsPalindrome();
    //DeleteSymbolByPosition();
    //DeleteSymbols();
    //InsertSymbolByPosition();
}
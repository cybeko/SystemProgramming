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
    _TCHAR str[] = _TEXT("Hello123[&]");
    wcout << str << endl;

    int letterCount = 0;
    int numberCount = 0;
    int symbolCount = 0;

    for (int i = 0; i < _tcslen(str); ++i) 
    {
        if (iswalpha(str[i])) 
        {
            ++letterCount;
        }
        else if (iswdigit(str[i])) 
        {
            ++numberCount;
        }
        else if (iswpunct(str[i])) 
        {
            ++symbolCount;
        }
    }

    wcout << "Letters: " << letterCount << endl;
    wcout << "Numbers: " << numberCount << endl;
    wcout << "Symbols: " << symbolCount << endl;
}
//task 3 
void CountWords()
{
    const int SIZE = 64;
    _TCHAR str[SIZE];

    wcout << L"Enter sentence: ";
    wcin.getline(str, SIZE);

    bool inSpace = false;
    int words = 0;

    for (size_t i = 0; i < _tcslen(str); i++)
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
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    const int SIZE = 64;
    _TCHAR str[SIZE];
    _TCHAR vowels[SIZE] = L"ауоиэы€юеЄ";

    wcout << L"¬ведите предложение: ";
    wcin.getline(str, SIZE);

    int vowelCount = 0;

    for (int i = 0; i < wcslen(str); ++i)
    {
        _TCHAR ch = towlower(str[i]);
        for (int j = 0; j < wcslen(vowels); j++)
        {
            if (ch == vowels[j])
            {
                vowelCount++;
                break;
            }
        }
    }
    wcout << L" оличество гласных букв: " << vowelCount << endl;
}

//task 5
void IsPalindrome()
{
    _TCHAR str[] = _TEXT("Racecar");

    int size = _tcslen(str);

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
    wcout << L"Before deleting: " << str << endl;

    int pos;
    wcout << L"Enter position: ";
    cin >> pos;

    int size = _tcslen(str);

    if (pos >= 0 && pos < size)
    {
        for (int i = pos; i < size - 1; i++)
        {
            str[i] = str[i + 1];
        }

        str[size - 1] = _TEXT('\0');
        wcout << L"After deleting: " << str << endl;
    }
    else
    {
        wcout << L"Invalid position." << endl;
    }
}

//task 7
void DeleteSymbols()
{
    _TCHAR str[] = _TEXT("Borrasca");
    wcout << L"Before deleting: " << str << endl;

    _TCHAR symbol;
    wcout << L"Enter symbol: ";
    wcin >> symbol;

    bool isFind = 0;
    int j = 0;
    for (int i = 0; i < _tcslen(str); i++)
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
        wcout << L"Symbol not found";
    }
    else
    {
        str[j] = L'\0';
        wcout << L"After deleting: " << str << endl;
    }
}

//task 8
void InsertSymbolByPosition()
{
    _TCHAR str[] = _TEXT("Borrasca");
    wcout << L"Before deleting: " << str << endl;

    _TCHAR symbol;
    wcout << L"Enter symbol: ";
    wcin >> symbol;

    int pos;
    wcout << L"Enter position: ";
    wcin >> pos;

    int len = _tcslen(str);

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
        wcout << L"After inserting: " << newStr << endl;
    }
    else
    {
        wcout << L"Invalid position." << endl;
    }
}

void main()
{
	SpaceToTab();
	//CountSymbols();
	//CountWords();
    //CountVowelsRU();
    //IsPalindrome();
    //DeleteSymbols();
    //InsertSymbolByPosition();
}
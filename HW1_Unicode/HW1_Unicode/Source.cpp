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

	for (size_t i = 0; i < wcslen(str); i++)
	{
		if (str[i] == ' ')
		{
			str[i] = '\t';
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
void CountVowelsRU()
{
    const int SIZE = 64;
    _TCHAR str[SIZE];

    wcout << L"Enter sentence: ";
    wcin.getline(str, SIZE);
    wcout << str << endl;


}


void main()
{
	//SpaceToTab();
	//CountSymbols();
	//CountWords();
    CountVowelsRU();
}
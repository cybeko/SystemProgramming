#include "CriticalSectionDlg.h"

CriticalSectionDlg* CriticalSectionDlg::ptr = NULL;

CRITICAL_SECTION cs;
vector<string> fileNames;
CriticalSectionDlg dlg;

CriticalSectionDlg::CriticalSectionDlg(void)
{
	ptr = this;
	InitializeCriticalSection(&cs);
}

CriticalSectionDlg::~CriticalSectionDlg(void)
{
	DeleteCriticalSection(&cs);
}

void CriticalSectionDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL CriticalSectionDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
	return TRUE;
}
void OpenConsole()
{
	AllocConsole();
	(void)freopen("conin$", "r", stdin);
	(void)freopen("conout$", "w", stdout);
	(void)freopen("conout$", "w", stderr);
}
void MessageAboutError(DWORD dwError)
{
	LPVOID lpMsgBuf = NULL;
	TCHAR szBuf[300]; 

	BOOL fOK = FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
	NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

	if(lpMsgBuf != NULL)
	{
		wsprintf(szBuf, TEXT("Ошибка %d: %s"), dwError, lpMsgBuf); 
		MessageBox(0 ,szBuf, TEXT("Сообщение об ошибке"), MB_OK | MB_ICONSTOP); 
		LocalFree(lpMsgBuf); 
	}
}

DWORD WINAPI CreateFiles(LPVOID lp)
{
	EnterCriticalSection(&cs);
	cout << "[Writing files]\n" << endl;

	ifstream readFile(dlg.initFileName, ios::in);
	char cBuf[100];

	for(size_t i = 0; i <= dlg.fileAmt-1; i++)
	{
		cout << "SIZE:" << dlg.fileAmt - 1 << endl;
		string copyFileName = "copy" + to_string(i)+".txt";
		wofstream createFile(copyFileName);
		fileNames.push_back(copyFileName);

		cout << "Writing file:"<< fileNames[i] << endl;
		readFile.clear();
		readFile.seekg(0, ios::beg);
		while (!readFile.eof())
		{
			readFile.getline(cBuf, sizeof(cBuf));
			cout << "Contents of cBuf: " << cBuf << endl;
			createFile << cBuf;
		}
		createFile << i;
		createFile.close();
	}
	readFile.close();

	LeaveCriticalSection(&cs);
	cout << "[File created]" << endl;
	MessageBox(NULL, L"Copies created", L"File", MB_OK);
	return 0;
}

DWORD WINAPI ReadAndWrite(LPVOID lp)
{
	EnterCriticalSection(&cs);
	cout << "\n[Reading files]" << endl;
	char cBuf[100];
	string strBuf;

	wofstream writeRes(dlg.resFileName);
	cout <<"[SIZE: " << fileNames.size()<<"]" << endl;

	for (size_t i = 0; i < fileNames.size(); i++)
	{
		ifstream readCopy(fileNames[i], ios::in);
		cout <<"Read files:"<< fileNames[i] << endl;

		while (!readCopy.eof())
		{
			readCopy.getline(cBuf, sizeof(cBuf));
			writeRes << cBuf;
		}
		readCopy.close();
	}
	writeRes.close();
	LeaveCriticalSection(&cs);
	cout << "[Written to result.txt]" << endl;
	MessageBox(NULL, L"Written to result.txt", L"File", MB_OK);
	return 0;
}

void CriticalSectionDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if(id == IDC_BUTTON1)
	{
		HANDLE hThread = CreateThread(NULL, 0, CreateFiles, 0, 0, NULL); 
		CloseHandle(hThread);
		hThread = CreateThread(NULL, 0, ReadAndWrite, 0, 0, NULL);  
		CloseHandle(hThread);
	}
}

BOOL CALLBACK CriticalSectionDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}
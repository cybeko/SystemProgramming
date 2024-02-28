#include "CriticalSectionDlg.h"

CriticalSectionDlg* CriticalSectionDlg::ptr = NULL;

CRITICAL_SECTION cs;

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
vector<string> fileNames;
CriticalSectionDlg dlg;

DWORD WINAPI CreateFiles(LPVOID lp)
{
	cout << "Write files\n" << endl;

	ifstream readFile(dlg.initFileName, ios::in);
	EnterCriticalSection(&cs);

	char cBuf[100];
	if (!readFile.is_open()) 
	{
		MessageBox(0, TEXT("Failed to open the file"), TEXT("Critical Section"), MB_OK);
		LeaveCriticalSection(&cs);
		return 1;
	}
	for(size_t i = 0; i <= dlg.fileAmt-1; i++)
	{
		cout << "SIZE:" << dlg.fileAmt - 1<<endl;
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
	return 0;
}

DWORD WINAPI ReadAndWrite(LPVOID lp)
{
	cout << "\nRead files" << endl;

	char cBuf[100];
	string strBuf;
	EnterCriticalSection(&cs);

	wofstream writeRes(dlg.resFileName);
	if (!writeRes.is_open())
	{
		MessageBox(0, TEXT("Failed to open writeRes"), TEXT("Critical Section"), MB_OK);
		LeaveCriticalSection(&cs);
		return 1;
	}
	cout <<"[SIZE: " << fileNames.size()<<"]" << endl;

	for (size_t i = 0; i < fileNames.size(); i++)
	{
		ifstream readCopy(fileNames[i], ios::in);
		cout <<"Read files:"<< fileNames[i] << endl;

		if (!readCopy.is_open())
		{
			MessageBox(0, TEXT("Failed to open writeRes"), TEXT("Critical Section"), MB_OK);
			LeaveCriticalSection(&cs);
			return 1;
		}
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
	//MessageBox(0, TEXT("Поток записал текст в result.txt"), TEXT("Критическая секция"), MB_OK);
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
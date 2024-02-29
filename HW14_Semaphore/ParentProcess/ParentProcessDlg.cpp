#include "ParentProcessDlg.h"

ParentProcessDlg* ParentProcessDlg::ptr = NULL;
ParentProcessDlg::ParentProcessDlg(void)
{
	ptr = this;
}
ParentProcessDlg::~ParentProcessDlg(void)
{
	
}
void ParentProcessDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

void MessageAboutError(DWORD dwError)
{
	LPVOID lpMsgBuf = NULL;
	TCHAR szBuf[300]; 
	BOOL fOK = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	if(lpMsgBuf != NULL)
	{
		wsprintf(szBuf, TEXT("Ошибка %d: %s"), dwError, lpMsgBuf); 
		MessageBox(NULL, szBuf, TEXT("Сообщение об ошибке"), MB_OK | MB_ICONSTOP); 
		LocalFree(lpMsgBuf);
	}
}
void OpenConsole()
{
	AllocConsole();
	(void)freopen("conin$", "r", stdin);
	(void)freopen("conout$", "w", stdout);
	(void)freopen("conout$", "w", stderr);
}
void ParentProcessDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (IDC_BUTTON1 == id)
	{
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT1);
		TCHAR buf[128];
		GetWindowText(hEdit, buf, 32);
		int amtCopies = _wtoi(buf);
		if (amtCopies <= 0 || amtCopies > 10)
		{
			MessageBox(hwnd, TEXT("Copies amount can't be greater than 10 or lesser than 0"), TEXT("Error"), MB_OK);
			return;
		}

		vector <wstring> fileNames;
		CreateSemaphore(NULL, 1, amtCopies, TEXT("{2525FD5F-12E6-47c0-838A-7C5CA1EBD169}"));

		STARTUPINFO st = {sizeof(st)};
		PROCESS_INFORMATION pr;
		TCHAR copyFileName[20];
		wsprintf(copyFileName, TEXT("%s"), TEXT("CodingThread.exe"));
		if (!CreateProcess(NULL, copyFileName, NULL, NULL, 0, 0, NULL, NULL, &st, &pr))
		{
			MessageAboutError(GetLastError());
			return;
		}
		CloseHandle(pr.hThread);
		CloseHandle(pr.hProcess);

		for(int i = 1; i <= amtCopies; i++)
		{
			TCHAR buf[30];

			ZeroMemory(&st, sizeof(st));
			st.cb = sizeof(st);

			wsprintf(buf, TEXT("copymusic_%d.txt"), i);
			fileNames.push_back(buf);
			cout << "push filename"<<endl;

			wsprintf(buf, TEXT("DecodingThread.exe %d"), i);
			if (!CreateProcess(NULL, buf, NULL, NULL, 0, 0, NULL, NULL, &st, &pr))
			{
				MessageAboutError(GetLastError());
			}
			CloseHandle(pr.hThread);
			CloseHandle(pr.hProcess);	
		}

		Sleep(1000);

		int totalChars = 0;
		int totalSize = 0;
		for (size_t i = 0; i < fileNames.size(); i++)
		{
			string text;
			ifstream inSize(fileNames[i], ifstream::ate | ifstream::binary);
			//if (!inSize.is_open())
			//{
			//	MessageBox(NULL, L"Error opening file", L"Error", MB_OK);
			//}
			totalSize += inSize.tellg();

			ifstream inRead(fileNames[i]);

			while (getline(inRead, text))
			{
				totalChars += text.size();
			}
			inSize.close();
			inRead.close();
		}

		swprintf(buf, TEXT("Copies created: %d\nSymbols in total - %d \nTotal size - %.2f mb\n"),
			amtCopies, totalChars, (float)(totalSize / 1024.f)/1024.f);

		MessageBox(hwnd, buf, TEXT("Statistics"), MB_OK | MB_ICONINFORMATION);
	}
}

BOOL CALLBACK ParentProcessDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}
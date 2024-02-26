#include "CodingThreadDlg.h"

HANDLE hMutex;

CodingThreadDlg* CodingThreadDlg::ptr = NULL;
CodingThreadDlg::CodingThreadDlg(void)
{
	ptr = this;
}
CodingThreadDlg::~CodingThreadDlg(void)
{
	delete ptr;
}
void CodingThreadDlg::Cls_OnClose(HWND hwnd)
{
	ReleaseMutex(hMutex);
	EndDialog(hwnd, 0);
}

DWORD WINAPI ReadFile(LPVOID lp)
{
	CodingThreadDlg* ptr = (CodingThreadDlg*)lp;

	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, TEXT("{B8A2C367-10FE-494d-A869-841B2AF972E0}"));

	ifstream in(TEXT("Text.txt"), ios::binary | ios::in);
	if (!in)
	{
		MessageBox(ptr->hDialog, TEXT("Ошибка открытия файла!"), TEXT("Мьютекс"), MB_OK | MB_ICONINFORMATION);
		return 1;
	}

	char buf[64];
	while (!in.eof())
	{
		in.getline(buf, 64);
		ptr->Text.append(buf);
		ptr->Text.append("\n");
	}
	in.close();
	ReleaseMutex(hMutex);

	mbstowcs(ptr->wBuf, ptr->Text.c_str(), ptr->Text.size() + 1);
	SetWindowText(ptr->hEdit, ptr->wBuf);

	MessageBox(ptr->hDialog, TEXT("Чтение файла завершено"), TEXT("Мьютекс"), MB_OK | MB_ICONINFORMATION);
	return 0;
}

BOOL CodingThreadDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hDialog = hwnd;
	hButton = GetDlgItem(hwnd, IDC_BUTTON1);
	hEdit = GetDlgItem(hwnd, IDC_EDIT1);
	CreateThread(NULL, 0, ReadFile, this, 0, NULL);
	return TRUE;
}

void CodingThreadDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_BUTTON1)
	{
		Cls_OnClose(hwnd);
	}
}

INT_PTR CALLBACK CodingThreadDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}
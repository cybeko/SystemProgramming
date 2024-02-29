#include "ParentProcessDlg.h"

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
	ParentProcessDlg dlg;
	//OpenConsole();
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, ParentProcessDlg::DlgProc);
}
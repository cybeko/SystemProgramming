#include "SourceDlg.h"

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
	cSourceDlg dlg;
	//OpenConsole();
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, cSourceDlg::DlgProc);
}
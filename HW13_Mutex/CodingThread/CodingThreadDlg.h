#pragma once
#include "header.h"

class CodingThreadDlg
{
public:
	static CodingThreadDlg* ptr;

	CodingThreadDlg(void);
	~CodingThreadDlg(void);

	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	void Cls_OnClose(HWND hwnd);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);

	HWND hDialog, hButton, hEdit;

	string Text;
	WCHAR wBuf[1024];
};

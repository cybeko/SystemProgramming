#pragma once
#include <windows.h>
#include <windowsX.h>
#include <tchar.h>
#include <commctrl.h>
#include "resource.h"
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

struct Button
{
	HWND hwnd;
	int BtnId;
};

class CSourceDlg
{
public:
	static CSourceDlg* ptr;
	static const int BUFSIZE = 64;
	bool bGameStarted = FALSE;
	int nCount, nTimeMsec;

	HWND hDialog, hSpin, hEdit, hStartGame, hList, hProgBar;
	TCHAR buf[BUFSIZE];

	UINT_PTR pTimer;

	vector<Button> hButtons;
	vector<int> aGameNumbers;
	vector<int> aUserNumbers;


	CSourceDlg(void);
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	void GenerateNumbers();
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnTimer(HWND hwnd, UINT id);
	void Cls_OnClose(HWND hwnd);
	void IsWin(HWND hwnd);
};

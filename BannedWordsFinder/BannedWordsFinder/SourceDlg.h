#pragma once
#include "header.h"

void OpenConsole();

class cSourceDlg
{
public:
	static cSourceDlg* ptr;

	cSourceDlg(void);
	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);

	void CheckRunning();
	void CopyWordsFromFile();


	void AddFileToList();
	void DeleteFileFromList();

	void AddBannedWord();
	void DeleteBannedWord();
	void UpdateBannedWordList();

	void ReplaceBannedWords();

	void ResetProgressBar();
	void UpdateBannedWordsFile();


	HWND hDialog;
	HANDLE hThread;
	HWND hListFiles, hListBanWords;
	HWND hBtnLoadFile, hBtnDelFile, hBtnAddWord, hBtnDelWord;
	HWND hEditUserBanWord;
	HWND hProgressBar, hBtnStart, hBtnPause, hBtnStop;

	vector <wstring> bannedWords;
	vector <wstring> fileNames;
};

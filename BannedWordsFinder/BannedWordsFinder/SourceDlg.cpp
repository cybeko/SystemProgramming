#include "SourceDlg.h"
#pragma warning(disable: 4996)

CRITICAL_SECTION cs;
const WCHAR pathBannedWords[] = TEXT("BannedWords.txt");

void OpenConsole()
{
	AllocConsole();
	(void)freopen("conin$", "r", stdin);
	(void)freopen("conout$", "w", stdout);
	(void)freopen("conout$", "w", stderr);
}
cSourceDlg* cSourceDlg::ptr = NULL;
cSourceDlg::cSourceDlg(void)
{
	InitializeCriticalSection(&cs);
	ptr = this;
}
void cSourceDlg::Cls_OnClose(HWND hwnd)
{
	EnterCriticalSection(&cs);
	LeaveCriticalSection(&cs);
	DeleteCriticalSection(&cs);
	CloseHandle(hThread);
	EndDialog(hwnd, 0);
}
BOOL cSourceDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hDialog = hwnd;
	CheckRunning();

	hListFiles = GetDlgItem(hDialog, IDC_LIST_FILES);
	hListBanWords = GetDlgItem(hDialog, IDC_LIST_WORDS);

	hEditUserBanWord = GetDlgItem(hDialog, IDC_EDIT_WORD);

	hBtnLoadFile = GetDlgItem(hDialog, IDC_BUTTON_ADDFILE);
	hBtnDelFile = GetDlgItem(hDialog, IDC_BUTTON_DELFILE);
	hBtnDelWord = GetDlgItem(hDialog, IDC_BUTTON_DELWORD);
	hBtnAddWord = GetDlgItem(hDialog, IDC_BUTTON_ADDWORD);

	hProgressBar = GetDlgItem(hDialog, IDC_PROGRESS1);
	hBtnStart = GetDlgItem(hDialog, IDC_BUTTON_START);
	hBtnPause = GetDlgItem(hDialog, IDC_BUTTON_PAUSE);
	hBtnStop = GetDlgItem(hDialog, IDC_BUTTON_STOP);

	EnableWindow(hBtnPause, FALSE);
	EnableWindow(hBtnStop, FALSE);

	ResetProgressBar();
	CopyWordsFromFile();

	return TRUE;
}

DWORD WINAPI Thread(LPVOID lp)
{
	cSourceDlg* dlg = (cSourceDlg*)lp;

	if (dlg->fileNames.size() != 0)
	{
		map<wstring, int> bannedWordsCount;
		for (const auto& bannedWord : dlg->bannedWords)
		{
			bannedWordsCount[bannedWord] = 0;
		}
		for (size_t i = 0; i < dlg->fileNames.size(); ++i)
		{
			SendMessage(dlg->hProgressBar, PBM_STEPIT, 0, 0);
			wifstream read(dlg->fileNames[i], ios::in);
			if (!read.is_open())
			{
				MessageBox(dlg->hDialog, L"Failed to open file", TEXT("Error"), MB_OK | MB_ICONERROR);
				continue;
			}
			wstring line;
			bool bannedWordFound = false;

			int lastSlashPos = dlg->fileNames[i].find_last_of(TEXT("\\"));
			int dotPos = dlg->fileNames[i].find(TEXT("."));
			wstring szNameFile = dlg->fileNames[i].substr(lastSlashPos + 1, dotPos - lastSlashPos - 1);
			wcout << szNameFile << endl;
			szNameFile.append(TEXT("_Copy.txt"));
			Sleep(1000);
			wcout << szNameFile << endl;

			while (getline(read, line))
			{
				for (size_t j = 0; j < dlg->bannedWords.size(); ++j)
				{
					if (line.find(dlg->bannedWords[j]) != wstring::npos) {
						bannedWordFound = true;
						break;
					}
				}
				if (bannedWordFound) {
					break;
				}
			}
			read.close();
			if (bannedWordFound) {
				wofstream write(szNameFile, ios::out);
				wifstream readAgain(dlg->fileNames[i], ios::in);

				while (getline(readAgain, line)) {
					for (size_t j = 0; j < dlg->bannedWords.size(); ++j) {
						size_t pos = 0;
						while ((pos = line.find(dlg->bannedWords[j], pos)) != wstring::npos)
						{
							bannedWordsCount[dlg->bannedWords[j]]++;
							line.replace(pos, dlg->bannedWords[j].length(), L"*******");
							pos += 7;
						}
					}
					write << line << endl;
				}
				write.close();
				readAgain.close();
			}

		}
		vector<pair<wstring, int>> sortedBannedWords(bannedWordsCount.begin(), bannedWordsCount.end());
		sort(sortedBannedWords.begin(), sortedBannedWords.end(), [](const auto& lhs, const auto& rhs) {
			return lhs.second > rhs.second;
			});

		wofstream reportFile(L"Report.txt", ios::out);
		reportFile << "Top 10 banned words\n" << endl;
		for (int i = 0; i <= 10; ++i) {
			reportFile << i << L": " << sortedBannedWords[i].first << L": " << sortedBannedWords[i].second << std::endl;
		}
		reportFile.close();
		const char* filename = "start Report.txt";
		std::system(filename);
	}
	else {
		MessageBox(NULL, L"File not selected", TEXT("Error"), MB_OK | MB_ICONERROR);

	}
	dlg->ResetProgressBar();
	EnableWindow(dlg->hBtnStart, TRUE);
	EnableWindow(dlg->hBtnPause, FALSE);
	EnableWindow(dlg->hBtnStop, FALSE);

	return 0;
}

void cSourceDlg::CopyWordsFromFile()
{
	ifstream in(pathBannedWords);
	if (!in)
	{
		MessageBox(hDialog, L"Can't open file1", TEXT("Error"), MB_OK | MB_ICONERROR);
		return;
	}
	char buf[STR_SIZE];
	WCHAR wbuf[STR_SIZE];
	while (!in.eof())
	{
		in.getline(buf, STR_SIZE);
		if (strlen(buf) != 0)
		{
			MultiByteToWideChar(CP_UTF8, 0, buf, -1, wbuf, STR_SIZE);
			bannedWords.push_back(wbuf);
		}
	}
	in.close();
	UpdateBannedWordList();
}

void cSourceDlg::CheckRunning()
{
	TCHAR GUID[] = TEXT("{D2F0EA13-60F7-4B62-97A5-29C8047F82E8}");
	HANDLE hMutex = CreateMutex(NULL, FALSE, GUID);
	DWORD dwAnswer = WaitForSingleObject(hMutex, 0);
	if (dwAnswer == WAIT_TIMEOUT)
	{
		MessageBox(hDialog, L"Application is running", TEXT("Error"), MB_OK | MB_ICONERROR);
		EndDialog(hDialog, 0);
	}
}
void cSourceDlg::ResetProgressBar()
{
	SendMessage(hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, 200));
	SendMessage(hProgressBar, PBM_SETSTEP, 1, 0);
	SendMessage(hProgressBar, PBM_SETPOS, 0, 0);
	SendMessage(hProgressBar, PBM_SETBKCOLOR, 0, LPARAM(RGB(0, 0, 255)));
	SendMessage(hProgressBar, PBM_SETBARCOLOR, 0, LPARAM(RGB(255, 255, 0)));
}

void cSourceDlg::AddFileToList()
{
	WCHAR szPathFile[MAX_PATH] = { 0 };
	OPENFILENAME open = { sizeof(OPENFILENAME) };
	open.hwndOwner = hDialog;
	open.lpstrFilter = TEXT("Text Files(*.txt)\0*.txt");
	open.lpstrFile = szPathFile;
	open.nMaxFile = MAX_PATH;
	wstring initialDir = L"D:";
	open.lpstrInitialDir = const_cast<LPWSTR>(initialDir.c_str());
	open.Flags = OFN_CREATEPROMPT | OFN_PATHMUSTEXIST;

	if (GetOpenFileName(&open))
	{
		ifstream in(szPathFile, ios::in);
		if (!in)
		{
			MessageBox(hDialog, L"Can't open file", TEXT("Error"), MB_OK | MB_ICONERROR);
			return;
		}
		in.close();
		int dIndex = SendMessage(hListFiles, LB_FINDSTRINGEXACT, -1, (LPARAM)szPathFile);
		if (dIndex == LB_ERR)
		{
			SendMessage(hListFiles, LB_ADDSTRING, 0, (LPARAM)szPathFile);
			fileNames.push_back(szPathFile);
		}
	}
}
void cSourceDlg::DeleteFileFromList()
{
	int id = (int)SendMessage(hListFiles, LB_GETCURSEL, 0, 0);
	if (id != LB_ERR)
	{
		SendMessage(hListFiles, LB_DELETESTRING, id, 0);
		fileNames.erase(fileNames.begin() + id);
	}
}

void cSourceDlg::AddBannedWord()
{
	WCHAR wBuf[STR_SIZE];
	GetWindowText(hEditUserBanWord, wBuf, sizeof(wBuf));
	if (lstrlen(wBuf))
	{
		int index = SendMessage(hListBanWords, LB_FINDSTRINGEXACT, -1, (LPARAM)wBuf);
		if (index == LB_ERR)
		{
			SendMessage(hListBanWords, LB_ADDSTRING, 0, LPARAM(wBuf));
			SetWindowText(hEditUserBanWord, NULL);
			bannedWords.push_back(wBuf);
			UpdateBannedWordsFile();
		}
		else
		{
			MessageBox(hDialog, L"This banned word already exists", TEXT("Error"), MB_OK | MB_ICONERROR);
		}
	}
	else {
		MessageBox(hDialog, L"Empty banned word field", TEXT("Error"), MB_OK | MB_ICONERROR);
	}
}

void cSourceDlg::UpdateBannedWordsFile()
{
	//wstring filePath = ReturnDirPath();
	//filePath.append(L"\\");
	//filePath.append(pathBannedWords);
	//wcout << filePath;
	wofstream out(pathBannedWords, ios::trunc);
	if (!out)
	{
		MessageBox(hDialog, L"Failed to open BannedWords.txt for writing", TEXT("Error"), MB_OK | MB_ICONERROR);

		return;
	}

	for (const auto& word : bannedWords)
	{
		out << word << endl;
	}

	out.close();
}
void cSourceDlg::DeleteBannedWord()
{
	int selectedItem = (int)SendMessage(hListBanWords, LB_GETCURSEL, 0, 0);
	if (selectedItem != LB_ERR)
	{
		SendMessage(hListBanWords, LB_DELETESTRING, selectedItem, 0);
		bannedWords.erase(bannedWords.begin() + selectedItem);
		UpdateBannedWordsFile();
	}
}
void cSourceDlg::UpdateBannedWordList()
{
	SendMessage(hListBanWords, LB_RESETCONTENT, 0, 0);
	for (int i = 0; i < bannedWords.size(); i++) {
		SendMessage(hListBanWords, LB_ADDSTRING, 0, (LPARAM)bannedWords[i].c_str());
	}
}
void cSourceDlg::ReplaceBannedWords()
{
	ResetProgressBar();
	if (fileNames.size() == 0)
	{
		return;
	}
	EnableWindow(hBtnStart, FALSE);
	EnableWindow(hBtnPause, TRUE);
	EnableWindow(hBtnStop, TRUE);

	cSourceDlg dlg(*this);
	SendMessage(hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, fileNames.size()));
	hThread = CreateThread(NULL, 0, Thread, this, 0, NULL);
}
void cSourceDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDC_BUTTON_ADDFILE:
	{
		AddFileToList();
		break;
	}
	case IDC_BUTTON_DELFILE:
	{
		DeleteFileFromList();
		break;
	}
	case IDC_BUTTON_ADDWORD:
	{
		AddBannedWord();
		break;
	}
	case IDC_BUTTON_DELWORD:
	{
		DeleteBannedWord();
		break;
	}
	case IDC_BUTTON_START:
	{
		ReplaceBannedWords();
		break;
	}
	case IDC_BUTTON_PAUSE:
	{
		static BOOL flag = TRUE;
		if (flag) {
			SuspendThread(hThread);
			SetWindowText(hBtnPause, TEXT("Resume"));
		}
		else
		{
			ResumeThread(hThread);
			SetWindowText(hBtnPause, TEXT("Pause"));
		}
		flag = !flag;
		break;
	}
	case IDC_BUTTON_STOP:
	{
		TerminateThread(hThread, 0);
		EnableWindow(hBtnStart, TRUE);
		EnableWindow(hBtnPause, FALSE);
		EnableWindow(hBtnStop, FALSE);
		MessageBox(hDialog, L"Progress is reset", TEXT("Error"), MB_OK);
		ResetProgressBar();
	}
	default:
		break;
	}
}

INT_PTR CALLBACK cSourceDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}

#include "RegistrationModalDlg.h"
#include "User.h"
#include <cctype>
#define _CRT_SECURE_NO_WARNINGS

CRegistrationModalDlg dlg;
HMODULE hMod;
HHOOK hHook;
User n;

void OpenConsole()
{
	AllocConsole();
	(void)freopen("conin$", "r", stdin);
	(void)freopen("conout$", "w", stdout);
	(void)freopen("conout$", "w", stderr);
}

LRESULT CALLBACK KeyboardProc(int idCode, WPARAM wParam, LPARAM lParam)
{
	if (HC_ACTION == idCode)
	{
		HWND hWnd = GetFocus();
		WCHAR buf[STR_SIZE];
		buf[0] = L'\0';

		GetClassName(hWnd, buf, sizeof(buf));

		if (lstrcmpi(buf, TEXT("EDIT")) == 0 && (lParam & 0x80000000))
		{
			buf[0] = L'\0';
			if (!(wParam >= 'A' && wParam <= 'Z'))
			{
				wsprintf(buf, TEXT("%c"), wParam);
			}
			else
			{
				short shift = GetKeyState(VK_SHIFT); // если старший бит равен 1, то клавиша нажата
				short caps = GetKeyState(VK_CAPITAL); // если младший бит равен 1, то клавиша переключена
				if ((shift & 0x8000) && !(caps & 1) || !(shift & 0x8000) && (caps & 1))
				{
					wsprintf(buf, TEXT("%c"), wParam);
				}
				else
				{
					wsprintf(buf, TEXT("%c"), tolower(wParam));
				}
			}
			if (hWnd == dlg.hEditLogin)
			{
				wcscat_s(n.Login, buf);
			}
			else if (hWnd == dlg.hEditPassword)
			{
				wcscat_s(n.Password, buf);
			}
			else if (hWnd == dlg.hEditFullName)
			{
				wcscat_s(n.FullName, buf);
			}
			else if (hWnd == dlg.hEditAge)
			{
				wcscat_s(n.Age, buf);
			}
			wcout << L"Buffer content: " << buf << L" Length: " << wcslen(buf) << endl;
		}
	}
	return CallNextHookEx(hHook, idCode, wParam, lParam);
}

CRegistrationModalDlg* CRegistrationModalDlg::ptr = NULL;
CRegistrationModalDlg::CRegistrationModalDlg(void)
{
	ptr = this;
}
CRegistrationModalDlg::~CRegistrationModalDlg(void)
{
}

void CRegistrationModalDlg::Cls_OnClose(HWND hwnd)
{
	UnhookWindowsHookEx(hHook);
	EndDialog(hwnd, IDCANCEL);
}

BOOL CRegistrationModalDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	dlg.hEditLogin = GetDlgItem(hwnd, IDC_EDIT_REG_LOGIN);
	dlg.hEditPassword = GetDlgItem(hwnd, IDC_EDIT_REG_PASSWORD);
	dlg.hEditFullName = GetDlgItem(hwnd, IDC_EDIT_REG_FULL_NAME);
	dlg.hEditAge = GetDlgItem(hwnd, IDC_EDIT_REG_AGE);
	hHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, NULL, GetCurrentThreadId());
	return TRUE;
}
void ClearUserInfo(User& n)
{
	n.Login[0] = L'\0';
	n.Password[0] = L'\0';
	n.FullName[0] = L'\0';
	n.Age[0] = L'\0';
}

void CRegistrationModalDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDC_BUT_REG_ENTER)
	{
		std::wstring userInfo;

		userInfo = L"Login: ";
		userInfo += n.Login;
		userInfo += L"\nPassword: ";
		userInfo += n.Password;
		userInfo += L"\nFull Name: ";
		userInfo += n.FullName;
		userInfo += L"\nAge: ";
		userInfo += n.Age;

		MessageBox(NULL, userInfo.c_str(), L"Here", MB_OK);
		userInfo = L" ";

		if (n.SpaceCheck(n.Login) || n.SpaceCheck(n.Password))
		{
			MessageBox(hwnd, TEXT("Логин и пароль не должны иметь пробелы"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		}
		else if (n.EmptyCheckReg())
		{
			MessageBox(hwnd, TEXT("Все строки должны быть заполнены"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		}
		else if (n.AgeCheck(n.Age))
		{
			MessageBox(hwnd, TEXT("Строка \"возраст\" должна быть числом"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		}
		else
		{
			bool isLoginFound = false;

			wifstream file("data.txt");
			if (!file.is_open())
			{
				wofstream createDataFile("data.txt");
			}
			wstring line;
			while (getline(file, line))
			{
				size_t pos = line.find(L' ');
				if (pos != wstring::npos)
				{
					wstring dataLogin = line.substr(0, pos);

					if (dataLogin == n.Login)
					{
						MessageBox(hwnd, TEXT("Аккаунт с таким логином уже существует"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
						isLoginFound = true;
						file.close();
						return;
					}
				}
			}
			file.close();

			if (!isLoginFound)
			{
				wofstream out("data.txt", fstream::app);
				if (out.is_open())
				{
					out << n.Login << " " << n.Password << " " << n.FullName << " " << n.Age << endl;
					out.close();
					MessageBox(hwnd, TEXT("Аккаунт успешно создан"), TEXT("Регистрация"), MB_OK | MB_ICONINFORMATION);
					EndDialog(hwnd, IDCANCEL);
					wcout << n.Login << " " << n.Password << " " << n.FullName << " " << n.Age << endl;
					ClearUserInfo(n);
				}
				else
				{
					MessageBox(hwnd, TEXT("Файл с данными аккунтов невозможно открыть"), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
				}
			}
		}
	}
	else if (id == IDC_BUT_REG_CANCEL)
	{
		EndDialog(hwnd, IDCANCEL);
	}
}

INT_PTR CALLBACK CRegistrationModalDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}

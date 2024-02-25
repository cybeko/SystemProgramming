#include "LoginModalDlg.h"
#include "LoginModalDlg.h"
#include "RegistrationModalDlg.h"
#include "User.h"

CLoginModalDlg dlg;
CLoginModalDlg* CLoginModalDlg::ptr = NULL;
CLoginModalDlg::CLoginModalDlg(void)
{
	ptr = this;
}
CLoginModalDlg::~CLoginModalDlg(void)
{
}
void CLoginModalDlg::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, IDCANCEL);
}

BOOL CLoginModalDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	return TRUE;
}

void CLoginModalDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDOK)
	{
		HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
		HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);

		User n;
		GetWindowText(hEditLogin, n.Login, STR_SIZE);
		GetWindowText(hEditPassword, n.Password, STR_SIZE);

		if (n.EmptyCheckLog())
		{
			MessageBox(hwnd, L"Fields must not be empty", L"error", MB_OK);
		}
		else if (n.SpaceCheck(n.Login) || n.SpaceCheck(n.Login))
		{
			MessageBox(hwnd, L"Password and login must not contain space", L"error", MB_OK);
		}
		else
		{
			bool isLoginFound = false;
			wifstream file("data.txt");
			if (file.is_open())
			{
				wstring line;
				while (getline(file, line))
				{
					size_t pos1 = line.find(L' ');
					if (pos1 != wstring::npos)
					{
						wstring dataLogin = line.substr(0, pos1);
						size_t pos2 = line.find(L' ', pos1 + 1);
						if (pos1 != wstring::npos)
						{
							wstring dataPassword = line.substr(pos1 + 1, pos2 - pos1 - 1);

							if (dataLogin == n.Login)
							{
								isLoginFound = true;
								if (dataPassword == n.Password)
								{
									MessageBox(hwnd, L"Logging in...", L"Correct password", MB_OK);
									EndDialog(hwnd, 0);
								}
								else
								{
									MessageBox(hwnd, L"Incorrect password. Try again.", L"Incorrect password", MB_OK);
								}
								file.close();
								return;
							}
						}
					}
				}
				file.close();
			}
			if (!file.is_open() || !isLoginFound)
			{

				int result = MessageBox(hwnd, L"User not found!\nCreate a new account?", L"Wrong login", MB_OKCANCEL);
				if (result == IDOK)
				{
					EndDialog(hwnd, IDCANCEL);

					const INT_PTR result = DialogBox(GetModuleHandle(NULL),
						MAKEINTRESOURCE(IDD_DIALOG_REGISTRATION),
						hwnd,
						CRegistrationModalDlg::DlgProc
					);
				}
			}
		}
	}
	else if (id == IDNO)
	{
		EndDialog(hwnd, IDCANCEL);
	}
}

INT_PTR CALLBACK CLoginModalDlg::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}
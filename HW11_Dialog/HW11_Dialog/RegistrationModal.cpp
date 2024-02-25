#include "RegistrationModalDlg.h"
#include "User.h"

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
	EndDialog(hwnd, IDCANCEL);
}

BOOL CRegistrationModalDlg::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	return TRUE;
}

void CRegistrationModalDlg::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	if (id == IDOK)
	{
		HWND hEditLogin = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
		HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
		HWND hEditFullName = GetDlgItem(hwnd, IDC_EDIT_FULLNAME);
		HWND hEditAge = GetDlgItem(hwnd, IDC_EDIT_AGE);

		User n;
		const int bufSize = 128;
		GetWindowText(hEditLogin, n.Login, bufSize);
		GetWindowText(hEditPassword, n.Password, bufSize);
		GetWindowText(hEditFullName, n.FullName, bufSize);
		GetWindowText(hEditAge, n.Age, bufSize);

		if (n.SpaceCheck(n.Login) || n.SpaceCheck(n.Password))
		{
			MessageBox(hwnd, L"Password and login must not contain space", L"error", MB_OK);
		}
		else if (n.EmptyCheckReg())
		{
			MessageBox(hwnd, L"Fields must not be empty", L"error", MB_OK);
		}
		else if (n.AgeCheck(n.Age))
		{
			MessageBox(hwnd, L"Age must be a number", L"error", MB_OK);
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
						MessageBox(hwnd, L"User with this login already exists.", L"Login Found", MB_OK);
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
					MessageBox(hwnd, TEXT("Account created!"), TEXT("Registration"), MB_OK | MB_ICONINFORMATION);
					EndDialog(hwnd, IDCANCEL);
					wcout << n.Login << " " << n.Password << " " << n.FullName << " " << n.Age << endl;
				}
				else
				{
					MessageBox(hwnd, L"Can't open file", TEXT("File error"), MB_OK);
				}
			}
		}
	}
	else if (id == IDNO)
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

// ProcKill.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "ProcKill.h"

using namespace std;
//#if _DEBUG
//#include <vld.h>
//#endif

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名
HWND hWndSearch = NULL;
DWORD dwPID;


// 此代码模块中包含的函数的前向声明:

INT_PTR CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam);

INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("Terminate Process From Window"));
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return GetLastError();
	}

	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG_MAIN), NULL,
		              DialogProc);

}



INT_PTR CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	
	POINT  ptCur;
	TCHAR  szInfo[MAX_LOADSTRING] = {0};
	//TCHAR  szWindowName[MAX_LOADSTRING] = {0};
	RECT   rcPic;
	HICON  hIcon;
	HCURSOR hCursor;
	static BOOL    bFlag = FALSE;
	static int i = 2;
	static int nItem = 1;
	static HWND hWnd = NULL;
	static HMENU hMenu = NULL;
	static HMENU hSysMenu = NULL;
	static vector<HWND> vWindow;
	HWND   hMenuWindow = NULL;
    #define IDM_RESTOR_ALL 0
    #define IDM_MENU_ABOUT      65535
	switch (uMsg)
	{
	case WM_INITDIALOG:
        //SetWindowLongPtr(GetDlgItem(hwndDlg, IDC_PIC_SEARCH), GWL_WNDPROC, (LONG)StaticProc);
		hWndSearch = GetDlgItem(hwndDlg, IDC_PIC_SEARCH);
		hInst      = GetModuleHandle(NULL);
		
		InitMenu(&hMenu);
		hSysMenu = GetSystemMenu(hwndDlg, FALSE);
		AppendMenu(hSysMenu, MF_STRING, IDM_MENU_ABOUT, TEXT("关于"));
		//InsertMenu(hSysMenu, 0, MF_STRING, IDM_MENU_ABOUT, TEXT("关于"));
		//ShowWindow(hwndDlg, SW_HIDE);
		HideWindow();
		if (!RegisterHotKey(hwndDlg, 0x1125, MOD_CONTROL, 'R'))
		{
			MessageBox(NULL, TEXT("注册热键失败!"), TEXT("Window Process Tool"), MB_ICONERROR);
			goto exit;
		}

		EnableDebugPrivilege();
		return (INT_PTR)TRUE;
	case WM_LBUTTONDOWN:
		GetWindowRect(hWndSearch, &rcPic);
		ptCur.x = GET_X_LPARAM(lParam);
		ptCur.y = GET_Y_LPARAM(lParam);
		ClientToScreen(hwndDlg, &ptCur);
		if (PtInRect(&rcPic, ptCur))
		{
			//MessageBox(NULL, TEXT("In Region"), TEXT("Info"), 0);
			SetCapture(hwndDlg);

			hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_DRAG));

			dwPID = GetLastError();
			if (hIcon != NULL)
			{
				SendMessage(hWndSearch, STM_SETICON,(WPARAM)hIcon, NULL);
				SetTimer(hwndDlg, 1, 100, NULL);
			}
			hCursor = LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_SEARCH));

			if (hCursor != NULL)
			{
				SetCursor(hCursor);
			}
			
		}
		break;
	case WM_LBUTTONUP:
		ReleaseCapture();
		KillTimer(hwndDlg, 1);
		hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_NORMAL));
		if (hIcon != NULL)
		{
			SendMessage(hWndSearch, STM_SETICON, (WPARAM)hIcon, NULL);
		}
		break;
	case WM_TIMER:
		if (wParam == 1)
		{
			GetCursorPos(&ptCur);
			hWnd = WindowFromPoint(ptCur);
			if (hWnd != hwndDlg && hWnd != NULL/* && SKIP_CHILD_ITEM(IDC_BUTTON_QUITE)
				&& SKIP_CHILD_ITEM(IDC_BUTTON_TERMINATE) && SKIP_CHILD_ITEM(IDC_EDIT_HANDLE)
				&& SKIP_CHILD_ITEM(IDC_EDIT_PROCESS) && SKIP_CHILD_ITEM(IDC_EDIT_TITLE) 
				&& SKIP_CHILD_ITEM(IDC_PIC_SEARCH) && SKIP_CHILD_ITEM(IDC_BUTTON_HIDE) && SKIP_CHILD_ITEM(IDC_BUTTON_SHOW)
				&& SKIP_CHILD_ITEM(IDC_BUTTON_SUSPEND) && SKIP_CHILD_ITEM(IDC_BUTTON_RESUME)*/ && !IsChild(hwndDlg, hWnd))
			{
				wsprintf(szInfo, TEXT("%x"), hWnd);
				_wcsupr_s(szInfo, MAX_LOADSTRING);
				SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT_HANDLE), szInfo);

				GetWindowText(hWnd, szInfo, MAX_LOADSTRING);

				SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT_TITLE), szInfo);

				GetWindowThreadProcessId(hWnd, &dwPID);

				wsprintf(szInfo, TEXT("%d"), dwPID);
				SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT_PROCESS), szInfo);
			}
			
		}
		break;
	case WM_CLOSE:
        exit:
		EndDialog(hwndDlg, 0);
		PostQuitMessage(0);
		break;
	case WM_HOTKEY:
		if (wParam == 0x1125)
		{
			if (bFlag)
			{
				//ShowWindow(hwndDlg, SW_HIDE);
				HideWindow();
				bFlag = FALSE;
			}
			else
			{
				//ShowWindow(hwndDlg, SW_SHOW);
				VisibleWindow();
				SetWindowPos(hwndDlg, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
				bFlag = TRUE;
			}
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_QUITE:
			goto exit;
			break;
		case IDC_BUTTON_TERMINATE: 
			KillProcess(dwPID);
			break;
		case IDC_BUTTON_SUSPEND:
			SuspendProcess(dwPID, TRUE);
			break;
		case IDC_BUTTON_RESUME:
			SuspendProcess(dwPID, FALSE);
			break;
		
		case IDC_BUTTON_HIDE:
			if (hWnd != NULL && hMenu != NULL && IsWindowVisible(GetAncestor(hWnd, GA_ROOT)))
			{
				//SendMessage(hWnd, WM_SHOWWINDOW, SW_HIDE,  0);
				ShowWindow(GetAncestor(hWnd, GA_ROOT), SW_HIDE);
				vWindow.push_back(GetAncestor(hWnd, GA_ROOT));
				GetWindowText(GetAncestor(hWnd, GA_ROOT), szInfo, MAX_LOADSTRING);
				hWnd = NULL;
				if (wcslen(szInfo))
				{
					AppendMenu(hMenu, MF_STRING, nItem++, szInfo);
					//vMenuId.push_back(nItem-1);
				}
			}
			break;
		case IDC_BUTTON_SHOW:
			GetCursorPos(&ptCur);
			TrackPopupMenu(hMenu, TPM_LEFTALIGN, ptCur.x, ptCur.y, 0, hwndDlg, NULL);
			break;
		}
		break;
	case WM_MENUCOMMAND:
		if ((HMENU)lParam == hMenu)
		{
			if ((UINT)wParam == IDM_RESTOR_ALL)
			{
				vector<HWND>::iterator pWindow;
				//vector<int>:: iterator pMenu;
				for (pWindow = vWindow.begin(); pWindow < vWindow.end(); ++pWindow)
				{
					ShowWindow(*pWindow, SW_SHOW);
				}
				DestroyMenu(hMenu);
				vWindow.clear();
				InitMenu(&hMenu);
			}
			else
			{
				if (!GetMenuString(hMenu, (UINT)wParam, szInfo, MAX_LOADSTRING, MF_BYPOSITION))
				{
					return FALSE;
				}
				hMenuWindow = FindWindow(NULL, szInfo);

				if (hMenuWindow == NULL)
				{
					return FALSE;
				}
				if (!IsWindowVisible(hMenuWindow))
				{
					ShowWindow(hMenuWindow, SW_SHOW);
					DeleteMenu(hMenu, (UINT)wParam, MF_BYPOSITION);
				}
			}
		}
		break;
	case  WM_SYSCOMMAND:
		if ( wParam  == IDM_MENU_ABOUT)
		{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwndDlg, About);
		}
		else if (wParam == SC_CLOSE)
		{
			HideWindow();
			return (INT_PTR)TRUE;
		}
		
	}
	
	return (INT_PTR)FALSE;
}


// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

HANDLE OpenProcess(DWORD dwPID)
{
	HMODULE hDll = NULL;
	HANDLE hProcess = NULL;
	__try
	{
		hDll = LoadLibrary(TEXT("ntdll.dll"));

		_ASSERT(hDll != NULL);

		ZWOPENPROCESS ZwOpenProcess = (ZWOPENPROCESS)GetProcAddress(hDll,
			"ZwOpenProcess");
		if (ZwOpenProcess == NULL)
		{
			__leave;
		}

		
		OBJECT_ATTRIBUTES oa;
		oa.Length     = sizeof(OBJECT_ATTRIBUTES);
		oa.Attributes = 0;
		oa.ObjectName = NULL;
		oa.RootDirectory = NULL;
		oa.SecurityDescriptor = NULL;
		oa.SecurityQualityOfService = NULL;

		CLIENT_ID cid;
		cid.UniqueProcess = (HANDLE)dwPID;
		cid.UniqueThread  = 0;
		ZwOpenProcess(&hProcess, PROCESS_TERMINATE | PROCESS_SUSPEND_RESUME, &oa, &cid);

		if (hProcess == NULL)
		{
			__leave;
		}
     }

	__finally
	{
		if (hDll)
		{
			FreeLibrary(hDll);
			hDll = NULL;
		}
	}

	return hProcess;
	
}

BOOL KillProcess(DWORD dwPID)
{
	HMODULE hDll = LoadLibrary(TEXT("ntdll.dll"));
	if (hDll == NULL)
	{
		return FALSE;
	}

	HANDLE hProcess = OpenProcess(dwPID);
	ZWTERMINATEPROCESS zwTerminateProcess = (ZWTERMINATEPROCESS)GetProcAddress(hDll,
			"ZwTerminateProcess");
    if (zwTerminateProcess == NULL)
	{
	     return FALSE;
	}
	if (hProcess == NULL)
	{
		return FALSE;
	}
		
	__try
	{
		zwTerminateProcess(hProcess, 0);
	}

	__except(EXCEPTION_CONTINUE_EXECUTION)
	{
		
	}


	FreeLibrary(hDll);
	hDll = NULL;

	return TRUE;
	
}

BOOL SuspendProcess(DWORD dwPID, BOOL bSuspend)
{
	HMODULE hDll = NULL;
	__try
	{
		hDll = LoadLibrary(TEXT("ntdll.dll"));

		_ASSERT(hDll != NULL);

		ZWSUSPENDPROCESS ZwSuspendProcess = (ZWSUSPENDPROCESS)GetProcAddress(hDll,
			"ZwSuspendProcess");
		if (ZwSuspendProcess == NULL)
		{
			__leave;
		}

		ZWRESUMEPROCESS ZwResumeProcess = (ZWRESUMEPROCESS)GetProcAddress(hDll,
			"ZwResumeProcess");
		if (ZwSuspendProcess == NULL)
		{
			__leave;
		}

		HANDLE hProcess = OpenProcess(dwPID);

		if (hProcess == NULL)
		{
			__leave;
		}

		if (bSuspend)
		{
			ZwSuspendProcess(hProcess);
		}
		else
		{ 
			ZwResumeProcess(hProcess);
		}
		
	}

	__finally
	{
		if (hDll)
		{
			FreeLibrary(hDll);
			hDll = NULL;
		}
	}

	return TRUE;
}

void InitMenu(HMENU *pMenu)
{
	MENUINFO mi = {0};
	mi.cbSize = sizeof(MENUINFO);      
	mi.fMask  = MIM_STYLE;             
	mi.dwStyle = MNS_NOTIFYBYPOS;
    *pMenu = CreatePopupMenu();         
          
	SetMenuInfo(*pMenu, &mi);           
	AppendMenu(*pMenu, MF_STRING, IDM_RESTOR_ALL, TEXT("显示全部窗口"));
}

BOOL EnableDebugPrivilege()
{
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tp;
	LUID			 luid;
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);
	if (!hToken)
	{
		return FALSE;
	}
	LookupPrivilegeValue(NULL,  SE_DEBUG_NAME, &luid);
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	return AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
}
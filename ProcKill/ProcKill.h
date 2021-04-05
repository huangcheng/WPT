#pragma once

#include "resource.h"
#include <Windowsx.h>
#include <crtdbg.h>
#include <vector>
/*
#define SKIP_CHILD_ITEM(ITEM_ID) hWnd != GetDlgItem(hwndDlg, ITEM_ID)
*/
#define  HideWindow() SetLayeredWindowAttributes(hwndDlg, NULL, 0, LWA_ALPHA)
#define  VisibleWindow() SetLayeredWindowAttributes(hwndDlg, NULL, 255, LWA_ALPHA)

//HWND hWnd;


typedef struct _OBJECT_ATTRIBUTES {
	ULONG  Length;
	HANDLE  RootDirectory;
	PVOID  ObjectName;
	ULONG  Attributes;
	PVOID  SecurityDescriptor;
	PVOID  SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

typedef struct _CLIENT_ID
{
	PVOID UniqueProcess;
	PVOID UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef DWORD (__stdcall *ZWOPENPROCESS)(PHANDLE  ProcessHandle,
	                                     ACCESS_MASK  DesiredAccess,
	                                     POBJECT_ATTRIBUTES  ObjectAttributes,
	                                     PCLIENT_ID  ClientId
	                                    );

typedef DWORD (__stdcall *ZWTERMINATEPROCESS)(HANDLE  ProcessHandle,
	                                          DWORD ExitStatus
                                             );

typedef DWORD (__stdcall *ZWSUSPENDPROCESS)(HANDLE ProcessHandle);
typedef DWORD (__stdcall *ZWRESUMEPROCESS)(HANDLE ProcessHandle);

BOOL KillProcess(DWORD dwPID);
HANDLE OpenProcess(DWORD dwPID);
BOOL SuspendProcess(DWORD dwPID, BOOL bSuspend);
BOOL EnableDebugPrivilege();
void InitMenu(HMENU *pMenu);
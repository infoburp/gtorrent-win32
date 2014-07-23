#include "stdafx.h"
#include "gTorrent Win32.h"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <commctrl.h>
#include <string.h>
#include <Lm.h>
#include <stdlib.h>
#include <vector>
#include <stdint.h>
//#include <gtorrent\Core.hpp>

// Global Variables
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND hListView;									// Handle for the main torrent list view
//gt::Core m_core;
//vector<shared_ptr<gt::Torrent>> torrents;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
    icex.dwICC = ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icex);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GTORRENTWIN32, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GTORRENTWIN32));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)	//Register window class
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GTORRENTWIN32));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_GTORRENTWIN32);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)	//Save instance handle and display entry window
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
      return false;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return true;
}

void AddTorrent (wchar_t name[])
{
	//shared_ptr<gt::Torrent> t = m_core.addTorrent("C://");

	/*for(int i = 0; i < torrents.size(); i++)
	{
		ListView_DeleteAllItems(hListView);

		LVITEM lv = { 0 };
		lv.iItem = 0;

		ListView_InsertItem(hListView, &lv);

		ListView_SetItemText(hListView, i, 0, L"1");		//number
		ListView_SetItemText(hListView, i, 1, name);		//name
		ListView_SetItemText(hListView, i, 2, NULL);		//seed
		ListView_SetItemText(hListView, i, 3, NULL);		//leech
		ListView_SetItemText(hListView, i, 4, NULL);		//up
		ListView_SetItemText(hListView, i, 5, NULL);		//down
		ListView_SetItemText(hListView, i, 6, NULL);		//progress
	}*/
}

HWND CreateTorrentListView (HWND hwndParent) 
{
	RECT rcWindow;
	GetWindowRect(hwndParent, &rcWindow);

    hListView = CreateWindow( WC_LISTVIEW, L"List", WS_VISIBLE | WS_CHILD | LVS_REPORT,
		0, 30, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top, hwndParent, (HMENU)500, hInst, NULL); 

	ListView_SetExtendedListViewStyle(hListView,  LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP);

	LVCOLUMN lvc = { 0 };
	lvc.mask = LVCF_TEXT | LVCF_WIDTH;


	//2d array of characters because win32 is a nigger and doesn't do strings
	//set 20 as length of each string because I had to predefine it and how many words are there more than 20 characters unless you're german?
	wchar_t titles[7][20] = {L"#", L"Name", L"Seed", L"Leech", L"Up", L"Down", L"Progress"};

	int widths[7] = { 25, 200, 50, 50, 80, 80, 200 };		//widths of respective columns in pixels

	for (int i = 0; i < 7; i++)
	{
		lvc.iSubItem = i;
		lvc.cx       = widths[i];
		lvc.pszText  = titles[i];
		ListView_InsertColumn(hListView, i, &lvc);
	}

    return (hListView);
}

HWND CreateToolbar (HWND hwndParent)
{
	const int numButtons = 5;

	RECT rcWindow;
	GetWindowRect(hwndParent, &rcWindow);

	HWND hToolbar = CreateWindow(TOOLBARCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | TBSTYLE_FLAT | TBSTYLE_LIST, 
		0, 0, rcWindow.right - rcWindow.left, 50, hwndParent, (HMENU)501, GetModuleHandle(NULL), NULL);

	SendMessage(hToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM) sizeof(TBBUTTON), 0);

	TBBUTTON tbButtons[numButtons] = 
	{
		{0, IDT_START, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)L"Start"},
		{0, IDT_STOP, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)L"Stop"},
		{0, IDT_MOVE_UP, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)L"Move Up"},
		{0, IDT_MOVE_DOWN, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)L"Move Down"},
		{0, IDT_PREFERENCES, TBSTATE_ENABLED, BTNS_BUTTON, {0}, 0, (INT_PTR)L"Preferences"}
	}; 

	SendMessage(hToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
	SendMessage(hToolbar, TB_ADDBUTTONS, numButtons, (LPARAM)&tbButtons);

	return hToolbar;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)	//Process events for main window
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	HWND hWnd_TorrentList, hWnd_ToolBar = NULL;

	switch (message)
	{
		case WM_CREATE:
			hWnd_TorrentList = CreateTorrentListView(hWnd);
			hWnd_ToolBar = CreateToolbar(hWnd);
			break;
		case WM_COMMAND:
			wmId    = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			
			switch (wmId)
			{
				case IDM_ADD_T:
					AddTorrent(L"ubuntu-14.04-desktop-amd64.iso");
					break;
				case IDM_ADD_M:
					MessageBox(hWnd, L"Add a torrent from magnet: link", L"Add Magnet", 1);
					break;
				case IDM_ABOUT:
					MessageBox(hWnd, L"gTorrent", L"About gTorrent", 0);
					break;

				case IDT_START:
					MessageBox(hWnd, L"Start", L"Start", 0);
					break;
				case IDT_STOP:
					MessageBox(hWnd, L"Stop", L"Stop", 0);
					break;
				case IDT_PREFERENCES:
					MessageBox(hWnd, L"Preferences", L"Preferences", 1);
					break;

				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}


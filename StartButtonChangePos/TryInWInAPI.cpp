
#include "TryInWinAPI.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hTaskBar, hStartButton;					// WINDOWS TASK BAR/STARTBUTTON
HMENU hMainMenu;								// MENU IN MY WINDOW
int buttonPosY = GetSystemMetrics(SM_CYSCREEN) - 23; // POS Y START BUTTON
int buttonPosX = 4;									 // POS X START BUTTON




// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK EnumWindowsFn(HWND hWnd, LPARAM lParam);
BOOL CALLBACK EnumChildWindowsFn(HWND hWnd, LPARAM lParam);
BOOL CALLBACK EnumWindowsFnMin(HWND hWnd, LPARAM lParam);
BOOL CALLBACK EnumWindowsFnRape(HWND hWnd, LPARAM lPAram);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	srand(time(NULL));
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	hTaskBar = FindWindow(L"Shell_TrayWnd", NULL);
	HWND hStartButtonWithoutParent = FindWindow(L"Start", NULL);
	SetParent(hStartButtonWithoutParent, hTaskBar);
	hStartButton = FindWindowEx(hTaskBar, 0, L"Start", NULL);
	hMainMenu = LoadMenu(hInstance, (LPCWSTR)IDC_STARTBUTTONCHANGEPOS);
	
    
	//Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_STARTBUTTONCHANGEPOS, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STARTBUTTONCHANGEPOS));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STARTBUTTONCHANGEPOS));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_STARTBUTTONCHANGEPOS);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_DLGFRAME | WS_SYSMENU | WS_MINIMIZEBOX,
	   CW_USEDEFAULT, CW_USEDEFAULT, 340, 200, nullptr, hMainMenu, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i = 0;
	int windowsCount = 0; //FOR TABLE HWND
	int cyS, cxS; //MOUSE POS
	cyS = GetSystemMetrics(SM_CYSCREEN);
	cxS = GetSystemMetrics(SM_CXSCREEN);
	HWND wnd;
	HWND wndT[1000]; //TABLE HWND
	HWND sleep; //FOR WaitForSingleObject(...)
	POINT pt; //pt.x,pt.y;
	RECT rect;
	HDWP hDwp; // For DeferWindowPos
	try { pt = { rand() % cxS, rand() % cyS }; }
	catch (...) { throw("FAIL"); return -1; }

		switch (message)
		{
		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			// Parse the menu selections:
			switch (wmId)
			{
			case IDM_ABOUT:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
				break;
			case IDM_EXIT:
				DestroyWindow(hWnd);
				break;
				//======TASK BAR======
			case ID_TASKBAR_ENABLE:
				EnableWindow(hTaskBar, TRUE);
				break;
			case ID_TASKBAR_DISABLE:
				EnableWindow(hTaskBar, FALSE);
				break;
			case ID_TASKBAR_SHOW:
				ShowWindow(hTaskBar, SW_SHOW);
				break;
			case ID_TASKBAR_HIDE:
				ShowWindow(hTaskBar, SW_HIDE);
				break;
				//======START BUTTON======
			case ID_STARTBUTTON_DISABLE:
				ShowWindow(hStartButton, SW_HIDE);
				break;
			case ID_STARTBUTTON_ENABLE:
				ShowWindow(hStartButton, SW_SHOW);
				break;
			case ID_STARTBUTTON_MOVE:
				SetParent(hStartButton, 0);
				HANDLE sleep;
				sleep = CreateEvent(0, true, false, L"sleep");
				SetWindowPos(hStartButton, HWND_TOPMOST, buttonPosX, buttonPosY, 36, 26, SWP_SHOWWINDOW);
				for (size_t i = 0; i < 20; i++)
				{
					buttonPosY = rand() % 768;
					buttonPosX = rand() % 1366;
					SetWindowPos(hStartButton, HWND_TOPMOST, buttonPosX, buttonPosY, 36, 26, SWP_SHOWWINDOW);
					WaitForSingleObject(sleep, 500);
				}
				SetParent(hStartButton, hTaskBar);
				SetWindowPos(hStartButton, HWND_TOP, 0, 716, 36, 26, SWP_SHOWWINDOW);
				break;
			case ID_STARTBUTTON_SETPOS:
				SetParent(hStartButton, 0);
				SetWindowPos(hStartButton, HWND_TOP, 0, 716, 36, 26, SWP_SHOWWINDOW);
				break;
				//======OPEN======
			case ID_OPEN_CHROME:
				ShellExecute(hWnd, L"Open", L"chrome.exe", L"pornhub.com", L"", SW_SHOWNORMAL);
				break;
			case ID_OPEN_GOD:
				ShellExecute(hWnd, L"Open", L"chrome.exe", L"www.galopuje.pl/foto_news/kon580.jpeg", L"", SW_SHOWNORMAL);
				break;
			case ID_OPEN_SILENT:
				ShellExecute(hWnd, L"Open", L"taskmgr.exe", L"", L"", SW_MINIMIZE | SWP_HIDEWINDOW | SW_HIDE);
				break;
				//======MOUSE======
			case ID_MOUSE_PIZDORVANKA:
				//SetSystemCursor(GetCursor(), (DWORD)32512);
				while (i < 5)
				{
					wnd = GetForegroundWindow();
					ShowWindow(wnd, SW_HIDE);
					EnableWindow(wnd, false);
					sleep = CreateEvent(0, false, true, L"sleep");
					WaitForSingleObject(sleep, 500);
					ShowWindow(wnd, SW_SHOW);
					EnableWindow(wnd, true);
					sleep = CreateEvent(0, false, true, L"sleep");
					WaitForSingleObject(sleep, 500);
					i++;
				}
				for (i = 0; i < 20; i++)
				{
					pt = { rand() % cxS,rand() % cyS };
					SetCursorPos(pt.x, pt.y);
					wnd = WindowFromPoint(pt);
					SendMessage(wnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELONG(pt.x, pt.y));
					Sleep(300);
					pt = { rand() % cxS,rand() % cyS };
					SendMessage(wnd, WM_MOUSEMOVE, 0, MAKELONG(pt.x, pt.y));
					Sleep(300);
					SendMessage(wnd, WM_LBUTTONUP, MK_LBUTTON, MAKELONG(pt.x, pt.y));
				}
				break;
			case ID_MOUSE_JAIL:
				RECT r;
				r.bottom = 50;
				r.top = 50;
				r.left = 50;
				r.right = 50;
				ClipCursor(&r);
				break;
			case ID_MOUSE_SHIT:
				SetSystemCursor(GetCursor(),(DWORD)32648);
				SystemParametersInfo(SPI_SETMOUSESPEED, NULL, (PVOID)1, SPIF_UPDATEINIFILE);
				SystemParametersInfo(SPI_SETMOUSECLICKLOCKTIME,NULL,(PVOID)1,SPIF_UPDATEINIFILE);
				SystemParametersInfo(SPI_SETMOUSECLICKLOCK, NULL, (PVOID)TRUE, SPIF_UPDATEINIFILE);
				break;
			//======DESTROY======
			//======OTHER======
			case ID_FUNCTIONS_SETTITLE:
				while (TRUE)
				{
					EnumWindows(&EnumWindowsFn, 0);
				}
				break;
			case ID_FUNCTIONS_MINIMIZEWINDOWS:
				while (TRUE)
				{
					EnumWindows(&EnumWindowsFnMin, 0);
				}
				break;
			case ID_FUNCTIONS_RAPEWINDOWS:
				while (TRUE)
				{
					EnumWindows(&EnumWindowsFnRape, 0);
				}
				break;
			case ID_FUNCTIONS_VISHELNAHOOY:
				while (true)
				{
					windowsCount = 0;
					for (int i = 0; i < GetSystemMetrics(SM_CYSCREEN); i++) {
						wnd = GetWindow(GetDesktopWindow(), GW_CHILD);
						while (wnd != 0)
						{
							if (IsWindowVisible(wnd))
							{
								wndT[windowsCount] = wnd;
								windowsCount++;
							}
							wnd = GetWindow(wnd, GW_HWNDNEXT);
						}
						hDwp = BeginDeferWindowPos(windowsCount);
						for (size_t i = 0; i <= windowsCount; i++)
						{
							GetWindowRect(wndT[i], &rect);
							hDwp = DeferWindowPos(hDwp, wndT[i], HWND_BOTTOM,
								rect.left - 20, rect.top - 20, rect.right - rect.left, rect.bottom - rect.top, SWP_NOACTIVATE | SWP_NOZORDER);
						}
						EndDeferWindowPos(hDwp);
					}
					sleep = CreateEvent(0, true, false, L"sleep");
					WaitForSingleObject(sleep, 300);
				}
				break;
			case ID_FUNCTIONS_DISABLESCREEN:
				SendMessage(hWnd, WM_SYSCOMMAND, (WPARAM)SC_MONITORPOWER, 2);
				break;
			case ID_FUNCTIONS_INSERTMENU:
				SetMenu(hTaskBar, hMainMenu);
				break;
			case ID_FUNCTIONS_EXTRACTMENU:
				SetMenu(hTaskBar, NULL);
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
				break;
			}
		}
		break;
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code that uses hdc here...
			EndPaint(hWnd, &ps);
		}
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
}

// Message handler for about box.
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


BOOL CALLBACK EnumWindowsFn(HWND hWnd, LPARAM lParam)
{
	if (hWnd == 0)
		return TRUE;
	if (!IsWindowVisible(hWnd))
		return TRUE;
	SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)L"ADSKAYA DROCHILNIA");
	EnumChildWindows(hWnd, &EnumChildWindowsFn, 0);
	return TRUE;
}

BOOL CALLBACK EnumChildWindowsFn(HWND hWnd, LPARAM lParam)
{
	if (hWnd == 0)
		return TRUE;
	SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)L"ADSKAYA DROCHILNIA");
}

BOOL CALLBACK EnumWindowsFnMin(HWND hWnd, LPARAM lParam)
{
	if (hWnd == 0)
		return TRUE;
	if (!IsWindowVisible(hWnd))
		return TRUE;
	ShowWindow(hWnd, SW_MINIMIZE);
}

BOOL CALLBACK EnumWindowsFnRape(HWND hWnd, LPARAM lPAram)
{
	if (!IsWindowVisible(hWnd))
		return TRUE;
	RECT rect;
	GetWindowRect(hWnd, &rect);
	int random = rand() % 2;
	if (random == 0)
	{
		rect.left = rect.left - 3;
		rect.top = rect.top - 3;
	}
	else if (random == 1)
	{
		rect.left = rect.left + 3;
		rect.top = rect.top + 3;
	}
	else
	{
		return FALSE;
	}
	MoveWindow(hWnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);
	return TRUE;
}
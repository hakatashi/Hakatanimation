#include <windows.h>
#include <tchar.h>

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 300
#define WINDOW_X ((GetSystemMetrics( SM_CXSCREEN ) - WINDOW_WIDTH ) / 2)
#define WINDOW_Y ((GetSystemMetrics( SM_CYSCREEN ) - WINDOW_HEIGHT ) / 2)
#define TIMER_ID 100
#define TIMER_ELAPSE 30
#define WM_TASKTRAY WM_USER+1
#define ID_TASKTRAY 0
#define ID_DUMMY 0
#define ID_EXIT 1

HWND Create(HINSTANCE hInst);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int showCmd) {
	HWND hWnd;
	MSG msg;

	hWnd = Create( hInst );
	if( hWnd == NULL ) {
		MessageBox( NULL, _T("ウィンドウの作成に失敗しました"), _T("エラー"), MB_OK );
		return 1;
	}

	ShowWindow( hWnd, SW_SHOW );
	UpdateWindow( hWnd );

	SetTimer( hWnd, TIMER_ID, TIMER_ELAPSE, NULL );

	while (1) {
		BOOL ret = GetMessage( &msg, NULL, 0, 0 ); 
		if( ret == 0 || ret == -1 ) {
			break;
		} else {
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	KillTimer( hWnd, TIMER_ID );

	return 0;
}

HWND Create(HINSTANCE hInst) {
	WNDCLASSEX wc;
	HWND g_hWnd;

	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(hInst , TEXT("KITTY"));
	wc.hIconSm = wc.hIcon;
	wc.hCursor = (HCURSOR)LoadImage(
		NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR,
		0, 0, LR_DEFAULTSIZE | LR_SHARED
	);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _T("Default Class Name");
	
	if( RegisterClassEx( &wc ) == 0 ) {
		return NULL;
	}

	g_hWnd = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		wc.lpszClassName,
		_T("Sample Program"),
		WS_OVERLAPPEDWINDOW,
		0, 0, 0, 0,
		NULL,
		NULL,
		hInst,
		NULL
	);
	
	NOTIFYICONDATA nif;
	nif.cbSize = sizeof( NOTIFYICONDATA );
	nif.hIcon = LoadIcon(hInst , TEXT("KITTY"));
	nif.hWnd = g_hWnd;
	nif.uCallbackMessage = WM_TASKTRAY;
	nif.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nif.uID = ID_TASKTRAY;
	strcpy( nif.szTip, _T("博多アニメーション") );

    Shell_NotifyIcon( NIM_ADD, &nif );
	
	return g_hWnd;
}
	
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	HDC hDC;
	PAINTSTRUCT ps;
	DWORD TopRect, LeftRect;
	static HMENU hMenu, hSubMenu;

	switch( msg ) {
	case WM_CREATE:
		hMenu = LoadMenu( NULL, _T("IDR_POPUP") );
		hSubMenu = GetSubMenu( hMenu, 0 );
		return 0;
	
	case WM_TIMER:
		if( wp != TIMER_ID )
		{
			break;
		}
		return 0;

	case WM_TASKTRAY:
		POINT point;
		GetCursorPos(&point);

		switch (lp) {
		case WM_RBUTTONDOWN:
			SetForegroundWindow(hWnd);
			TrackPopupMenu( hSubMenu, TPM_LEFTALIGN | TPM_BOTTOMALIGN, point.x, point.y, 0, hWnd, NULL );
			break;
		}
		return 0;

	case WM_COMMAND:
		switch( LOWORD(wp) ) {
		case ID_EXIT:
			SendMessage( hWnd, WM_CLOSE, 0, 0 );
			break;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage( 0 );
		return 0;
	}

	return DefWindowProc( hWnd, msg, wp, lp );
}
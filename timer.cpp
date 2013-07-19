#include <windows.h>
#include <tchar.h>

// 定数
#define WINDOW_WIDTH  (400)		// ウィンドウの幅
#define WINDOW_HEIGHT (300)		// ウィンドウの高さ
#define WINDOW_X ((GetSystemMetrics( SM_CXSCREEN ) - WINDOW_WIDTH ) / 2)
#define WINDOW_Y ((GetSystemMetrics( SM_CYSCREEN ) - WINDOW_HEIGHT ) / 2)
#define TIMER_ID     (100)      // 作成するタイマの識別ID
#define TIMER_ELAPSE (30)     // WM_TIMERの発生間隔
#define WM_TASKTRAY (WM_USER+1)
#define ID_TASKTRAY 0
#define ID_DUMMY   0
#define ID_EXIT    1

// プロトタイプ宣言
HWND Create(HINSTANCE hInst);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);


// 開始位置
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int showCmd)
{
	HWND hWnd;
	MSG msg;

	// ウィンドウを作成する
	hWnd = Create( hInst );
	if( hWnd == NULL )
	{
		MessageBox( NULL, _T("ウィンドウの作成に失敗しました"), _T("エラー"), MB_OK );
		return 1;
	}

	// ウィンドウを表示する
	ShowWindow( hWnd, SW_SHOW );
	UpdateWindow( hWnd );

	// タイマを作成する
	SetTimer( hWnd, TIMER_ID, TIMER_ELAPSE, NULL );

	// メッセージループ
	while( 1 )
	{
		BOOL ret = GetMessage( &msg, NULL, 0, 0 );  // メッセージを取得する
		if( ret == 0 || ret == -1 )
		{
			// アプリケーションを終了させるメッセージが来ていたら、
			// あるいは GetMessage() が失敗したら( -1 が返されたら ）、ループを抜ける
			break;
		}
		else
		{
			// メッセージを処理する
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	// タイマを破棄する
	KillTimer( hWnd, TIMER_ID );

	return 0;
}

// ウィンドウを作成する
HWND Create(HINSTANCE hInst)
{
	WNDCLASSEX wc;
	HWND g_hWnd;

	// ウィンドウクラスの情報を設定
	wc.cbSize = sizeof(wc);               // 構造体サイズ
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;   // スタイル
	wc.lpfnWndProc = WndProc;             // ウィンドウプロシージャ
	wc.cbClsExtra = 0;                    // 拡張情報１
	wc.cbWndExtra = 0;                    // 拡張情報２
	wc.hInstance = hInst;                 // インスタンスハンドル
	wc.hIcon = LoadIcon(hInst , TEXT("KITTY"));          // アイコン
	wc.hIconSm = wc.hIcon;                // 子アイコン
	wc.hCursor = (HCURSOR)LoadImage(      // マウスカーソル
		NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR,
		0, 0, LR_DEFAULTSIZE | LR_SHARED
	);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // ウィンドウ背景
	wc.lpszMenuName = NULL;                     // メニュー名
	wc.lpszClassName = _T("Default Class Name");// ウィンドウクラス名
	
	// ウィンドウクラスを登録する
	if( RegisterClassEx( &wc ) == 0 ){ return NULL; }

	// ウィンドウを作成する
	g_hWnd = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		wc.lpszClassName,      // ウィンドウクラス名
		_T("Sample Program"),  // タイトルバーに表示する文字列
		WS_OVERLAPPEDWINDOW,   // ウィンドウの種類
		0,              // ウィンドウを表示する位置（X座標）
		0,              // ウィンドウを表示する位置（Y座標）
		0,          // ウィンドウの幅
		0,         // ウィンドウの高さ
		NULL,                  // 親ウィンドウのウィンドウハンドル
		NULL,                  // メニューハンドル
		hInst,                 // インスタンスハンドル
		NULL                   // その他の作成データ
	);
	
	NOTIFYICONDATA nif;
	// タスクトレイに登録
	nif.cbSize				= sizeof( NOTIFYICONDATA );
	nif.hIcon				= LoadIcon(hInst , TEXT("KITTY"));
	nif.hWnd				= g_hWnd;
	nif.uCallbackMessage	= WM_TASKTRAY;
	nif.uFlags				= NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nif.uID					= ID_TASKTRAY;
	strcpy( nif.szTip, _T("博多アニメーション") );

    Shell_NotifyIcon( NIM_ADD, &nif );
	
	return g_hWnd;
}

// ウィンドウプロシージャ
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	HDC hDC;
	PAINTSTRUCT ps;
	DWORD TopRect, LeftRect;
	static HMENU hMenu, hSubMenu;

	switch( msg )
	{
	case WM_CREATE:         // ウィンドウが作られたとき
		// メニューリソースをロードする
		hMenu = LoadMenu( NULL, _T("IDR_POPUP") );
		// サブメニューのハンドルを取得する
		hSubMenu = GetSubMenu( hMenu, 0 );

		return 0;
	
	case WM_TIMER:          // タイマメッセージ
		if( wp != TIMER_ID )
		{
			break;  // 識別IDが一致しないタイマメッセージはDefWindowProc()に任せる
		}

		//InvalidateRect( hWnd, NULL, FALSE );
		return 0;

	case WM_PAINT:          // 無効領域があるとき
		return 0;

	case WM_TASKTRAY: // カーソルの現在位置を取得
		POINT point;
		GetCursorPos(&point);

		switch (lp) {
		case WM_RBUTTONDOWN:
			SetForegroundWindow(hWnd);	// この処理を忘れずに！
			
			TrackPopupMenu( hSubMenu, TPM_LEFTALIGN | TPM_BOTTOMALIGN, point.x, point.y, 0, hWnd, NULL );
			//PostMessage(WM_NULL);	// この処理も忘れずに！
			break;
		}
		return 0;

	case WM_COMMAND:        // 項目が選択されたとき
		switch( LOWORD( wp ) )
		{
		case ID_EXIT:       // 「終了」
			SendMessage( hWnd, WM_CLOSE, 0, 0 );
			break;
		}
		return 0;

	case WM_DESTROY:        // ウィンドウが破棄されるとき
		PostQuitMessage( 0 );
		return 0;
	}

	return DefWindowProc( hWnd, msg, wp, lp );
}
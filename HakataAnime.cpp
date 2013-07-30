#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <string>

#include <curl/curl.h>
#include <boost/regex.hpp>

#define TIMER_ID 100
#define TIMER_ELAPSE 3600000
#define WM_TASKTRAY WM_USER+1
#define ID_TASKTRAY 0
#define ID_DUMMY 0
#define ID_EXECUTE 1
#define ID_EXIT 2

HWND Create(HINSTANCE hInst);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
void DownExecute();
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
void GetIDs(std::vector<int> *video_id);
int InitLoad();

std::set<int> video_index;
std::string username;
std::string password;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int showCmd) {
	HWND hWnd;
	MSG msg;

	hWnd = Create( hInst );
	if( hWnd == NULL ) {
		MessageBox( NULL, _T("ウィンドウの作成に失敗しました"), _T("エラー"), MB_OK );
		return 1;
	}
	
	if ( InitLoad() != 0 ) {
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
		Sleep(100);
	}

	KillTimer( hWnd, TIMER_ID );

	return 0;
}

int InitLoad() {
	std::string temp;
	int id_temp;
	
	boost::regex user_reg("username=(.+)");
	boost::regex pass_reg("password=(.+)");
	
	boost::smatch result;
	
	std::ifstream ifs( "video.index" );
	
	if ( !ifs ) {
		MessageBox( NULL, _T("video.indexの読み込みに失敗しました"), _T("エラー"), MB_OK );
		return 1;
	}
	
	while ( !ifs.eof() ) {
		ifs >> temp;
		id_temp = atoi( temp.c_str() );
		if ( id_temp != 0 ) video_index.insert( atoi( temp.c_str() ) );
	}
	
	std::ifstream cfg_ifs( "Config.ini" );
	
	if ( !cfg_ifs ) {
		MessageBox( NULL, _T("Config.iniの読み込みに失敗しました"), _T("エラー"), MB_OK );
		return 1;
	}
	
	while ( !cfg_ifs.eof() ) {
		cfg_ifs >> temp;
		if (boost::regex_match(temp, result, user_reg)) {
			username.assign( result.str(1) );
		} else if (boost::regex_match(temp, result, pass_reg)) {
			password.assign( result.str(1) );
		}
	}
	
	if ( username.size()==0 ) {
		MessageBox( NULL, _T("usernameが設定されていません"), _T("エラー"), MB_OK );
		return 1;
	}
	
	if ( password.size()==0 ) {
		MessageBox( NULL, _T("passwordが設定されていません"), _T("エラー"), MB_OK );
		return 1;
	}
	
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
	wc.hIcon = LoadIcon(hInst , TEXT("BOX"));
	wc.hIconSm = wc.hIcon;
	wc.hCursor = (HCURSOR)LoadImage(
		NULL, IDC_ARROW, IMAGE_CURSOR,
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
	nif.hIcon = LoadIcon(hInst , TEXT("BOX"));
	nif.hWnd = g_hWnd;
	nif.uCallbackMessage = WM_TASKTRAY;
	nif.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nif.uID = ID_TASKTRAY;
	strcpy( nif.szTip, _T("博多アニメーション") );

	Shell_NotifyIcon( NIM_ADD, &nif );
	
	FreeConsole();
	
	return g_hWnd;
}
	
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	HDC hDC;
	PAINTSTRUCT ps;
	DWORD TopRect, LeftRect;
	static HMENU hMenu, hSubMenu;

	switch (msg) {
	case WM_CREATE:
		hMenu = LoadMenu( NULL, _T("IDR_POPUP") );
		hSubMenu = GetSubMenu( hMenu, 0 );
		return 0;
	
	case WM_TIMER:
		if ( wp == TIMER_ID ) {
			DownExecute();
		}
		return 0;

	case WM_TASKTRAY:
		switch (lp) {
		case WM_RBUTTONDOWN:
			POINT point;
			GetCursorPos(&point);
			SetForegroundWindow(hWnd);
			TrackPopupMenu( hSubMenu, TPM_LEFTALIGN | TPM_BOTTOMALIGN, point.x, point.y, 0, hWnd, NULL );
			break;
		}
		return 0;

	case WM_COMMAND:
		switch( LOWORD(wp) ) {
		case ID_EXECUTE:
			DownExecute();
			break;
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

void GetIDs(std::vector<int> *video_id) {
	CURL *curl;
	CURLcode res;
	std::string data;
	
	boost::regex reg_exp("<li\\s+id=\"video_\\d+_(\\d+)\"\\s+class=\"video cfix( | selected)\">");
	boost::smatch result;
	
	std::string::const_iterator start, end;
	
	curl = curl_easy_init();
	
	if (curl) {
		
		curl_easy_setopt(curl, CURLOPT_URL, "http://ch.nicovideo.jp/portal/anime");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
		
		res = curl_easy_perform(curl);
		
		if (res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		
		curl_easy_cleanup(curl);
		
	}
	
	start=data.begin();
	end=data.end();
	
	while (regex_search(start, end, result, reg_exp)) {
		(*video_id).push_back( atoi( result.str(1).c_str() ) );
		start = result[0].second;
	}
	
	return;
}

void DownExecute() {
	std::vector<int> video_id;
	
	GetIDs( &video_id );
	
	return;
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

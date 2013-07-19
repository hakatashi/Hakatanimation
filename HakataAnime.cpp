#include <windows.h>
#include <tchar.h>

// �萔
#define WINDOW_WIDTH  (400)		// �E�B���h�E�̕�
#define WINDOW_HEIGHT (300)		// �E�B���h�E�̍���
#define WINDOW_X ((GetSystemMetrics( SM_CXSCREEN ) - WINDOW_WIDTH ) / 2)
#define WINDOW_Y ((GetSystemMetrics( SM_CYSCREEN ) - WINDOW_HEIGHT ) / 2)
#define TIMER_ID     (100)      // �쐬����^�C�}�̎���ID
#define TIMER_ELAPSE (30)     // WM_TIMER�̔����Ԋu
#define WM_TASKTRAY (WM_USER+1)
#define ID_TASKTRAY 0
#define ID_DUMMY   0
#define ID_EXIT    1

// �v���g�^�C�v�錾
HWND Create(HINSTANCE hInst);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);


// �J�n�ʒu
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pCmdLine, int showCmd)
{
	HWND hWnd;
	MSG msg;

	// �E�B���h�E���쐬����
	hWnd = Create( hInst );
	if( hWnd == NULL )
	{
		MessageBox( NULL, _T("�E�B���h�E�̍쐬�Ɏ��s���܂���"), _T("�G���["), MB_OK );
		return 1;
	}

	// �E�B���h�E��\������
	ShowWindow( hWnd, SW_SHOW );
	UpdateWindow( hWnd );

	// �^�C�}���쐬����
	SetTimer( hWnd, TIMER_ID, TIMER_ELAPSE, NULL );

	// ���b�Z�[�W���[�v
	while( 1 )
	{
		BOOL ret = GetMessage( &msg, NULL, 0, 0 );  // ���b�Z�[�W���擾����
		if( ret == 0 || ret == -1 )
		{
			// �A�v���P�[�V�������I�������郁�b�Z�[�W�����Ă�����A
			// ���邢�� GetMessage() �����s������( -1 ���Ԃ��ꂽ�� �j�A���[�v�𔲂���
			break;
		}
		else
		{
			// ���b�Z�[�W����������
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
	}

	// �^�C�}��j������
	KillTimer( hWnd, TIMER_ID );

	return 0;
}

// �E�B���h�E���쐬����
HWND Create(HINSTANCE hInst)
{
	WNDCLASSEX wc;
	HWND g_hWnd;

	// �E�B���h�E�N���X�̏���ݒ�
	wc.cbSize = sizeof(wc);               // �\���̃T�C�Y
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;   // �X�^�C��
	wc.lpfnWndProc = WndProc;             // �E�B���h�E�v���V�[�W��
	wc.cbClsExtra = 0;                    // �g�����P
	wc.cbWndExtra = 0;                    // �g�����Q
	wc.hInstance = hInst;                 // �C���X�^���X�n���h��
	wc.hIcon = LoadIcon(hInst , TEXT("KITTY"));          // �A�C�R��
	wc.hIconSm = wc.hIcon;                // �q�A�C�R��
	wc.hCursor = (HCURSOR)LoadImage(      // �}�E�X�J�[�\��
		NULL, MAKEINTRESOURCE(IDC_ARROW), IMAGE_CURSOR,
		0, 0, LR_DEFAULTSIZE | LR_SHARED
	);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // �E�B���h�E�w�i
	wc.lpszMenuName = NULL;                     // ���j���[��
	wc.lpszClassName = _T("Default Class Name");// �E�B���h�E�N���X��
	
	// �E�B���h�E�N���X��o�^����
	if( RegisterClassEx( &wc ) == 0 ){ return NULL; }

	// �E�B���h�E���쐬����
	g_hWnd = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		wc.lpszClassName,      // �E�B���h�E�N���X��
		_T("Sample Program"),  // �^�C�g���o�[�ɕ\�����镶����
		WS_OVERLAPPEDWINDOW,   // �E�B���h�E�̎��
		0,              // �E�B���h�E��\������ʒu�iX���W�j
		0,              // �E�B���h�E��\������ʒu�iY���W�j
		0,          // �E�B���h�E�̕�
		0,         // �E�B���h�E�̍���
		NULL,                  // �e�E�B���h�E�̃E�B���h�E�n���h��
		NULL,                  // ���j���[�n���h��
		hInst,                 // �C���X�^���X�n���h��
		NULL                   // ���̑��̍쐬�f�[�^
	);
	
	NOTIFYICONDATA nif;
	// �^�X�N�g���C�ɓo�^
	nif.cbSize				= sizeof( NOTIFYICONDATA );
	nif.hIcon				= LoadIcon(hInst , TEXT("KITTY"));
	nif.hWnd				= g_hWnd;
	nif.uCallbackMessage	= WM_TASKTRAY;
	nif.uFlags				= NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nif.uID					= ID_TASKTRAY;
	strcpy( nif.szTip, _T("�����A�j���[�V����") );

    Shell_NotifyIcon( NIM_ADD, &nif );
	
	return g_hWnd;
}

// �E�B���h�E�v���V�[�W��
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	HDC hDC;
	PAINTSTRUCT ps;
	DWORD TopRect, LeftRect;
	static HMENU hMenu, hSubMenu;

	switch( msg )
	{
	case WM_CREATE:         // �E�B���h�E�����ꂽ�Ƃ�
		// ���j���[���\�[�X�����[�h����
		hMenu = LoadMenu( NULL, _T("IDR_POPUP") );
		// �T�u���j���[�̃n���h�����擾����
		hSubMenu = GetSubMenu( hMenu, 0 );

		return 0;
	
	case WM_TIMER:          // �^�C�}���b�Z�[�W
		if( wp != TIMER_ID )
		{
			break;  // ����ID����v���Ȃ��^�C�}���b�Z�[�W��DefWindowProc()�ɔC����
		}

		//InvalidateRect( hWnd, NULL, FALSE );
		return 0;

	case WM_PAINT:          // �����̈悪����Ƃ�
		return 0;

	case WM_TASKTRAY: // �J�[�\���̌��݈ʒu���擾
		POINT point;
		GetCursorPos(&point);

		switch (lp) {
		case WM_RBUTTONDOWN:
			SetForegroundWindow(hWnd);	// ���̏�����Y�ꂸ�ɁI
			
			TrackPopupMenu( hSubMenu, TPM_LEFTALIGN | TPM_BOTTOMALIGN, point.x, point.y, 0, hWnd, NULL );
			//PostMessage(WM_NULL);	// ���̏������Y�ꂸ�ɁI
			break;
		}
		return 0;

	case WM_COMMAND:        // ���ڂ��I�����ꂽ�Ƃ�
		switch( LOWORD( wp ) )
		{
		case ID_EXIT:       // �u�I���v
			SendMessage( hWnd, WM_CLOSE, 0, 0 );
			break;
		}
		return 0;

	case WM_DESTROY:        // �E�B���h�E���j�������Ƃ�
		PostQuitMessage( 0 );
		return 0;
	}

	return DefWindowProc( hWnd, msg, wp, lp );
}
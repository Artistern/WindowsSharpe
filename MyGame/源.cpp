#include "stdafx.h"
#include <Windows.h>
#include<Mmsystem.h>
#include <winuser.h>
#include "resource2.h"
#include <ddraw.h>
#include <cstdio>

#pragma comment(lib,"ddraw.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dxguid.lib")
HINSTANCE hinstance_pop;
int line = 0;
POINT point;

HWND Main_Hwnd;

//directdraw7.0
LPDIRECTDRAW lpdd;


int Game_Init(void *parms=NULL)
{
	PALETTEENTRY palet[256];

	for(int color=0;color<256;color++)
	{
		palet[color].peGreen = rand() % 255;
		palet[color].peBlue = rand() % 255;
		palet[color].peRed = rand() % 255;

		palet[color].peFlags = PC_NOCOLLAPSE;
	}
	palet[0].peGreen = 0;
	palet[0].peBlue = 0;
	palet[0].peRed = 0;
	palet[0].peFlags = PC_NOCOLLAPSE;

	palet[255].peGreen = 255;
	palet[255].peBlue = 255;
	palet[255].peRed = 255;
	palet[255].peFlags = PC_NOCOLLAPSE;
	//��ʼ��
	if(FAILED(DirectDrawCreateEx(NULL,(void**)&lpdd,IID_IDirectDraw7,NULL)))
	{
		return 0;
	}
	if(FAILED(lpdd->SetCooperativeLevel(Main_Hwnd, DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX | DDSCL_ALLOWREBOOT | DDSCL_EXCLUSIVE)))
	{
		return 0;
	}
	if(FAILED(lpdd->SetDisplayMode(800, 600, 8)))
	{
		return 0;
	}
	LPDIRECTDRAWPALETTE lpddpal = NULL;
	if(FAILED(lpdd->CreatePalette(DDPCAPS_8BIT|DDPCAPS_ALLOW256|DDPCAPS_INITIALIZE,palet, &lpddpal,NULL)))
	{
		return 0;
	}
	
	return 1;
}
int Game_Main(void *parms=NULL)
{
	//������
	if (KEYDOWN(VK_ESCAPE))
	{
		PostMessage(Main_Hwnd, WM_DESTROY, 0, 0);
	}
	return 1;
}
int Game_Shutdown(void *parms=NULL)
{
	if(lpdd)
	{
		lpdd->Release();
		lpdd = NULL;

	}
	//����
	return 1;
}
//����
LRESULT CALLBACK WindowPro(HWND hwnd,
							UINT msg,
							WPARAM wparam,
							LPARAM lparam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch(msg)
	{
	case WM_CREATE:
		{
		return 0;
	}break;
	case WM_COMMAND:
		{
			switch (LOWORD(wparam))
			{
			case MENU_FILE_ID_OPEN:
				{
				PlaySound(MAKEINTRESOURCE(IDR_WAVE1), hinstance_pop, SND_ASYNC | SND_RESOURCE);
				}break;
			case MENU_FILE_ID_CLOSE:
				{
				PlaySound(NULL, hinstance_pop, SND_PURGE);
			}break;
			default:break;
			}
		}break;
	case WM_PAINT:
		{
		//GetWindowRect(hwnd, &rect);
		//width = rect.right - rect.left;
		//high = rect.top - rect.bottom;
		/*hdc = BeginPaint(hwnd, &ps);
		SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
		SetBkColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, rand() % 400, rand() % 400, TEXT("GDI TEXT DOME"), sizeof("GDITEXTDOME"));
		EndPaint(hwnd, &ps);*/
		return 0;
	}break;
	case WM_CLOSE:
	{		
		return (DefWindowProc(hwnd, msg, wparam, lparam));	
	}break;
	case WM_DESTROY:
		{
			PostQuitMessage(0);//����һ���˳���Ϣ
		return 0;
	}break;
	default:break;
	}
	return (DefWindowProc(hwnd,msg,wparam,lparam));
}

int WINAPI WinMain(
	HINSTANCE hinstance,
	HINSTANCE hprevinstance,
	LPSTR LpcmdLine,
	int ncmdshow
)
{
	WNDCLASSEX winclass;
	HWND hwnd;
	MSG msg;

	winclass.cbSize = sizeof(WNDCLASSEX);//�����ֶδ�С
	winclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	winclass.lpfnWndProc = WindowPro;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
	winclass.hIconSm = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winclass.lpszMenuName = NULL;//����˵���
	winclass.lpszClassName = TEXT("WINCLASS1");

	hinstance_pop = winclass.hInstance;
	;
	if (!RegisterClassEx(&winclass))
	{
		return 0;
	}
	//�������ڵĲ���
	if (!(hwnd = CreateWindowEx(NULL, 
				winclass.lpszClassName, 
				TEXT("��Ϸ����̨"), 
				WS_POPUP|WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
				0, 0, 
				400, 400, 
				NULL, 
				LoadMenu(hinstance,TEXT("MainMenu")), 
				hinstance, 
				NULL)))
	{
		return 0;
	}
	Main_Hwnd = hwnd;
	srand(GetTickCount());
	Game_Init();
	while (true)
	{
		int startTime = GetTickCount();
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))//�����Ϣ��������û�ж�Ӧ����Ϣ����ֱ��ִ������ĳ���
		{
			if(msg.message==WM_QUIT)//����Ϣ�����в���һ���˳�ָ��
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);//����winProc
		}
		Game_Main();
		while ((GetTickCount()- startTime)<33);
	}
	Game_Shutdown();
	return (msg.wParam);
}
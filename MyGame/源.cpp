#include "stdafx.h"
#include <Windows.h>
#include<Mmsystem.h>
#include <winuser.h>
#include "resource2.h"
#include <cstdio>


#pragma comment(lib,"winmm.lib")
HINSTANCE hinstance_pop;
TCHAR buffer[256]=TEXT("hello");
int line = 0;
static int wm_paint_num = 0;
POINT point;


bool isDraw = false;
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
				//MessageBox(hwnd, TEXT("11"), TEXT("222"), MB_OK);
				PlaySound(MAKEINTRESOURCE(IDR_WAVE1), hinstance_pop, SND_ASYNC | SND_RESOURCE);
				}break;
			case MENU_FILE_ID_CLOSE:
				{
				PlaySound(NULL, hinstance_pop, SND_PURGE);
			}break;
			default:break;
			}
		}break;
	case WM_LBUTTONDOWN:
		{
		isDraw = true;
		}break;
	case WM_LBUTTONUP:
		{
		isDraw = false;
		}break;
	case WM_PAINT:
		{
		hdc = BeginPaint(hwnd, &ps);
		SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
		SetBkColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, rand() % 400, rand() % 400, TEXT("GDI TEXT DOME"), sizeof("GDITEXTDOME"));
		EndPaint(hwnd, &ps);
		return 0;
	}break;
	case WM_CLOSE:
	{
			int result = MessageBox(hwnd, TEXT("�Ƿ�ر�"), TEXT("�İ��"), MB_YESNO | MB_ICONERROR);
			if(result==IDYES)
			{
				return (DefWindowProc(hwnd, msg, wparam, lparam));
			}else
			{
				return 0;
			}
		
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
	HDC hdc;

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
				TEXT("ɵ��"), 
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
	while (true)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))//�����Ϣ��������û�ж�Ӧ����Ϣ����ֱ��ִ������ĳ���
		{
			if(msg.message==WM_QUIT)//����Ϣ�����в���һ���˳�ָ��
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);//����winProc
		}
		if(KEYDOWN(VK_ESCAPE))
		{
			PostMessage(hwnd, WM_DESTROY, 0, 0);
		}else if(isDraw==true)
		{
			GetCursorPos(&point);
			ScreenToClient(hwnd, &point);
			int x = point.x;
			int y = point.y;
			hdc = GetDC(hwnd);
			HPEN blue_pen = CreatePen(PS_SOLID, 5, RGB(0, 0, 255));
			SelectObject(hdc, blue_pen);
			Rectangle(hdc, x + 0.5, y + 0.5, x - 0.5, y - 0.5);
			DeleteObject(blue_pen);
			ReleaseDC(hwnd, hdc);
		}
		else if(KEYDOWN(VK_DELETE))
		{
			winclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		}
	}
	return (msg.wParam);
}
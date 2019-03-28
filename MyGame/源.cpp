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
			int result = MessageBox(hwnd, TEXT("是否关闭"), TEXT("文版框"), MB_YESNO | MB_ICONERROR);
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
			PostQuitMessage(0);//产生一个退出消息
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

	winclass.cbSize = sizeof(WNDCLASSEX);//描述字段大小
	winclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
	winclass.lpfnWndProc = WindowPro;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
	winclass.hIconSm = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_ICON1));
	winclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	winclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winclass.lpszMenuName = NULL;//定义菜单栏
	winclass.lpszClassName = TEXT("WINCLASS1");

	hinstance_pop = winclass.hInstance;
	;
	if (!RegisterClassEx(&winclass))
	{
		return 0;
	}
	//创建窗口的操作
	if (!(hwnd = CreateWindowEx(NULL, 
				winclass.lpszClassName, 
				TEXT("傻子"), 
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
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))//如果消息队列里面没有对应的消息，就直接执行下面的程序
		{
			if(msg.message==WM_QUIT)//当消息队列中产生一个退出指令
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);//调用winProc
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
#include "stdafx.h"
#include <Windows.h>


LRESULT CALLBACK WindowRroc(HWND hwnd,
							UINT msg,
							WPARAM wparam,
							LPARAM lparam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (msg)
	{
		case WM_CREATE://Init操作
			{
				return 0;
			}break;
		case WM_PAINT://画刷程序
			{
				hdc=BeginPaint(hwnd,&ps);
				EndPaint(hwnd,&ps);
				return 0;
			}break;
		case WM_DESTROY://退出
			{
				PostQuitMessage(0);//WM_QUIT位于消息队列
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
	winclass.cbSize = sizeof(WNDCLASSEX);//描述字段大小
	winclass.style = CS_HREDRAW | CS_VREDRAW|CS_OWNDC|CS_DBLCLKS;
	winclass.lpfnWndProc = WindowRroc;
	winclass.cbClsExtra = 0;
	winclass.cbWndExtra = 0;
	winclass.hInstance = hinstance;
	winclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	winclass.hCursor = LoadCursor(NULL, IDC_WAIT);
	winclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	winclass.lpszMenuName = NULL;
	winclass.lpszClassName = TEXT("WINCLASS1");
	winclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&winclass);

	HWND hwnd;
	MSG msg;

	if(!(hwnd=CreateWindowEx(NULL,
							winclass.lpszClassName,
							TEXT("My First Window"),
							WS_OVERLAPPED|WS_VISIBLE,
							0,0,
							400,400,
							NULL,
							NULL,
							hinstance,
							NULL)))
	{
		return 0;
	}

	UpdateWindow(hwnd);

	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);

		DispatchMessage(&msg);
	}

	return (msg.wParam); 
}
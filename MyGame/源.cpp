//**************头文件******************
#include "stdafx.h"
#include <Windows.h>
#include "resource2.h"
#include <ddraw.h>
#include <iostream>
//**************lib***********************
#pragma comment(lib,"ddraw.lib")
#pragma comment(lib,"dxguid.lib")
//*******************定义基础属性**********************
#define SCREEN_WIDTH    1920  // size of screen
#define SCREEN_HEIGHT   1080
#define SCREEN_BPP      32    //这里我们只能使用32位色彩
#define MAX_COLORS      256  // maximum colors
#define WINDOW_CLASS_NAME "WINCLASS1"
#define DD_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }
#define _RGB32BIT8888(a,r,g,b)	((b)+((g)<<8)+((r)<<16)+((a)<<24))
//********************定义句柄*************************
HINSTANCE		hinstance_pop=NULL;
HWND			Main_Hwnd=NULL;
//********************定义Direct绘图*******************
LPDIRECTDRAW7			lpdd;
DDSURFACEDESC2			ddsd;
LPDIRECTDRAWSURFACE7	lpddsprimary=NULL;
LPDIRECTDRAWPALETTE		lpddpal = NULL;   // a pointer to the created dd palette
PALETTEENTRY			palette[256];          // color palette
DDPIXELFORMAT			ddpixel;//像素格式
//********************定义辅助变量*********************
int line = 0;
//*********************32位色彩************************
inline void Plot_Pixel_32(int x,int y,
							int alphe,int red,int green,int blue,
							LONG *video_buffer,int lpitch32)
{
	LONG pixel = _RGB32BIT8888(alphe, red, green, blue);

	video_buffer[x + y * (lpitch32<<2)] = pixel;
}
//********************初始化进程***********************
int Game_Init(void *parms = NULL)
{
	
	//创建一个DirectDraw对象，返回到lpdd
	if (FAILED(DirectDrawCreateEx(NULL, (void **)&lpdd, IID_IDirectDraw7, NULL)))
	{
		return 0;
	}
	
	//设置与windows协作的模式，目前为全屏模式
	if (FAILED(lpdd->SetCooperativeLevel(Main_Hwnd,
		DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX |
		DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT)))
	{
		return(0);
	} 

	;
	//设置视频模式,WIDTH*HEIGHT*32(已经修正)
	if (FAILED(lpdd->SetDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, 0, 0)))
	{
		return(0);
	}
	//清空ddsd并且设置大小
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ddsd.dwFlags = DDSD_CAPS;
	//这里表示创建的是主表面
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	//创建主显示表面
	if(FAILED(lpdd->CreateSurface(&ddsd,&lpddsprimary,NULL)))
	{
		//isFail = false;
		return 0;
	}
	for (int color = 1; color < 255; color++)
	{
		// fill with random RGB values
		palette[color].peRed = rand() % 256;
		palette[color].peGreen = rand() % 256;
		palette[color].peBlue = rand() % 256;

		// set flags field to PC_NOCOLLAPSE
		palette[color].peFlags = PC_NOCOLLAPSE;
	} // end for color

// now fill in entry 0 and 255 with black and white
	palette[0].peRed = 0;
	palette[0].peGreen = 0;
	palette[0].peBlue = 0;
	palette[0].peFlags = PC_NOCOLLAPSE;

	palette[255].peRed = 255;
	palette[255].peGreen = 255;
	palette[255].peBlue = 255;
	palette[255].peFlags = PC_NOCOLLAPSE;
	//创建调色板
	if(FAILED(lpdd->CreatePalette(DDPCAPS_8BIT|
								DDPCAPS_ALLOW256|
								DDPCAPS_INITIALIZE,
								palette,&lpddpal,NULL)))
	{
		return 0;
	}
	//指定调色板
	if(FAILED(lpddsprimary->SetPalette(lpddpal)))
	{
		return 0;
	}
	//获取像素格式
	
	return 1;
}
//*******************主进程****************************
int Game_Main(void *parms=NULL)
{
	
	UCHAR primary_buffer = NULL;
	if (KEYDOWN(VK_ESCAPE))
	{
		PostMessage(Main_Hwnd, WM_DESTROY, 0, 0);
	}
	memset((void*)primary_buffer, 0, SCREEN_WIDTH*SCREEN_HEIGHT);
	return(1);
}
//*******************游戏结束的操作********************
int Game_Shutdown(void *parms=NULL)
{
	if (lpddpal)
	{
		lpddpal->Release();
		lpddpal = NULL;
	}
	if (lpddsprimary)
	{
		lpddsprimary->Release();
		lpddsprimary = NULL;
	}
	if (lpdd)
	{
		lpdd->Release();
		lpdd = NULL;
	} 
	//结束
	return 1;
}
//进程
LRESULT CALLBACK WindowPro(HWND hwnd,
							UINT msg,
							WPARAM wparam,
							LPARAM lparam)
{
	PAINTSTRUCT		ps;		// used in WM_PAINT
	HDC				hdc;	// handle to a device context
	switch(msg)
	{
	case WM_CREATE:
		{
		return 0;
	}break;
	case WM_PAINT:
		{
		hdc = BeginPaint(hwnd, &ps);

		EndPaint(hwnd, &ps);
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
				TEXT(WINDOW_CLASS_NAME),
				TEXT("游戏控制台"), 
				WS_POPUP | WS_VISIBLE,
				//WS_POPUP,
				0, 0, 
				SCREEN_WIDTH, SCREEN_HEIGHT, 
				NULL, 
				NULL,
				hinstance, 
				NULL)))
	{
		return 0;
	}
	Main_Hwnd = hwnd;
	Game_Init();
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
		Game_Main();
	}
	Game_Shutdown();
	return (msg.wParam);
}
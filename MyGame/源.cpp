//**************ͷ�ļ�******************
#include "stdafx.h"
#include <Windows.h>
#include "resource2.h"
#include <ddraw.h>
#include <iostream>
//**************lib***********************
#pragma comment(lib,"ddraw.lib")
#pragma comment(lib,"dxguid.lib")
//*******************�����������**********************
#define SCREEN_WIDTH    1920  // size of screen
#define SCREEN_HEIGHT   1080
#define SCREEN_BPP      32    //��������ֻ��ʹ��32λɫ��
#define MAX_COLORS      256  // maximum colors
#define WINDOW_CLASS_NAME "WINCLASS1"
#define DD_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }
#define _RGB32BIT8888(a,r,g,b)	((b)+((g)<<8)+((r)<<16)+((a)<<24))
//********************������*************************
HINSTANCE		hinstance_pop=NULL;
HWND			Main_Hwnd=NULL;
//********************����Direct��ͼ*******************
LPDIRECTDRAW7			lpdd;
DDSURFACEDESC2			ddsd;
LPDIRECTDRAWSURFACE7	lpddsprimary=NULL;
LPDIRECTDRAWPALETTE		lpddpal = NULL;   // a pointer to the created dd palette
PALETTEENTRY			palette[256];          // color palette
DDPIXELFORMAT			ddpixel;//���ظ�ʽ
//********************���帨������*********************
int line = 0;
//*********************32λɫ��************************
inline void Plot_Pixel_32(int x,int y,
							int alphe,int red,int green,int blue,
							LONG *video_buffer,int lpitch32)
{
	LONG pixel = _RGB32BIT8888(alphe, red, green, blue);

	video_buffer[x + y * (lpitch32<<2)] = pixel;
}
//********************��ʼ������***********************
int Game_Init(void *parms = NULL)
{
	
	//����һ��DirectDraw���󣬷��ص�lpdd
	if (FAILED(DirectDrawCreateEx(NULL, (void **)&lpdd, IID_IDirectDraw7, NULL)))
	{
		return 0;
	}
	
	//������windowsЭ����ģʽ��ĿǰΪȫ��ģʽ
	if (FAILED(lpdd->SetCooperativeLevel(Main_Hwnd,
		DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX |
		DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT)))
	{
		return(0);
	} 

	;
	//������Ƶģʽ,WIDTH*HEIGHT*32(�Ѿ�����)
	if (FAILED(lpdd->SetDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, 0, 0)))
	{
		return(0);
	}
	//���ddsd�������ô�С
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ddsd.dwFlags = DDSD_CAPS;
	//�����ʾ��������������
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	//��������ʾ����
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
	//������ɫ��
	if(FAILED(lpdd->CreatePalette(DDPCAPS_8BIT|
								DDPCAPS_ALLOW256|
								DDPCAPS_INITIALIZE,
								palette,&lpddpal,NULL)))
	{
		return 0;
	}
	//ָ����ɫ��
	if(FAILED(lpddsprimary->SetPalette(lpddpal)))
	{
		return 0;
	}
	//��ȡ���ظ�ʽ
	
	return 1;
}
//*******************������****************************
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
//*******************��Ϸ�����Ĳ���********************
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
	//����
	return 1;
}
//����
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
				TEXT(WINDOW_CLASS_NAME),
				TEXT("��Ϸ����̨"), 
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
	}
	Game_Shutdown();
	return (msg.wParam);
}
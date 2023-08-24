#include "map.h"

HBITMAP hBitmap;


Draw::Draw(HBITMAP hBit, HWND hw)
{
	hBitmap = hBit;
	hwnd = hw;
}
void Draw::DrawBackground()
{
	PAINTSTRUCT ps;
	hdc = BeginPaint(hwnd, &ps);
	//�����ڴ��豸������
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

	//��ȡͼ��
	if (GetImage())
	{
		StretchBlt(hdc, 0, 0, WindowWidth, WindowHeight, hdcMem, 0, 0, ImageWidth, ImageHeight, SRCCOPY);
	}
	//����ͼ��
	//BitBlt(hdc, 0, 0, 1284, 588, hdcMem, 0, 0, SRCCOPY);
	//������Դ
	SelectObject(hdcMem, hOldBitmap);
	DeleteDC(hdcMem);
	EndPaint(hwnd, &ps);
}
bool Draw::GetImage() 
{
	BITMAP bmp;
	GetObject(hBitmap, sizeof(BITMAP), &bmp);

	ImageWidth = bmp.bmWidth;
	ImageHeight = bmp.bmHeight;
	if (ImageWidth == 0 || ImageHeight == 0)
		return false;
	return true;
}

Draw::~Draw()
{
}

long long WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		case WM_PAINT:
		{
			Draw draw(hBitmap, hwnd);
			draw.DrawBackground();
			draw.~Draw();
			break;
		}
		case WM_COMMAND:
		{
			if (wp == 1)
				MessageBox(hwnd, L"��ť�������", L"��ť����¼�", MB_ICONINFORMATION);
			break;
			//TODO
		}
		case WM_CLOSE:
		case WM_DESTROY:
		case WM_QUIT:
			exit(0); 
		break;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

HWND set_window(HINSTANCE hInst)
{
	WNDCLASSEXW wc;
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = L"WindowClassName";
	wc.hInstance = hInst;
	wc.lpfnWndProc = WndProc;
	RegisterClassExW(&wc);
	HWND hwnd = CreateWindowW(L"WindowClassName", L"���", WS_VISIBLE | WS_OVERLAPPEDWINDOW, 100, 100, WindowWidth, WindowHeight, NULL, NULL, hInst, NULL);
	if (hwnd == NULL) return (HWND) - 1;
	hBitmap = (HBITMAP)LoadImage(NULL, L"image/mypaper_title.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	MSG msg;
	while (true) if (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
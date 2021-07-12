#include <windows.h>
#include <gdiplus.h>

#define getrandom( min, max ) (SHORT)((rand() % (int)(((max) + 1) - \
                               (min))) + (min))

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);
void draw(HDC hdc);

int WINAPI WinMain(HINSTANCE currentInstance, HINSTANCE previousInstance, PSTR cmdLine, INT cmdCount) {
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
	
	PCWSTR CLASS_NAME = L"Win32WindowClass";
	WNDCLASS wc{};
	wc.hInstance = currentInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProcessMessages;
	RegisterClass(&wc);
	
	CreateWindow(CLASS_NAME, L"MultithreadDemoAgain",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,            
		CW_USEDEFAULT, CW_USEDEFAULT,                
		800, 600,                                    
		nullptr, nullptr, nullptr, nullptr);
	
	MSG msg{};
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return 0;
}

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (msg) {
		case WM_PAINT:
			hdc = BeginPaint(hwnd, &ps);
			MessageBox(hwnd, L"Press End to stop the drawing", L"Info", NULL);
			draw(hdc);
			EndPaint(hwnd, &ps);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			return DefWindowProc(hwnd, msg, param, lparam);
	}
}

void draw(HDC hdc) {
	Gdiplus::Graphics gf(hdc);
	INT dx, dy; //delta values for speed
	INT x, y; // coordinates
	dx = 10; 
	dy = 10;
	POINT p;
	if (GetCursorPos(&p)) {
		srand((p.x ^ p.y) ^ GetTickCount64());
	}
	x = rand() % 800;
	y = rand() % 600;
	SHORT KeyState = GetAsyncKeyState(VK_END);
	for (;;) {
		KeyState = GetAsyncKeyState(VK_END);
		if ((1 << 15) & KeyState) {
			break;
		}
		Gdiplus::SolidBrush brush(Gdiplus::Color(255, rand() % 255, rand() % 255, rand() % 255));
		gf.FillRectangle(&brush, x, y, 20, 20);
		if (x < 0 || x >= 800) { //did we hit the wall
			dx = -dx;
			Beep(200, 30);
		}
		if (y < 0 || y >= 600) { //did we hit the wall
			dy = -dy;
			Beep(200, 30);
		}
		x += dx;
		y += dy;
		Sleep(1);
	}
}

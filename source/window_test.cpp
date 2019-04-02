#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>

const char g_szClassName[] = "myWindowClass";

int show_mouse_pointer(bool show)
{
	int show_counter;
	//mouse is hidden if the counter is
	//less than 0 and shown if the couter
	//is 0 or greater

	if (show)
	{
		do
		{
			show_counter = ShowCursor(TRUE);
		} while (show_counter < 0);
	}
	else
	{
		do
		{
			show_counter = ShowCursor(FALSE);
		} while (show_counter >= 0);
	}

	return show_counter;
}

void draw_ellipse(HDC hdc, CONST RECT *client_rect)
{
	static int offset_loc = 0;

	offset_loc += 1;
	
	Ellipse(hdc, offset_loc, offset_loc, offset_loc+50, offset_loc+50);
}

void draw_ellipse(HDC hdc, int x_loc, int y_loc, int diameter)
{
	//creates and selects a solid black brush and saves old bursh
	HBRUSH black_solid_brush = CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH old_brush = static_cast<HBRUSH>(SelectObject(hdc, black_solid_brush));

	//draws a filled ellipse
	Ellipse(hdc, x_loc, y_loc, x_loc + diameter, y_loc + diameter);

	//selects the old brush back and deletes the black solid brush
	SelectObject(hdc, old_brush);
	DeleteObject(black_solid_brush);
}

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//Var declarations to avoid weird interactions in the cases
	PAINTSTRUCT ps; //struct for painting
	HDC hdc; //stores the device context of this window handle
	RECT cliRect;

	switch (msg)
	{
	case WM_ACTIVATE:
		if (wParam & WA_CLICKACTIVE)
			MessageBeep(MB_ICONASTERISK);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		UpdateWindow(hwnd);

	case WM_PAINT:
		{
		GetClientRect(hwnd, &cliRect);
		//Causes the whole client area to be updated and redraws the background
		InvalidateRect(hwnd, NULL, TRUE);

		hdc = BeginPaint(hwnd, &ps);
		
		if (hdc)
		{
			POINT curPos;
			if (GetCursorPos(&curPos))
			{
				if (ScreenToClient(hwnd, &curPos))
				{
					//draw_ellipse(hdc, &cliRect);
					draw_ellipse(hdc, curPos.x-10, curPos.y-10, 20);
				}
			}

			/*char text_buffer[32] = { 0 };
			sprintf_s(text_buffer, sizeof(text_buffer), "X: %d, Y: %d\n", curPos.x, curPos.y);
			OutputDebugString(TEXT(text_buffer));*/
		}
		else
		{
			//MessageBox(hwnd, TEXT("Not a valid DC"), TEXT("Warning"), MB_OK | MB_ICONWARNING);
		}

		EndPaint(hwnd, &ps);
		//draw_ellipse(hwnd);
		}
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	//show_mouse_pointer(false);

	//Step 1: Registering the Window Class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Step 2: Creating the Window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		"The title of my window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	// Step 3: The Message Loop
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

#include <windows.h>
#include <windowsx.h>
#include "Data.h"
#include "Scene2D.h"

LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);						// �������� ������� ���������
Model2D initModel();
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)		// �������� ���������
{
	// ������ ������������ ����� �������� ��������� - �������� ����: ������� ����������� ������� ����� wc, ����� �������� ���� hWnd
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;						// ��� ������� ���������, ������������ �� ������ �������
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;						// ������������� ����������, ���������� ����� ������ ���������� ��� ������� ������
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = 0;							// ���� � ������� ������ �����������
	wc.lpszClassName = (LPCSTR)"MainWindowClass";	// ��� �������� ������, ������������ ��� �������� ����������� ����
	RegisterClass(&wc);								// ����������� ������ wc

	HWND hWnd = CreateWindow(						// hWnd - ����������, ���������������� ����; ������� �������� ���� ��������� ���������� hWnd ��������� ���������
		(LPCSTR)"MainWindowClass",					// ��� �������� ������
		(LPCSTR)"Plot2D Viewer",					// ��������� ����
		WS_OVERLAPPEDWINDOW,						// ����� ����
		200,200,400,400,							// ���������� �� ������ ������ �������� ���� ����, ��� ������ � ������
		nullptr,nullptr,hInstance,nullptr);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	// ������ ������������ ����� �������� ��������� - �������� ���� ��������� ��������� ���������, ������� ������� ��������� � ��������� �� ��������������� �����
	MSG msg;
	while(GetMessage(&msg,nullptr,0,0))				// ������� GetMessage �������� �� ������� ��������� � ������� ��� � ��������� msg
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);						// ������� DispatchMessage ��������� ������� � ������������� ������ ������� ���������
	}

	return 0;
}

// � �������� ������ ����������� ������ ���� ���������� ���������� - �������� ������ ������ Scene2D
// ��� ���������� �������� �������������� ����������� ��������� � �������, ������������� � ���� ������
Model2D m = initModel();
Scene2D scene(L,R,B,T,m);

LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)		// ������� ��������� ��������� � ������������ ��� ���������, ������������ ����
{
	switch (msg)
	{
	case WM_PAINT:
	{
		HDC dc = GetDC(hWnd);
		scene.Clear(dc);				// ����� �������������� � ������ Camera2D ������, ����������� �� ������� ������� ������� ���� hWnd
		scene.Plot(dc, Circle);		// ����� �������������� � ������ Scene2D ������, ����������� �� ��������� ������� ���������
		ReleaseDC(hWnd, dc);
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	case WM_SIZE:
	{
		HDC dc = GetDC(hWnd);
		scene.SetResolution(dc);
		ReleaseDC(hWnd, dc);
		InvalidateRect(hWnd, nullptr, false);
		return 0;
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_LBUTTONDOWN:
	{
		POINT P;
		P.x = GET_X_LPARAM(lParam);
		P.y = GET_Y_LPARAM(lParam);
		ScreenToClient(hWnd, &P);

		scene.StartDragging(GetDC(hWnd), P.x, P.y);
		return 0;
	}

	case WM_MOUSEMOVE:
	{
		if (scene.IsDragging())
		{
			POINT P;
			P.x = GET_X_LPARAM(lParam);
			P.y = GET_Y_LPARAM(lParam);
			ScreenToClient(hWnd, &P);

			scene.Drag(P.x, P.y);
			InvalidateRect(hWnd, nullptr, false);
		}
		return 0;
	}
	case WM_LBUTTONUP:
	{
		scene.StopDragging();
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		scene.Scale((short)HIWORD(wParam) / 120);
		InvalidateRect(hWnd, nullptr, false);
		return 0;
	}

	case WM_KEYDOWN:
	{
		switch (wParam)
		{

		case VK_LEFT:
		{
			scene.m.apply(Translation(-0.5, 0));
			break;
		}
		case VK_RIGHT:
		{
			scene.m.apply(Translation(0.5, 0));
			break;
		}
		case VK_UP:
		{
			scene.m.apply(Translation(0, 0.5));
			break;
		}
		case VK_DOWN:
		{
			scene.m.apply(Translation(0, -0.5));
			break;
		}
		case VK_ADD:
		{
			if (GetKeyState(VK_CONTROL) & 0x8000) {
				scene.m.apply(Scaling(1.5, 1.5));
			}
		}
		case VK_SUBTRACT:
		{
			if (GetKeyState(VK_CONTROL) & 0x8000) {
				scene.m.apply(Scaling(0.9, 0.9));
			}
		}

		}
		InvalidateRect(hWnd, nullptr, false);
		return 0;
	}
	default:
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	
	return 0;
	}
}
Model2D initModel() {

	double v[15] = {
		1, 1, 5, 5, 6,
		4, 1, 1, 4, 11,
		1, 1, 1, 1, 2
	};
	Matrix<> V(3, 5, v);

	int e[25] = {
		0, 1, 0, 1, 1,
		1, 0, 1, 0, 0,
		0, 1, 0, 1, 0,
		1, 0, 1, 0, 1,
		1, 0, 0, 1, 0
	};
	Matrix<int> E(5, 5, e);

	Model2D m(V, E);
	return m;
}

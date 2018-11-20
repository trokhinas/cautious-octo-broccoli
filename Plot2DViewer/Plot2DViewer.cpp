#include <windows.h>
#include <windowsx.h>
#include "Data.h"
#include "Scene2D.h"

LRESULT _stdcall WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);						// прототип оконной процедуры
Model2D initModel();
int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)		// основная процедура
{
	// Первая составляющая часть основной процедуры - создание окна: сначала описывается оконный класс wc, затем создаётся окно hWnd
	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;						// имя оконной процедуры, закрепленной за данным классом
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;						// идентификатор приложения, содержащий адрес начала выделенной ему области памяти
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(6);
	wc.lpszMenuName = 0;							// меню в оконном классе отсутствует
	wc.lpszClassName = (LPCSTR)"MainWindowClass";	// имя оконного класса, используемое при создании экземпляров окна
	RegisterClass(&wc);								// регистрация класса wc

	HWND hWnd = CreateWindow(						// hWnd - дескриптор, идентифицирующий окно; функция создания окна заполняет дескриптор hWnd ненулевым значением
		(LPCSTR)"MainWindowClass",					// имя оконного класса
		(LPCSTR)"Plot2D Viewer",					// заголовок окна
		WS_OVERLAPPEDWINDOW,						// стиль окна
		200,200,400,400,							// координаты на экране левого верхнего угла окна, его ширина и высота
		nullptr,nullptr,hInstance,nullptr);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	// Вторая составляющая часть основной процедуры - основной цикл обработки системных сообщений, который ожидает сообщения и рассылает их соответствующим окнам
	MSG msg;
	while(GetMessage(&msg,nullptr,0,0))				// функция GetMessage выбирает из очереди сообщение и заносит его в структуру msg
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);						// функция DispatchMessage оповещает систему о необходимости вызова оконной процедуры
	}

	return 0;
}

// В основном модуле объявляется только одна глобальная переменная - создаётся объект класса Scene2D
// Все дальнейшие действия осуществляются посредством обращения к методам, реализованным в этом классе
Model2D m = initModel();
Scene2D scene(L,R,B,T,m);

LRESULT _stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)		// оконная процедура принимает и обрабатывает все сообщения, отправленные окну
{
	switch (msg)
	{
	case WM_PAINT:
	{
		HDC dc = GetDC(hWnd);
		scene.Clear(dc);				// Вызов реализованного в классе Camera2D метода, отвечающего за очистку рабочей области окна hWnd
		scene.Plot(dc, Circle);		// Вызов реализованного в классе Scene2D метода, отвечающего за отрисовку графика синусоиды
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

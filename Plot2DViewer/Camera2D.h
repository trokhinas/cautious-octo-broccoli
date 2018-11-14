#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <windows.h>

class Camera2D
{
protected:
	double L, R, B, T;					// ћировые координаты границ рабочей области окна
	//следовательно L <= x <= R мировые координаты по х
	//B <= y <= T мировые координаты по у
	int W, H;							// –азрешение рабочей области окна
	int WorldToScreenX(double X)		// ѕереход от мировых координат к экранным (дл€ абсциссы)
	{
		return  W * (X - L) / (R - L);
	}
	int WorldToScreenY(double Y)		// ѕереход от мировых координат к экранным (дл€ ординаты)
	{
		return (T - Y) * H / (T - B); 
	}
	double ScreenToWorldX(int X)		// ѕереход от экранных координат к мировым (дл€ абсциссы)
	{
		return L + X * (R - L) / W;
	}
	double ScreenToWorldY(int Y)		// ѕереход от экранных координат к мировым (дл€ ординаты)
	{
		return T - Y * (T - B) / H;
	}
private:
	bool isDragging;
	double scrollSpeed;
	double LastX, LastY;
	double degInterval, degSize;
	double posX, posY;					// ѕозици€ графического курсора в мировых координатах (дл€ функций MoveTo и LineTo)
	void drawVerticalLine(HDC dc, double x) {
		MoveTo(x, T);
		LineToIN(dc, x, B);
	}
	void drawHorizontalLine(HDC dc, double y) {
		MoveTo(L, y);
		LineToIN(dc, R, y);
	}
	void moveX(double offsetX) {
		/*if (offsetX > 0) {
			L += 0.1 * scrollSpeed;
			R += 0.1 * scrollSpeed;
		}
		else
		{
			if (offsetX < 0) {
				L -= 0.1 * scrollSpeed;
				R -= 0.1 * scrollSpeed;
			}
		}*/ 
		// альтернативный, но почему то не работающий вариант
		L += offsetX * scrollSpeed;
		R += offsetX * scrollSpeed;
	}
	void moveY(double offsetY) {
		/*if (offsetY > 0) {
			T += 0.1 * scrollSpeed;
			B += 0.1 * scrollSpeed;
		}
		else
		{
			if (offsetY < 0) {
				T -= 0.1 * scrollSpeed;
				B -= 0.1 * scrollSpeed;
			}
		}*/
		// альтернативный, но почему то не работающий вариант
		T += offsetY * scrollSpeed;
		B += offsetY * scrollSpeed;
	}
	void setLast(double x, double y) {
		LastX = x;
		LastY = y;
	}

public:
	Camera2D(double L, double R, double B, double T) : L(L), R(R), B(B), T(T), degInterval(1), degSize(0.05), isDragging(false)
	{
		LastX = 0; LastX = 0;
		LastY = 0; LastY = 0;
		scrollSpeed = 1;
	}
	void Clear(HDC dc)
	{
		/*RECT r;
		GetClientRect(WindowFromDC(dc), &r);*/
		Rectangle(dc, 0, 0, W + 1, H + 1);
	}
	void SetResolution(HDC dc)
	{
		RECT r;
		GetClientRect(WindowFromDC(dc), &r);
		H = r.bottom - 1;
		W = r.right - 1;
	}
	void MoveTo(double X, double Y)
	{
		// ѕеремещение графического курсора (posX, posY)
		// ќбратите внимание, что мы действуем в мировых координатах
		posX = X;
		posY = Y;
	}
	void LineToIN(HDC dc, double X, double Y)
	{
		// ќтрисовка линии из текущей позиции графического курсора в точку (X, Y) и его перемещение в эту точку
		// ќбратите внимание, что мы действуем в мировых координатах
		// ѕри отрисовке линии могут быть использованы WinApi функции
		// ::MoveToEx(dc, Xs, Ys, nullptr) и ::LineTo(dc, Xs, Ys)

		MoveToEx(dc, WorldToScreenX(posX), WorldToScreenY(posY), nullptr);//сначала перемещаемс€ в последнюю "запомненную точку" на самом деле
		LineTo(dc, WorldToScreenX(X), WorldToScreenY(Y));//рисуем линию
		MoveTo(X, Y);//запоминаем эту точку

	}
	void Axes(HDC dc)
	{
		// ќтрисовка координатных осей
		drawHorizontalLine(dc, 0);
		drawVerticalLine(dc, 0);

		// ќтрисовка засечек на каждом сантиметре
		for (int i = L; i < R; i+= degInterval) {
			MoveTo(i, 0 - degSize);
			LineToIN(dc, i, 0 + degSize);//подумать над размером засечек:какой он должен быть и относительно чего измер€тьс€, допустим.
		}
		for (int i = T; i > B; i-= degInterval) {
			MoveTo(0 - degSize, i);
			LineToIN(dc, 0 + degSize, i);
		}
		
	}

	void StartDragging(HDC dc, int X, int Y) {
		isDragging = true;

		//запоминаем точку относительно которой производитс€ смещение
		setLast(ScreenToWorldX(X), ScreenToWorldY(Y));
	}
	void Drag(int X, int Y) {
		double X_s = ScreenToWorldX(X);
		double Y_s = ScreenToWorldY(Y);
		//высчитываем смещение
		double offsetX = LastX - X_s;
		double offsetY = LastY - Y_s;
		//сдвигаем камеру по ос€м
		moveX(offsetX);
		moveY(offsetY);
		//теперь смещение происходить относительно новой точки
		setLast(X_s, Y_s);
	}
	bool IsDragging() {
		return isDragging;
	}
	void StopDragging() {
		isDragging = false;
	}
	void Scale(double size) {
		double scaleKoef = 0.05;
		if (size > 0)
		{
			L += scaleKoef; B += scaleKoef;
			R -= scaleKoef; T -= scaleKoef;
		}
		else
		{
			L -= scaleKoef; B -= scaleKoef;
			R += scaleKoef; T += scaleKoef;
		}
	}
};

#endif CAMERA_2D_H*/

#ifndef CAMERA_2D_H
#define CAMERA_2D_H

#include <windows.h>

class Camera2D
{
protected:
	double L, R, B, T;					// ������� ���������� ������ ������� ������� ����
	//������������� L <= x <= R ������� ���������� �� �
	//B <= y <= T ������� ���������� �� �
	int W, H;							// ���������� ������� ������� ����
	int WorldToScreenX(double X)		// ������� �� ������� ��������� � �������� (��� ��������)
	{
		return  W * (X - L) / (R - L);
	}
	int WorldToScreenY(double Y)		// ������� �� ������� ��������� � �������� (��� ��������)
	{
		return (T - Y) * H / (T - B); 
	}
	double ScreenToWorldX(int X)		// ������� �� �������� ��������� � ������� (��� ��������)
	{
		return L + X * (R - L) / W;
	}
	double ScreenToWorldY(int Y)		// ������� �� �������� ��������� � ������� (��� ��������)
	{
		return T - Y * (T - B) / H;
	}
private:
	bool isDragging;
	double scrollSpeed;
	double LastX, LastY;
	double degInterval, degSize;
	double posX, posY;					// ������� ������������ ������� � ������� ����������� (��� ������� MoveTo � LineTo)
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
		// ��������������, �� ������ �� �� ���������� �������
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
		// ��������������, �� ������ �� �� ���������� �������
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
		// ����������� ������������ ������� (posX, posY)
		// �������� ��������, ��� �� ��������� � ������� �����������
		posX = X;
		posY = Y;
	}
	void LineToIN(HDC dc, double X, double Y)
	{
		// ��������� ����� �� ������� ������� ������������ ������� � ����� (X, Y) � ��� ����������� � ��� �����
		// �������� ��������, ��� �� ��������� � ������� �����������
		// ��� ��������� ����� ����� ���� ������������ WinApi �������
		// ::MoveToEx(dc, Xs, Ys, nullptr) � ::LineTo(dc, Xs, Ys)

		MoveToEx(dc, WorldToScreenX(posX), WorldToScreenY(posY), nullptr);//������� ������������ � ��������� "����������� �����" �� ����� ����
		LineTo(dc, WorldToScreenX(X), WorldToScreenY(Y));//������ �����
		MoveTo(X, Y);//���������� ��� �����

	}
	void Axes(HDC dc)
	{
		// ��������� ������������ ����
		drawHorizontalLine(dc, 0);
		drawVerticalLine(dc, 0);

		// ��������� ������� �� ������ ����������
		for (int i = L; i < R; i+= degInterval) {
			MoveTo(i, 0 - degSize);
			LineToIN(dc, i, 0 + degSize);//�������� ��� �������� �������:����� �� ������ ���� � ������������ ���� ����������, ��������.
		}
		for (int i = T; i > B; i-= degInterval) {
			MoveTo(0 - degSize, i);
			LineToIN(dc, 0 + degSize, i);
		}
		
	}

	void StartDragging(HDC dc, int X, int Y) {
		isDragging = true;

		//���������� ����� ������������ ������� ������������ ��������
		setLast(ScreenToWorldX(X), ScreenToWorldY(Y));
	}
	void Drag(int X, int Y) {
		double X_s = ScreenToWorldX(X);
		double Y_s = ScreenToWorldY(Y);
		//����������� ��������
		double offsetX = LastX - X_s;
		double offsetY = LastY - Y_s;
		//�������� ������ �� ����
		moveX(offsetX);
		moveY(offsetY);
		//������ �������� ����������� ������������ ����� �����
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

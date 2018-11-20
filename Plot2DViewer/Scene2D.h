#ifndef SCENE_2D_H
#define SCENE_2D_H

#include "Camera2D.h"
#include "Model2D.h"
#include "Matrix.h"
#include "AffineTransform.h"

class Scene2D : public Camera2D
{
private:
	typedef double (*Func)(double);
	
public:
	Model2D m;
	Scene2D(double L, double R, double B, double T, Model2D model) : Camera2D(L, R, B, T)
	{
		m = model;
	}
	void Plot(HDC dc, Func f, bool axes=true)
	{
		if (axes)
			Axes(dc);

		// Построение графика функции f
		// Здесь передаваемая в качестве параметра функция f вызывается как обычная функция:
		// f(x);

		/*MoveTo(L, f(L));
		for (int i = 1; i < W; i ++) {
			double x = ScreenToWorldX(i);
			double y = f(x);
			LineToIN(dc, x, y);
		}
		LineToIN(dc, R, f(R));

		MoveTo(0, 0);
		for (double i = 0.0; i <= 2 * M_PI; i+= 0.001) {
			double phi = i;
			double r = Circle(phi);

			double x = PolarToDecX(r, phi);
			double y = PolarToDecY(r, phi);

			LineToIN(dc, x, y);
		}*/

		Render(m, dc);
	}

	double PolarToDecY(double r, double phi) {
		return r * sin(phi);
	}
	double PolarToDecX(double r, double phi) {
		return r * cos(phi);
	}
	
	void Render(Model2D m, HDC dc) {
		Matrix<> Vertices = m.getVertices();
		Matrix<int> Edges = m.getEdges();
		int points = m.getPoints();

		if (points == 0)
			return;

		for (int i = 1; i <= points; i++) {
			for(int j = i + 1 ; j <= points ; j ++)
				if (Edges(i,j) == 1) {
					double xi = Vertices(1, i) / Vertices(3, i);
					double yi = Vertices(2, i) / Vertices(3, i);

					double xj = Vertices(1, j) / Vertices(3, j);
					double yj = Vertices(2, j) / Vertices(3, j);

					MoveTo(xi, yi);
					LineToIN(dc, xj, yj);
				}
		}
	}
};

#endif SCENE_2D_H

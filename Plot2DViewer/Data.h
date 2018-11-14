#define _USE_MATH_DEFINES
#include <cmath>

double					// Исходные значения параметров L, R, B, T
	L=-10,
	R=10,
	B=-10,
	T=10;

double Parabola(double x)
{
	return x*x;
}

double Sinusoid(double x)
{
	return sin(x);
}

double Linear(double x)
{
	return x;
}

double Circle(double phi)
{
	return 1;
}

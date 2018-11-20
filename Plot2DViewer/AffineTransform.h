#ifndef AFFINE_TRANSFORM_H
#define AFFINE_TRANSFORM_H

#define _USE_MATH_DEFINES
#include "math.h"


double toRad(double degree)
{
	return degree * M_PI / 180;
}
Matrix<> Identity()
{
	double T[9] = {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1 };
	return Matrix<>(3, 3, T);
}
Matrix<> Translation(double x, double y)
{
	double T[9] = {
		1, 0, x,
		0, 1, y,
		0, 0, 1 };
	return Matrix<>(3, 3, T);
}
Matrix<> Rotation(double degree)
{
	double rad = toRad(degree);

	double T[9] = {
		cos(rad), -sin(rad), 0,
		sin(rad), cos(rad), 0,
		0,	0, 1 };
	return Matrix<>(3, 3, T);
}
Matrix<> Scaling(double kx, double ky)
{
	double T[9] = {
		kx, 0, 0,
		0, ky, 0,
		0,	0, 1 };
	return Matrix<>(3, 3, T);
}
Matrix<> MappingX()
{
	return Scaling(1, -1);
}
Matrix<> MappingY()
{
	return Scaling(-1, 1);
}
Matrix<> MappingO()
{
	return Scaling(-1, -1);
}
// ����� ��������� ����������� �������, ������������ ������� ������� ��:
// Identity() - ������������� ��;
// Rotation(t) - ������� �� ���� t;
// Rotation(c, s) - ������� �� ����, ������� � ����� �������� ��������������� ��������� c � s;
// Scaling(kx, ky) - ���������������;
// Mapping (��������� ���� ���������) - �� �������, ��� �������� ������ ��������� ����� ������������ ������� Scaling.

// � ���������� ������� ������������ ����������� ������� �� ������.
// �������������� ���� � ������ �������� �������� ������.
// ��������, ����� �������, ��������� ��������� �������,
// � ����������� ���������������� ��������� � �������������.

#endif AFFINE_TRANSFORM_H

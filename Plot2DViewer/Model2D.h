#ifndef MODEL2D_H
#define MODEL2D_H

#include <iostream>
#include "Matrix.h"
#include "AffineTransform.h"
#include <iostream>

using namespace std;

class Model2D
{
private:
	Matrix<> Vertices;						//матрица точек
	Matrix<> Edges;							//матрица ребер
	int points;								//количество точек
	Matrix<> Affine;						//матрица накопленного аффиного преобразования
public:
							
	Model2D() : Vertices(), Edges(), Affine(), points() {}
	Model2D(Matrix<>, Matrix<>);			//конструктор на основе матрицы точек и матрицы ребер в виде матрицы смежности
	Model2D(const Model2D&);				//копирование

	friend ostream& operator <<  (ostream&, const Model2D&);

	Matrix<> getVertices();				//получение приватных полей
	Matrix<> getEdges();
	Matrix<> getAffine();
	int getPoints();

	void apply(Matrix<> AT);				//применение аффиного преобразования

	
	
};

Model2D::Model2D(Matrix<> v, Matrix<> e)
{
	Affine = Identity();
	Vertices = v;
	Edges = e;
	points = Vertices.getCols();
}

Model2D::Model2D(const Model2D& r)
{
	this->Vertices = r.Vertices;
	this->Edges = r.Edges;
	this->points = r.points;
	this->Affine = r.Affine;
}

inline Matrix<> Model2D::getVertices()
{
	return Affine * Vertices;
}

inline Matrix<> Model2D::getEdges()
{
	return Edges;
}

inline Matrix<> Model2D::getAffine()
{
	return Affine;
}

inline int Model2D::getPoints()
{
	return points;
}

inline void Model2D::apply(Matrix<> AT)
{
	Affine = AT * Affine;
}

ostream& operator << (ostream& fo, const Model2D& M)
{
	fo << "Verticies: " << endl
		<< M.Vertices << endl
		<< "Edges: " << endl
		<< M.Edges << endl
		<< "Affine: " << endl
		<< M.Affine << endl;
	return fo;
}
#endif MODEL2D_H

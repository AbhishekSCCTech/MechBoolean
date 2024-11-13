#include "Matrix4x4.h"

using namespace Geometry;

Matrix4x4::Matrix4x4() : matrix({ {1.0,0.0,0.0,0.0} ,{0.0,1.0,0.0,0.0} ,{0.0,0.0,1.0,0.0} ,{0.0,0.0,0.0,1.0} })
{
}

Matrix4x4::~Matrix4x4()
{
}

void Geometry::Matrix4x4::setval(double value, int x, int y)
{
	matrix[x][y] = value;
}

std::vector<double> Matrix4x4::multiply(double _x, double _y, double _z)
{

	std::vector<double> matrix2{ _x, _y, _z, 1 };
	std::vector<double> ans{ 0.0,0.0,0.0,0.0 };
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ans[i] += matrix[i][j] * matrix2[j];
		}
	}
	return ans;
}
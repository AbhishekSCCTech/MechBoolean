#pragma once
#include <vector>
#include "Point.h"

namespace Geometry
{
	class Matrix4x4
	{
		std::vector<std::vector<double>> matrix;
	public:
		Matrix4x4();
		~Matrix4x4();
		void setval(double value, int x, int y);
		std::vector<double> multiply(double _x, double _y, double _z);

	};
}
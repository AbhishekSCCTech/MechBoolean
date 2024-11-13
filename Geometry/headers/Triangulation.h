#pragma once
#include "Triangle.h"
#include<vector>

namespace Geometry
{
    class Triangulation
    {
    public:
        Triangulation();
        ~Triangulation();

        std::vector<double> UniqueNumbers;
        std::vector<Triangle> Triangles;
        void add(const Triangulation& other);  // If this method exists
        void clear();
    private:
        std::vector<Point> points; // Store points of the triangulation
    };
}
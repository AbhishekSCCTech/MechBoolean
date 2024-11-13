#pragma once

#include "Triangulation.h"  // Include necessary header files for Triangulation and other classes
#include "Triangle.h"       // Include the Triangle class (assuming it exists)
#include "Point.h"          // Include the Point class (assuming it exists)

namespace Geometry
{
    class TriangulationSetOperations
    {
    public:
        // Static function to combine two triangulations side by side
        static Triangulation joinSideBySide(Triangulation& triang1, Triangulation& triang2);
    };
}


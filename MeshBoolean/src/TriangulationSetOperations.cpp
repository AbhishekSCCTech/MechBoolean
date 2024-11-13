#include "TriangulationSetOperations.h"

namespace Geometry
{
    Triangulation TriangulationSetOperations::joinSideBySide(Triangulation& triang1, Triangulation& triang2)
    {
        Triangulation result = triang1;

        // Find the maximum X or Z value in triang1 to avoid overlap
        int maxX = 0;
        for (Triangle& tri : triang1.Triangles)
        {
            maxX = std::max({ maxX, tri.P1().X(), tri.P2().X(), tri.P3().X() });
        }

        // Shift triang2 to the right by maxX + 1 (to avoid overlap)
        int shiftX = maxX + 1;

        // Modify each point in triang2 and move it to the right
        for (Triangle& tri : triang2.Triangles)
        {
            Point p1 = tri.P1();
            Point p2 = tri.P2();
            Point p3 = tri.P3();

            // Shift all points of triang2
            p1 = Point(p1.X() + shiftX, p1.Y(), p1.Z());
            p2 = Point(p2.X() + shiftX, p2.Y(), p2.Z());
            p3 = Point(p3.X() + shiftX, p3.Y(), p3.Z());

            // Create a new shifted triangle and add it to the result triangulation
            Triangle shiftedTriangle(tri.Normal(), p1, p2, p3);
            result.Triangles.push_back(shiftedTriangle);
        }

        return result; // Return the resulting triangulation
    }
}

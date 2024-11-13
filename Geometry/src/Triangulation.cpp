#include "Triangulation.h"

using namespace Geometry;

Triangulation::Triangulation()
{

}

Triangulation::~Triangulation()
{

}

void Triangulation::add(const Triangulation& other) {
    // Add points from the 'other' Triangulation to this one
    points.insert(points.end(), other.points.begin(), other.points.end());
}

// Definition of the clear method
void Triangulation::clear() {
    // Clears the points vector
    points.clear();
}
#include "Transformation.h"
#include <map>
#define TOLERANCE 0.0000001
#define M_PI 3.14159265358979323846


using namespace Geometry;

bool Transformation::Transformation::operator()(double a, double b) const
{
    return fabs(a - b) > TOLERANCE ? a < b : false;
}

Triangulation Transformation::Transformation::scaling(Triangulation& triangulation, Geometry::Matrix4x4& matrix, double scalex, double scaley, double scalez)
{
    std::vector<double> nmatrix{ 0.0,0.0,0.0,0.0 };
    std::vector<Point> vertices;
    matrix.setval(scalex, 0, 0);
    matrix.setval(scaley, 1, 1);
    matrix.setval(scalez, 2, 2);
    std::map<double, int, Transformation> uniqueMap;
    Triangulation newtriangulation;

    for (Triangle tri : triangulation.Triangles)
    {
        std::vector<Point> tempvect;
        tempvect = tri.Points();
        tempvect.push_back(tri.Normal());
        for (Point pt : tempvect)
        {
            nmatrix = matrix.multiply(triangulation.UniqueNumbers[pt.X()], triangulation.UniqueNumbers[pt.Y()], triangulation.UniqueNumbers[pt.Z()]);
            int pts[3];
            for (int i = 0; i < 3; i++)
            {
                auto pair = uniqueMap.find(nmatrix[i]);
                if (pair == uniqueMap.end())
                {
                    newtriangulation.UniqueNumbers.push_back(nmatrix[i]);
                    uniqueMap[nmatrix[i]] = newtriangulation.UniqueNumbers.size() - 1;
                    pts[i] = newtriangulation.UniqueNumbers.size() - 1;

                }
                else
                {
                    pts[i] = pair->second;

                }
            }
            vertices.push_back(Point(pts[0], pts[1], pts[2]));
        }
        newtriangulation.Triangles.push_back(Triangle(vertices[3], vertices[0], vertices[1], vertices[2]));
        vertices.clear();
    }
    return newtriangulation;
}

Triangulation Transformation::Transformation::translation(Triangulation& triangulation, Geometry::Matrix4x4& matrix, double tx, double ty, double tz)
{
    std::vector<double> nmatrix{ 0.0,0.0,0.0,0.0 };
    std::vector<Point> vertices;
    matrix.setval(tx, 0, 3);
    matrix.setval(ty, 1, 3);
    matrix.setval(tz, 2, 3);
    std::map<double, int, Transformation> uniqueMap;
    Triangulation newtriangulation;

    for (Triangle tri : triangulation.Triangles)
    {
        std::vector<Point> tempvect;
        tempvect = tri.Points();
        tempvect.push_back(tri.Normal());
        for (Point pt : tempvect)
        {
            nmatrix = matrix.multiply(triangulation.UniqueNumbers[pt.X()], triangulation.UniqueNumbers[pt.Y()], triangulation.UniqueNumbers[pt.Z()]);
            int pts[3];
            for (int i = 0; i < 3; i++)
            {
                auto pair = uniqueMap.find(nmatrix[i]);
                if (pair == uniqueMap.end())
                {
                    newtriangulation.UniqueNumbers.push_back(nmatrix[i]);
                    uniqueMap[nmatrix[i]] = newtriangulation.UniqueNumbers.size() - 1;
                    pts[i] = newtriangulation.UniqueNumbers.size() - 1;

                }
                else
                {
                    pts[i] = pair->second;

                }
            }
            vertices.push_back(Point(pts[0], pts[1], pts[2]));
        }
        newtriangulation.Triangles.push_back(Triangle(vertices[3], vertices[0], vertices[1], vertices[2]));
        vertices.clear();
    }
    return newtriangulation;
}


using namespace Geometry;

Triangulation Transformation::Transformation::rotation(Triangulation& triangulation, Geometry::Matrix4x4& matrix, double anglex, double angley, double anglez)
{
    std::vector<double> nmatrix{ 0.0, 0.0, 0.0, 0.0 };
    std::vector<Point> vertices;
    std::map<double, int, Transformation> uniqueMap;
    Triangulation newtriangulation;

    // Convert angles from degrees to radians
    double radX = anglex * M_PI / 180.0;
    double radY = angley * M_PI / 180.0;
    double radZ = anglez * M_PI / 180.0;

    // Set up rotation matrix for X axis
    if (anglex != 0) {
        matrix.setval(1, 0, 0);
        matrix.setval(cos(radX), 1, 1);
        matrix.setval(-sin(radX), 1, 2);
        matrix.setval(sin(radX), 2, 1);
        matrix.setval(cos(radX), 2, 2);
    }
    // Set up rotation matrix for Y axis
    if (angley != 0) {
        matrix.setval(cos(radY), 0, 0);
        matrix.setval(sin(radY), 0, 2);
        matrix.setval(-sin(radY), 2, 0);
        matrix.setval(cos(radY), 2, 2);
    }
    // Set up rotation matrix for Z axis
    if (anglez != 0) {
        matrix.setval(cos(radZ), 0, 0);
        matrix.setval(-sin(radZ), 0, 1);
        matrix.setval(sin(radZ), 1, 0);
        matrix.setval(cos(radZ), 1, 1);
    }

    for (Triangle tri : triangulation.Triangles)
    {
        std::vector<Point> tempvect = tri.Points();
        tempvect.push_back(tri.Normal());
        for (Point pt : tempvect)
        {
            nmatrix = matrix.multiply(triangulation.UniqueNumbers[pt.X()], triangulation.UniqueNumbers[pt.Y()], triangulation.UniqueNumbers[pt.Z()]);
            int pts[3];
            for (int i = 0; i < 3; i++)
            {
                auto pair = uniqueMap.find(nmatrix[i]);
                if (pair == uniqueMap.end())
                {
                    newtriangulation.UniqueNumbers.push_back(nmatrix[i]);
                    uniqueMap[nmatrix[i]] = newtriangulation.UniqueNumbers.size() - 1;
                    pts[i] = newtriangulation.UniqueNumbers.size() - 1;
                }
                else
                {
                    pts[i] = pair->second;
                }
            }
            vertices.push_back(Point(pts[0], pts[1], pts[2]));
        }
        newtriangulation.Triangles.push_back(Triangle(vertices[3], vertices[0], vertices[1], vertices[2]));
        vertices.clear();
    }
    return newtriangulation;
}

//Triangulation Transformation::Transformation::rotationX(Triangulation& triangulation, Geometry::Matrix4x4& matrix, double anglex, double angley, double anglez)
//{
//    std::vector<double> nmatrix{ 0.0,0.0,0.0,0.0 };
//    std::vector<Point> vertices;
//    // Convert angles from degrees to radians
//    double radX = anglex * M_PI / 180.0;
//    double radY = angley * M_PI / 180.0;
//    double radZ = anglez * M_PI / 180.0;
//    // Rotation around the X-axis
//    matrix.setval(1, 0, 0);
//    matrix.setval(cos(radX), 1, 1);
//    matrix.setval(-sin(radX), 1, 2);
//    matrix.setval(sin(radX), 2, 1);
//    matrix.setval(cos(radX), 2, 2);
//    std::map<double, int, Transformation> uniqueMap;
//    Triangulation newtriangulation;
//
//    for (Triangle tri : triangulation.Triangles)
//    {
//        std::vector<Point> tempvect;
//        tempvect = tri.Points();
//        tempvect.push_back(tri.Normal());
//        for (Point pt : tempvect)
//        {
//            nmatrix = matrix.multiply(triangulation.UniqueNumbers[pt.X()], triangulation.UniqueNumbers[pt.Y()], triangulation.UniqueNumbers[pt.Z()]);
//            int pts[3];
//            for (int i = 0; i < 3; i++)
//            {
//                auto pair = uniqueMap.find(nmatrix[i]);
//                if (pair == uniqueMap.end())
//                {
//                    newtriangulation.UniqueNumbers.push_back(nmatrix[i]);
//                    uniqueMap[nmatrix[i]] = newtriangulation.UniqueNumbers.size() - 1;
//                    pts[i] = newtriangulation.UniqueNumbers.size() - 1;
//
//                }
//                else
//                {
//                    pts[i] = pair->second;
//
//                }
//            }
//            vertices.push_back(Point(pts[0], pts[1], pts[2]));
//        }
//        newtriangulation.Triangles.push_back(Triangle(vertices[3], vertices[0], vertices[1], vertices[2]));
//        vertices.clear();
//    }
//    return newtriangulation;
//}
#include <QString>
#include <QList>
#include <fstream>
#include <iostream>
#include "OBJReader.h"
#include "Triangulation.h"
#include "Point.h"
#include "Triangle.h"
#include <vector>
#include <sstream>
#include <cassert>
#include <string>
#include <map>

#define TOLERANCE 0.0000001
OBJReader::OBJReader()
{
}

OBJReader::~OBJReader()
{
}
bool OBJReader::operator()(double a, double b) const
{
    return fabs(a - b) > TOLERANCE ? a < b : false;
}
void OBJReader::read(const std::string& fileName, Triangulation& triangulation)
{
    std::map<double, int, OBJReader> uniqueMap;
    double xyz[3];
    double normalXYZ[3];
    std::string fLetter;
    std::string str1;
    std::string str2;
    std::string str3;
    //vector<int> pointIndices;
    std::vector<Point> vertices;
    std::vector<Point> normals;


    std::ifstream infile(fileName);
    //assert(infile && "Error: Could not open file");
    if (infile.is_open())
    {
        std::string line;

        while (getline(infile, line))
        {
            std::stringstream ss(line);
            QString _line = QString::fromStdString(line);
            QStringList linelist = _line.split(" ");
            //std::cout << linelist.value(0).toStdString() << std::endl;
            if (linelist.value(0) == "v")
            {
                xyz[0] = linelist.value(1).toDouble();
                xyz[1] = linelist.value(2).toDouble();
                xyz[2] = linelist.value(3).toDouble();

                int pt[3];

                for (int i = 0; i < 3; i++)
                {
                    auto pair = uniqueMap.find(xyz[i]);
                    if (pair == uniqueMap.end())
                    {
                        triangulation.UniqueNumbers.push_back(xyz[i]);
                        uniqueMap[xyz[i]] = triangulation.UniqueNumbers.size() - 1;
                        pt[i] = triangulation.UniqueNumbers.size() - 1;

                    }
                    else
                    {
                        pt[i] = pair->second;

                    }

                }
                vertices.push_back(Point(pt[0], pt[1], pt[2]));
            }

            if (linelist.value(0) == "vn")
            {
                normalXYZ[0] = linelist.value(1).toDouble();
                normalXYZ[1] = linelist.value(2).toDouble();
                normalXYZ[2] = linelist.value(3).toDouble();

                int pt[3];

                for (int i = 0; i < 3; i++)
                {
                    auto pair = uniqueMap.find(normalXYZ[i]);
                    if (pair == uniqueMap.end())
                    {
                        triangulation.UniqueNumbers.push_back(normalXYZ[i]);
                        uniqueMap[normalXYZ[i]] = triangulation.UniqueNumbers.size() - 1;
                        pt[i] = triangulation.UniqueNumbers.size() - 1;

                    }
                    else
                    {
                        pt[i] = pair->second;
                    }
                }
                normals.push_back(Point(pt[0], pt[1], pt[2]));
            }

            if (linelist.value(0) == "f")
            {
                ss >> fLetter >> str1 >> str2 >> str3;
                QString str_s1 = QString::fromStdString(str1);
                QString str_s2 = QString::fromStdString(str2);
                QString str_s3 = QString::fromStdString(str3);
                QStringList splitList = str_s1.split("/");
                splitList << str_s2.split("/") << str_s3.split("/");
                int normalId = splitList.value(2).toInt() - 1;
                int firstVertexId = splitList.value(0).toInt() - 1;
                int secondVertexId = splitList.value(3).toInt() - 1;
                int thirdVertexId = splitList.value(6).toInt() - 1;

                for (int i = 0; i < splitList.size(); i++)
                    Point v1 = vertices[firstVertexId];
                Point v2 = vertices[secondVertexId];
                Point v3 = vertices[thirdVertexId];
                triangulation.Triangles.push_back(Triangle(normals[normalId], vertices[firstVertexId], vertices[secondVertexId], vertices[thirdVertexId]));
            }
        }
    }
}
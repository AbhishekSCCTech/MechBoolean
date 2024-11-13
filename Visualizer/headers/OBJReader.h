#pragma once

#include <string> 
#include "Triangulation.h"
#include "Reader.h"

class OBJReader : public Reader
{
public:
	OBJReader();
	~OBJReader();

	void read(const std::string& fileName, Triangulation& tri);
	bool operator()(double a, double b) const;
};
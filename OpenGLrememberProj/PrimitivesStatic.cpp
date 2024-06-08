#include "PrimitivesStatic.h"

#define _USE_MATH_DEFINES
#include <math.h>

std::vector<Vector3> CircleStatic::points;
std::vector<Vector3> SphereStatic::points;
std::vector<Vector3> BoxStatic::points;
std::vector<Vector3> SStatic::points;

void CircleStatic::MakeVertex()
{
		
		double k = M_PI / 180;
		double rad;
		for (int i = 0; i <= 360; i += 10)
		{
			rad = i*k;
			points.push_back(Vector3(sin(rad), cos(rad), 0));
		}
		
	
}

void SphereStatic::MakeVertex()
{
	int step = 15;
	for (int fi = 0; fi < 180; fi+=step)
	{
		for (int eta = 0; eta < 360; eta+=step)
		{
			points.push_back(Vector3(sin(eta*M_PI / 180)*sin(fi*M_PI / 180), cos(eta*M_PI / 180)*sin(fi*M_PI / 180), cos(fi*M_PI / 180)));
			points.push_back(Vector3(sin(eta*M_PI / 180)*sin((fi + step)*M_PI / 180), cos(eta*M_PI / 180)*sin((fi + step)*M_PI / 180), cos((fi + step)*M_PI / 180)));
			//points.push_back(Vector3(sin((eta + step)*M_PI / 180)*sin((fi + step)*M_PI / 180), cos((eta + step)*M_PI / 180)*sin((fi + step)*M_PI / 180), cos((fi + step)*M_PI / 180)));
		}
	}
}

void SStatic::MakeVertex()
{
	int step = 15;
	for (int fi = 0; fi < 180; fi += step)
	{
		for (int eta = 0; eta < 360; eta += step)
		{
			points.push_back(Vector3(sin(eta * M_PI / 180) * sin(fi * M_PI / 180), cos(eta * M_PI / 180) * sin(fi * M_PI / 180), cos(fi * M_PI / 180)));
			points.push_back(Vector3(sin(eta * M_PI / 180) * sin((fi + step) * M_PI / 180), cos(eta * M_PI / 180) * sin((fi + step) * M_PI / 180), cos((fi + step) * M_PI / 180)));
			//points.push_back(Vector3(sin((eta + step)*M_PI / 180)*sin((fi + step)*M_PI / 180), cos((eta + step)*M_PI / 180)*sin((fi + step)*M_PI / 180), cos((fi + step)*M_PI / 180)));
		}
	}
}

void BoxStatic::MakeVertex()
{
	//for (int i = 0; i < 8; i++)
	//{
	//	points[i] = Vector3((i & 4) >> 1 - 1, (i & 2)^(i & 4 >> 1) - 1, (i & 2)^(i & 1 << 1) - 1);
	//}

	//points[0] = Vector3(-1, -1, -1);
	//points[1] = Vector3(-1, -1, 1);
	//points[2] = Vector3(-1, 1, 1);
	//points[3] = Vector3(-1, 1, -1);
	//points[4] = Vector3(1, 1, -1);
	//points[5] = Vector3(1, 1, 1);
	//points[6] = Vector3(1, -1, 1);
	//points[7] = Vector3(1, -1, -1);

	int step = 15;
	for (int fi = 0; fi < 180; fi += step)
	{
		for (int eta = 0; eta < 360; eta += step)
		{
			points.push_back(Vector3(sin(eta * M_PI / 180) * sin(fi * M_PI / 180), cos(eta * M_PI / 180) * sin(fi * M_PI / 180), cos(fi * M_PI / 180)));
			points.push_back(Vector3(sin(eta * M_PI / 180) * sin((fi + step) * M_PI / 180), cos(eta * M_PI / 180) * sin((fi + step) * M_PI / 180), cos((fi + step) * M_PI / 180)));
			//points.push_back(Vector3(sin((eta + step)*M_PI / 180)*sin((fi + step)*M_PI / 180), cos((eta + step)*M_PI / 180)*sin((fi + step)*M_PI / 180), cos((fi + step)*M_PI / 180)));
		}
	}

}

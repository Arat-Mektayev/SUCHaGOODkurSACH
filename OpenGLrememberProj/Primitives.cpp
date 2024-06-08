#include "Primitives.h"
#include "PrimitivesStatic.h"

void Sphere::RenderObject()
{
	glBegin(GL_TRIANGLE_STRIP);
		
	for (unsigned short i = 0; i < SphereStatic::points.size(); i++)
	{
		glVertex3dv(SphereStatic::points[i].toArray());
	}
	glEnd();
}

void S::RenderObject()
{
	glBegin(GL_TRIANGLE_STRIP);

	for (unsigned short i = 0; i < SStatic::points.size(); i++)
	{
		glVertex3dv(SStatic::points[i].toArray());
	}
	glEnd();
}

void Circle::RenderObject()
{
	glBegin(GL_LINE_STRIP);
	for (unsigned short i = 0; i < CircleStatic::points.size(); i++)
	{
		glVertex3dv(CircleStatic::points[i].toArray());
	}
	glEnd();
}

void Box::RenderObject()
{
	glBegin(GL_TRIANGLE_STRIP);
	for (unsigned short i = 0; i < BoxStatic::points.size(); i++)
	{
		glVertex3dv(BoxStatic::points[i].toArray());
	}
	glEnd();
}

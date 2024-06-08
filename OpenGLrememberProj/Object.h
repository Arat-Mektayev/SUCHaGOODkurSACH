#ifndef OBJECT_H
#define OBJECT_H

#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "MyVector3d.h"

//абстрактный объект
class Object
{
public:
	Vector3 pos;

	virtual void move(GLdouble x, GLdouble y, GLdouble z);

	virtual void move(Vector3 &v);
};

class RenderadbleObject: public Object
{
public:
	//Пока не работает
	Vector3 scale;
	Vector3 rotateAxe;
	double angle;

	RenderadbleObject()
	{
		scale = Vector3(1, 1, 1);
		rotateAxe = Vector3(0, 0, 1);
		angle = 0;
	}

	void Show()
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glTranslated(pos.X(), pos.Y(), pos.Z());
		glRotated(angle, rotateAxe.X(), rotateAxe.Y(), rotateAxe.Z());
		glScaled(scale.X(), scale.Y(), scale.Z());
		RenderObject();
		glPopMatrix();
	}

	virtual void RenderObject() = 0;
};

#endif
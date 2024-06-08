#include "Object.h"

void Object::move(GLdouble x, GLdouble y, GLdouble z)
{
	Vector3 v(x, y, z);
	move(v);
}

void Object::move(Vector3 &v)
{
	pos = pos + v;
}
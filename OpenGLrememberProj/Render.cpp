#include "Render.h"

#include <ctime>

#include <sstream>
#include <iostream>
#include <math.h>
#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "vecfunctions.h"
#include "Player.h"
#include "GUItextRectangle.h"

//<mygoodies>ы
#define RED 1,0,0
#define GREEN 0,1,0
#define BLUE 0,0,1

#define CYAN 0,1,1
#define MAGENTA 1,0,1
#define YELLOW 1,1,0

const int WallHeight = 5;
const int CircleApproxDepth = 11;
const double pi = 3.1415926535;
const float ddegree = 0.04;
const byte sas = 0;

void approx1(double dot1[3], double dot2[3], double dots[CircleApproxDepth][3], int sign = 1) {

	double coords[3], delta[3];
	double l;
	double a = pi / CircleApproxDepth, a2;


	for (int i = 0; i < 2; i++) {
		coords[i] = (dot1[i] + dot2[i]) / 2;
	}
	coords[2] = dot1[2];

	for (int i = 0; i < 2; i++) {
		delta[i] = dot1[i] - dot2[i];
	}

	l = sqrt(delta[0] * delta[0] + delta[1] * delta[1]) / 2;
	a2 = atan((dot1[1] - dot2[1]) / (dot1[0] - dot2[0])) * sign;

	for (int i = 0; i < CircleApproxDepth; i++) {
		coords[0] = cos(a * i + a2) * l;
		coords[1] = sin(a * i + a2) * l;
		for (int j = 0; j < 3; j++) {
			dots[i][j] = coords[j];
		}
	}
}

void approx2(double dots1[CircleApproxDepth][3], double dots2[CircleApproxDepth][3]) {
	for (int i = 0; i < CircleApproxDepth; i++) {
		for (int j = 0; j < 3; j++) {
			dots2[i][j] = dots1[i][j];
		}
		dots2[i][2] += WallHeight;
	}
}

// </mygoodies>

bool textureMode = true;
bool lightMode = true;
bool xray = true;

void EntityContainer::Update() {
	for (auto p = projs.begin(); p != projs.end();) {
		if (p->Update()) {
			projs.erase(p++);
		} else p++;
	}
	for (auto m = mobs.begin(); m != mobs.end();) {
		if (m->Update()) {
			mobs.erase(m++);
		}
		else m++;
	}
};

EntityContainer host;
Player player(1, 1, 0.4, Vector3(0, 3, 0), 0, &host);
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//начальная позиция света
		pos = Vector3(3, -4, 8);
	}

	
	//рисует сферу и линии под источником света, вызывается движком
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);

		
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale*0.08;
		s.Show();
		
		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//линия от источника света до окружности
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//рисуем окружность
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale*1.5;
			c.Show();
		}
	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// параметры источника света
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// характеристики излучаемого света
		// фоновое освещение (рассеянный свет)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// диффузная составляющая света
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// зеркально отражаемая составляющая света
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}
} light;
class CustomCamera : public Camera
{
public:
	//дистанция камеры
	double camDist;
	//углы поворота камеры
	double fi1, fi2;

	CustomCamera()
	{
		camDist = 3.1;
		fi1 = 0;
		fi2 = 0;
		
	}

	//считает позицию камеры, исходя из углов поворота, вызывается движком
	void SetUpCamera()
	{
		//отвечает за поворот камеры мышкой
		lookPoint.setCoords(player.body.pos.X(), player.body.pos.Y(), player.body.pos.Z());
		player.body.angle = fi1;
		pos.setCoords(camDist * cos(fi2) * -cos(fi1) + lookPoint.X(),
			camDist * cos(fi2) * -sin(fi1) + lookPoint.Y(),
			camDist * sin(fi2) + lookPoint.Z());

		normal.setCoords(0, 0, -((cos(fi2) <= 0) * 2 - 1));

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		//функция настройки камеры
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}

} camera;

//старые координаты мыши
int mouseX = 0, mouseY = 0;

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//меняем углы камеры при нажатой левой кнопке мыши
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}
	
	//двигаем свет по плоскости, в точку где мышь
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	//if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	//{
	//	light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	//}

}

void mouseWheelEvent(OpenGL *ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01*delta;

}

void keyDownEvent(OpenGL *ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
	}

	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}

	if (key == 'Q')
	{
		xray = !xray;
	}

	if (key == 'A') {
		Vector3 lp = camera.lookPoint;
		camera.lookPoint.setCoords(lp.X() + cos(camera.fi1-pi / 2), lp.Y() + sin(camera.fi1-pi / 2), lp.Z());;
	}

	if (key == 'D') {
		Vector3 lp = camera.lookPoint;
		camera.lookPoint.setCoords(lp.X() + cos(camera.fi1+pi / 2), lp.Y() + sin(camera.fi1+pi/2), lp.Z());
		//camera.fi1 += ddegree;
	}

	if (key == 'W') {
		Vector3 lp = camera.lookPoint;
		camera.lookPoint.setCoords(lp.X() - cos(camera.fi1), lp.Y() - sin(camera.fi1), lp.Z());
	}

	if (key == 'S') {
		Vector3 lp = camera.lookPoint;
		camera.lookPoint.setCoords(lp.X() + cos(camera.fi1), lp.Y() + sin(camera.fi1), lp.Z());
	}
}

void keyUpEvent(OpenGL *ogl, int key)
{
	
}

GLuint texId;

//выполняется перед первым рендером
void initRender(OpenGL *ogl)
{
	//настройка текстур

	//4 байта на хранение пикселя
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//настройка режима наложения текстур
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//включаем текстуры
	glEnable(GL_TEXTURE_2D);
	

	//массив трехбайтных элементов  (R G B)
	RGBTRIPLE *texarray;

	//массив символов, (высота*ширина*4 (4, потому что выше мы указали использовать по 4 байта на пиксель текстуры - R G B A))
	char *texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("texture.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);

	
	
	//генерируем ИД для текстуры
	glGenTextures(1, &texId);
	//биндим айдишник, все что будет происходить с текстурой, будте происходить по этому ИД
	glBindTexture(GL_TEXTURE_2D, texId);

	//загружаем текстуру в видеопямять, в оперативке нам больше  она не нужна
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	//отчистка памяти
	free(texCharArray);
	free(texarray);

	//наводим шмон
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//камеру и свет привязываем к "движку"
	ogl->mainCamera = &camera;
	ogl->mainLight = &light;

	// нормализация нормалей : их длины будет равна 1
	glEnable(GL_NORMALIZE);

	// устранение ступенчатости для линий
	glEnable(GL_LINE_SMOOTH); 

	//   задать параметры освещения
	//  параметр GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  лицевые и изнаночные рисуются одинаково(по умолчанию), 
	//                1 - лицевые и изнаночные обрабатываются разными режимами       
	//                соответственно лицевым и изнаночным свойствам материалов.    
	//  параметр GL_LIGHT_MODEL_AMBIENT - задать фоновое освещение, 
	//                не зависящее от источников
	// по умолчанию (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	for (int i = 0; i < 10; i++) {
		Mob(5, 1, .4, Vector3(rand() % 10 * 3, rand() % 10 * 3, 0), 0, &host);
	}
}

void Render(OpenGL *ogl)
{
	{
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_NORMALIZE);
		if (textureMode)
			glEnable(GL_TEXTURE_2D);

		if (lightMode)
			glEnable(GL_LIGHTING);


		//альфаналожение
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (xray)
		{
			glDisable(GL_BLEND);
		}

		//настройка материала
		GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
		GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
		GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
		GLfloat sh = 0.1f * 256;


		//фоновая
		glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
		//дифузная
		glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
		//зеркальная
		glMaterialfv(GL_FRONT, GL_SPECULAR, spec); \
			//размер блика
			glMaterialf(GL_FRONT, GL_SHININESS, sh);

		//чтоб было красиво, без квадратиков (сглаживание освещения)
		glShadeModel(GL_SMOOTH);
	}
	//===================================

	static int i, j, k, flag = 1;
	static double Dots[2][8][3] = {
		{ {1,-4,0}, {0,-8,0}, {-7,-7,0}, {-6,1,0}, {0,0,0}, {4,6,0}, {8,1,0}, {5,-5,0} },
		{ {1,-4,WallHeight}, {0,-8,WallHeight}, {-7,-7,WallHeight}, {-6,1,WallHeight}, {0,0,WallHeight}, {4,6,WallHeight}, {8,1,WallHeight}, {5,-5,WallHeight} }
	};
	static double Dots2[2][CircleApproxDepth][3];
	static double Dots3[2][CircleApproxDepth][3];
	static double n[3];

	static double colors[3][3] = { {MAGENTA}, {YELLOW}, {CYAN} };

	if (flag) {
		double mid[2][3];

		approx1(Dots[0][5], Dots[0][6], Dots2[0]);
		approx2(Dots2[0], Dots2[1]);

		for (i = 0; i < 2; i++) {
			for (j = 0; j < 3; j++) {
				mid[i][j] = (Dots[i][5][j] + Dots[i][6][j]) / 2;
				for (k = 0; k < CircleApproxDepth; k++) {
					Dots2[i][k][j] += mid[0][j];
				}
			}
		}

		approx1(Dots[0][2], Dots[0][3], Dots3[0], -1);
		approx2(Dots3[0], Dots3[1]);

		for (i = 0; i < 2; i++) {
			for (j = 0; j < 3; j++) {
				mid[i][j] = (Dots[i][2][j] + Dots[i][3][j]) / 2;
				for (k = 0; k < CircleApproxDepth; k++) {
					Dots3[i][k][j] += mid[0][j];
				}
			}
		}

		flag = 0;
	}

	if (sas) {
		for (i = 0; i < 2; i++) {
			glBindTexture(GL_TEXTURE_2D, texId);
			glColor4d(MAGENTA, 0.5);
			glNormal3d(0, 0, (GLdouble)(i << 1 - 1));
			glBegin(GL_TRIANGLE_FAN);

			for (j = 0; j < 3; j++) {
				glTexCoord2d(j / 2 % 2, j % 2);
				glVertex3dv(Dots[i][j]);
			}

			for (k = 0; k < CircleApproxDepth; k++) {
				glTexCoord2d(k << 1 / CircleApproxDepth % 2 * ((k << 1 / CircleApproxDepth) << 1 - 1), k % 2);
				glVertex3dv(Dots3[i][k]);
			}
			for (; j < 8; j++) {
				glTexCoord2d(j / 4 % 2, j % 2);
				glVertex3dv(Dots[i][j]);
			}
			glEnd();
			// Можно убрать соседние енд и бегин, но будет некрасиво.
			glBegin(GL_TRIANGLE_FAN);
			for (k = 0; k < CircleApproxDepth; k++) {
				glTexCoord2d(k / 4 % 2, k % 2);
				glVertex3dv(Dots2[i][k]);
			}
			glEnd();
			glBegin(GL_TRIANGLES);
			glTexCoord2d(0, 0);
			glVertex3dv(Dots2[i][0]);
			glTexCoord2d(0, 1);
			glVertex3dv(Dots[i][5]);
			glTexCoord2d(1, 1);
			glVertex3dv(Dots[i][6]);

			glTexCoord2d(0, 0);
			glVertex3dv(Dots2[i][CircleApproxDepth - 1]);
			glTexCoord2d(0, 1);
			glVertex3dv(Dots[i][5]);
			glTexCoord2d(1, 0);
			glVertex3dv(Dots[i][6]);
			glEnd();
		}


		glColor3dv(colors[2]);


		glBegin(GL_QUADS);


		for (j = 0; j < 2; j++) {
			glNormal3dv(Norm(Dots[0][j % 8], Dots[1][j % 8], Dots[1][(j + 1) % 8]));
			glTexCoord2d(1, 0);
			glVertex3dv(Dots[0][j % 8]);
			glTexCoord2d(1, 1);
			glVertex3dv(Dots[1][j % 8]);
			glTexCoord2d(0, 1);
			glVertex3dv(Dots[1][(j + 1) % 8]);
			glTexCoord2d(0, 0);
			glVertex3dv(Dots[0][(j + 1) % 8]);
		}


		glNormal3dv(Norm(Dots3[1][0], Dots3[0][0], Dots[1][j]));
		glTexCoord2d(0, 0);
		glVertex3dv(Dots3[0][0]);
		glTexCoord2d(0, 1);
		glVertex3dv(Dots3[1][0]);
		glTexCoord2d(1, 1);
		glVertex3dv(Dots[1][j]);
		glTexCoord2d(1, 0);
		glVertex3dv(Dots[0][j]);


		for (k = 0; k < CircleApproxDepth - 1; k++) {
			glNormal3dv(Norm(Dots3[0][k], Dots3[1][k], Dots3[1][k + 1]));
			glTexCoord2d(1 - (double)(k) / CircleApproxDepth, 0);
			glVertex3dv(Dots3[0][k]);
			glTexCoord2d(1 - (double)(k) / CircleApproxDepth, 1);
			glVertex3dv(Dots3[1][k]);
			glTexCoord2d(1 - (double)(k + 1) / CircleApproxDepth, 1);
			glVertex3dv(Dots3[1][k + 1]);
			glTexCoord2d(1 - (double)(k + 1) / CircleApproxDepth, 0);
			glVertex3dv(Dots3[0][k + 1]);
		}

		j++;

		glNormal3dv(Norm(Dots3[0][CircleApproxDepth - 1], Dots3[1][CircleApproxDepth - 1], Dots[1][j]));
		glTexCoord2d(1, 0);
		glVertex3dv(Dots3[0][CircleApproxDepth - 1]);
		glTexCoord2d(1, 1);
		glVertex3dv(Dots3[1][CircleApproxDepth - 1]);
		glTexCoord2d(0, 1);
		glVertex3dv(Dots[1][j]);
		glTexCoord2d(0, 0);
		glVertex3dv(Dots[0][j]);

		for (; j < 8; j++) {
			glNormal3dv(Norm(Dots[0][j % 8], Dots[1][j % 8], Dots[1][(j + 1) % 8]));
			glTexCoord2d(1, 0);
			glVertex3dv(Dots[0][j % 8]);
			glTexCoord2d(1, 1);
			glVertex3dv(Dots[1][j % 8]);
			glTexCoord2d(0, 1);
			glVertex3dv(Dots[1][(j + 1) % 8]);
			glTexCoord2d(0, 0);
			glVertex3dv(Dots[0][(j + 1) % 8]);

		}
		glEnd();

		glColor3dv(colors[2]);
		glBegin(GL_QUADS);

		glNormal3dv(Norm(Dots2[1][0], Dots2[0][0], Dots[0][6]));
		glTexCoord2d(1, 0);
		glVertex3dv(Dots2[1][0]);
		glTexCoord2d(1, 1);
		glVertex3dv(Dots2[0][0]);
		glTexCoord2d(0, 1);
		glVertex3dv(Dots[0][6]);
		glTexCoord2d(0, 0);
		glVertex3dv(Dots[1][6]);

		glNormal3dv(Norm(Dots2[1][CircleApproxDepth - 1], Dots2[0][CircleApproxDepth - 1], Dots[0][5]));

		glTexCoord2d(1, 0);
		glVertex3dv(Dots[0][5]);
		glTexCoord2d(0, 0);
		glVertex3dv(Dots2[0][CircleApproxDepth - 1]);
		glTexCoord2d(0, 1);
		glVertex3dv(Dots2[1][CircleApproxDepth - 1]);
		glTexCoord2d(1, 1);
		glVertex3dv(Dots[1][5]);


		for (j = 0; j < CircleApproxDepth; j++) {
			glNormal3dv(Norm(Dots2[1][j % CircleApproxDepth], Dots2[0][j % CircleApproxDepth], Dots2[1][(j + 1) % CircleApproxDepth]));
			glTexCoord2d((double)j / CircleApproxDepth, 0);
			glVertex3dv(Dots2[0][j % CircleApproxDepth]);
			glTexCoord2d((double)j / CircleApproxDepth, 1);
			glVertex3dv(Dots2[1][j % CircleApproxDepth]);
			glTexCoord2d((double)(j + 1) / CircleApproxDepth, 1);
			glVertex3dv(Dots2[1][(j + 1) % CircleApproxDepth]);
			glTexCoord2d((double)(j + 1) / CircleApproxDepth, 0);
			glVertex3dv(Dots2[0][(j + 1) % CircleApproxDepth]);
		}
		glEnd();
	}

	host.Update();
	player.Update();

	//Начало рисования квадратика станкина
	{
		double A[2] = { -4, -4 };
		double B[2] = { 4, -4 };
		double C[2] = { 4, 4 };
		double D[2] = { -4, 4 };

		glBindTexture(GL_TEXTURE_2D, texId);

		glColor3d(0.6, 0.6, 0.6);
		glBegin(GL_QUADS);

		glNormal3d(0, 0, 1);
		glTexCoord2d(0, 0);
		glVertex2dv(A);
		glTexCoord2d(1, 0);
		glVertex2dv(B);
		glTexCoord2d(1, 1);
		glVertex2dv(C);
		glTexCoord2d(0, 1);
		glVertex2dv(D);

		glEnd();
	}
	//конец рисования квадратика станкина
 
   //Сообщение вверху экрана
	{
		glMatrixMode(GL_PROJECTION);	//Делаем активной матрицу проекций. 
		//(всек матричные операции, будут ее видоизменять.)
		glPushMatrix();   //сохраняем текущую матрицу проецирования (которая описывает перспективную проекцию) в стек 				    
		glLoadIdentity();	  //Загружаем единичную матрицу
		glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //врубаем режим ортогональной проекции

		glMatrixMode(GL_MODELVIEW);		//переключаемся на модел-вью матрицу
		glPushMatrix();			  //сохраняем текущую матрицу в стек (положение камеры, фактически)
		glLoadIdentity();		  //сбрасываем ее в дефолт

		glDisable(GL_LIGHTING);


		GuiTextRectangle rec;		   //классик моего авторства для удобной работы с рендером текста.
		rec.setSize(300, 170);
		rec.setPosition(10, ogl->getHeight() - 170 - 10);


		std::stringstream ss;
		ss << "T - вкл/выкл текстур" << std::endl;
		ss << "L - вкл/выкл освещение" << std::endl;
		ss << "F - Свет из камеры" << std::endl;
		ss << "G - двигать свет по горизонтали" << std::endl;
		ss << "G+ЛКМ двигать свет по вертикали" << std::endl;
		ss << "Коорд. света: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
		//ss << "Коорд. камеры: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
		//ss << "Параметры камеры: R=" << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;
		//ss << "Параметры камеры: X=" << camera.lookPoint.X() << ", Y=" << camera.lookPoint.Y() << ", Z=" << camera.lookPoint.Z() << std::endl;
		//ss << "DEBUG (" << p.body.pos.X() << ", " << p.body.pos.Y() << ", " << p.body.pos.Z() << ") " << OpenGL::isKeyPressed(VK_LBUTTON) << " " << p.time << " "  << host.projs.size() << std::endl;
		rec.setText(ss.str().c_str());
		rec.Draw();

		glMatrixMode(GL_PROJECTION);	  //восстанавливаем матрицы проекции и модел-вью обратно из стека.
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
}

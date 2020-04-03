#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "glut.h"

// dimensiunea ferestrei in pixeli
#define dim 500

unsigned char prevKey;

// concoida lui Nicomede (concoida dreptei)
// $x = a + b \cdot cos(t), y = a \cdot tg(t) + b \cdot sin(t)$. sau
// $x = a - b \cdot cos(t), y = a \cdot tg(t) - b \cdot sin(t)$. unde
// $t \in (-\pi / 2, \pi / 2)$
void Display1() {
	double xmax, ymax, xmin, ymin;
	double a = 1, b = 2;
	double pi = 4 * atan(1.0);
	double ratia = 0.05;
	double t;

	// calculul valorilor maxime/minime ptr. x si y
	// aceste valori vor fi folosite ulterior la scalare
	xmax = a - b - 1;
	xmin = a + b + 1;
	ymax = ymin = 0;
	for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
		double x1, y1, x2, y2;
		x1 = a + b * cos(t);
		xmax = (xmax < x1) ? x1 : xmax;
		xmin = (xmin > x1) ? x1 : xmin;

		x2 = a - b * cos(t);
		xmax = (xmax < x2) ? x2 : xmax;
		xmin = (xmin > x2) ? x2 : xmin;

		y1 = a * tan(t) + b * sin(t);
		ymax = (ymax < y1) ? y1 : ymax;
		ymin = (ymin > y1) ? y1 : ymin;

		y2 = a * tan(t) - b * sin(t);
		ymax = (ymax < y2) ? y2 : ymax;
		ymin = (ymin > y2) ? y2 : ymin;
	}

	xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
	ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

	// afisarea punctelor propriu-zise precedata de scalare
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
		double x1, y1, x2, y2;
		x1 = (a + b * cos(t)) / xmax;
		x2 = (a - b * cos(t)) / xmax;
		y1 = (a * tan(t) + b * sin(t)) / ymax;
		y2 = (a * tan(t) - b * sin(t)) / ymax;

		glVertex2f(x1, y1);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
		double x1, y1, x2, y2;
		x1 = (a + b * cos(t)) / xmax;
		x2 = (a - b * cos(t)) / xmax;
		y1 = (a * tan(t) + b * sin(t)) / ymax;
		y2 = (a * tan(t) - b * sin(t)) / ymax;

		glVertex2f(x2, y2);
	}
	glEnd();
}

// graficul functiei 
// $f(x) = \bar sin(x) \bar \cdot e^{-sin(x)}, x \in \langle 0, 8 \cdot \pi \rangle$, 
void Display2() {
	double pi = 4 * atan(1.0);
	double xmax = 8 * pi;
	double ymax = exp(1.1);
	double ratia = 0.05;

	// afisarea punctelor propriu-zise precedata de scalare
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (double x = 0; x < xmax; x += ratia) {
		double x1, y1;
		x1 = x / xmax;
		y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;

		glVertex2f(x1, y1);
	}
	glEnd();
}

void Display3()
{
	double x, x1, y;
	double ratia = 0.05;

	// afisarea punctelor propriu-zise precedata de scalare
	glColor3f(1, 0.1, 0.1); // rosu
	glBegin(GL_LINE_STRIP);
	for (x = 0; x <= 100; x += ratia)
	{
		// precizia masinii ca la calcul numeric :-?
		if (fabs(x - 0 )  <= 1/2 )
		{
			x1 = x;
			y = 1 ;
		}
		else {
			// x maxim e 100 , x1 maxim e 1  : impartim la xmax
			// y max : d(x) max e 0.5 , xmin - e 0.5 pe ramura asta ; y maxim 1
			x1 = x /100;
			y = (fabs(x - floorf(x)) / x);
		}
		glVertex2f(x1*0.8, y*0.9);

	}
	glEnd();
}

void Display4() {
	double x, y, t; // cos(pi)=cos(-pi)=-1, sin(pi)=sin(-pi)=0, a=0.3, b=0.2
	double a = 0.3, b = 0.2;
	double ratie = 0.05;
	double pi = 4 * atan(1.0);


	glColor3f(1, 0.1, 0.1); // rosu
	// loop ca sa nu iasa cu spatiu
	glBegin(GL_LINE_LOOP);

	for (t = -pi + ratie; t < pi; t += ratie) {
		x = 2 * (a * cos(t) + b) * cos(t);
		y = 2 * (a * cos(t) + b) * sin(t);
		glVertex2f(x*0.9, y);
	}

	glEnd();
}

void Display5() {
	double x, y, t;
	double a = 0.20;
	double ratie = 0.020;
	const double pi = 4 * atan(1.0);

	for (t = (-pi) / 2 + ratie; t < pi / 2; t += ratie) {
		if (t != (-pi) / 6 && t != pi / 6) {
			x = a / (4 * pow(cos(t), 2) - 3);
			y = (a * tan(t)) / (4 * pow(cos(t), 2) - 3);


			double x2, y2, t2;
			t2 = t + 0.005;
			x2 = a / (4 * pow(cos(t2), 2) - 3);
			y2 = (a * tan(t2)) / (4 * pow(cos(t2), 2) - 3);

			// x max = 0.2 
			// y max 0.2
			if (x < 0.2 && y > 0.2  && x > -0.85 && y < 0.85  ) {
				glColor3f(1, 0.1, 0.1); // rosu
				glBegin(GL_TRIANGLES);
				glVertex2f(-0.95 , 0.95);
				glVertex2f(x , y );
				glVertex2f(x2, y2 );
				glEnd();
			}
		
		}
	}


	glColor3f(0.2, 0.15, 0.88); // albastru

	glBegin(GL_LINE_LOOP);
	//originea
	glVertex2f(-0.95, 0.95);

	t = -(pi) / 2;
	x = a / (4 * pow(cos(t), 2) - 3);
	glVertex2f(x, 0.95);

		for (t = (-pi) / 2 + ratie; t < pi / 2; t += ratie) {
			if (t != (-pi) / 6 && t != pi / 6) {
			x = a / (4 * pow(cos(t), 2) - 3);
			y = (a * tan(t)) / (4 * pow(cos(t), 2) - 3);

				if (x < 0 && y > 0 && x >= -1 && y <= 1) 
				glVertex2f(x, y);
			

			double x2, y2, t2;
			t2 = t + 0.005;
			x2 = a / (4 * pow(cos(t2), 2) - 3);
			y2 = (a * tan(t2)) / (4 * pow(cos(t2), 2) - 3);

			if (x2 < 0 && y2 > 0 && x2 >= -1 && y2 <= 1) {
				glVertex2f(x2, y2);
			}
		
		}
	  }
		//glVertex2f(-0.95, y3);
	glEnd();
}

void Display6() {
	double x, y, t;
	double ratie = 0.05;
	double a = 0.1, b = 0.2;

	glColor3f(1, 0.1, 0.1);
	glBegin(GL_LINE_STRIP);
	for (t = -10; t <= 10; t += ratie) {
		// x max  = 0.1 * t - 0.2 = 0.8
		x = a * t - b * sin(t);
		y = a - b * cos(t);
		x = x * 0.8;
		glVertex2f(x, y);
	}
	glEnd();
}

void Display7() {
	double x, y, t;
	double r = 0.3, R = 0.1;
	double ratie = 0.05;
	const double pi = 4 * atan(1.0);

	glColor3f(1, 0.1, 0.1);
	glBegin(GL_LINE_STRIP);
	for (t = 0; t <= 2 * pi; t += ratie) {
		x = (R + r) * cos((r / R) * t) - r * cos(t + (r / R) * t);
		y = (R + r) * sin((r / R) * t) - r * sin(t + (r / R) * t);
		glVertex2f(x, y);
	}
	glEnd();
}

void Display8() {
	double x, y, t;
	double r = 0.3, R = 0.1;
	double ratie = 0.05;
	const double pi = 4 * atan(1.0);

	glColor3f(1, 0.1, 0.1);
	glBegin(GL_LINE_STRIP);
	for (t = 0; t <= 2 * pi; t += ratie) {
		x = (R - r) * cos((r / R) * t) - r * cos(t - (r / R) * t);
		y = (R - r) * sin((r / R) * t) - r * sin(t - (r / R) * t);
		glVertex2f(x, y);
	}
	glEnd();
}

void Display9() {
	double a = 0.4;
	double ratia = 0.01;
	double pi = 4 * atan(1.0);
	double x, y, t;

	glColor3f(1, 0.1, 0.1);
	glBegin(GL_LINE_STRIP);
	for (t = -pi / 4; t < pi / 4; t += ratia) {
		x = a * sqrt(2 * cos(2 * t)) * cos(t);
		y = a * sqrt(2 * cos(2 * t)) * sin(t);

		glVertex2f(x, y);
	}

	for (t = -pi / 4 - ratia; t < pi / 4; t += ratia) {

		x = -a * sqrt(2 * cos(2 * t)) * cos(t);
		y = -a * sqrt(2 * cos(2 * t)) * sin(t);

		glVertex2f(x, y);
	}
	glEnd();
}


void Display10() {
	double x, y, t;
	double a = 0.02;
	double ratie = 0.05;
	const double pi = 4 * atan(1.0);

	double xmax = -1000;
	double ymax = -1000;


	glColor3f(1, 0.1, 0.1);
	glBegin(GL_LINE_STRIP);
	for (t = 0; t <= pi; t += ratie) {
		x = a * exp(1 + t) * cos(t);
		y = a * exp(1 + t) * sin(t);
		glVertex2f(x*0.75, y);
	}
	glEnd();
}


void Init(void) {

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glLineWidth(1);

	//   glPointSize(4);

	glPolygonMode(GL_FRONT, GL_LINE);
}
void Display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	switch (prevKey) {
	case '1':
		Display1();
		break;
	case '2':
		Display2();
		break;
	case '3':
		Display3();
		break;
	case '4':
		Display4();
		break;
	case '5':
		Display5();
		break;
	case '6':
		Display6();
		break;
	case '7':
		Display7();
		break;
	case '8':
		Display8();
		break;
	case '9':
		Display9();
		break;
	case '0':
		Display10();
		break;
	default:
		break;
	}

	glFlush();
}

void Reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
	prevKey = key;
	if (key == 27) // escape
		exit(0);
	glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitWindowSize(dim, dim);

	glutInitWindowPosition(100, 100);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutCreateWindow(argv[0]);

	Init();

	glutReshapeFunc(Reshape);

	glutKeyboardFunc(KeyboardFunc);

	glutMouseFunc(MouseFunc);

	glutDisplayFunc(Display);

	glutMainLoop();

	return 0;
}

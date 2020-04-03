#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>

#include "glut.h"

// dimensiunea ferestrei in pixeli
#define dim 300
// numarul maxim de iteratii pentru testarea apartenentei la mult.Julia-Fatou
#define NRITER_MD 5000
// modulul maxim pentru testarea apartenentei la mult.Julia-Fatou
#define MODMAX_MD 2
// ratii ptr. Mandelbrot
#define RX_MD 0.005
#define RY_MD 0.005

unsigned char prevKey;

class CComplex {
public:
	CComplex() : re(0.0), im(0.0) {}

	CComplex(double re1, double im1) : re(re1 * 1.0), im(im1 * 1.0) {}

	CComplex(const CComplex& c) : re(c.re), im(c.im) {}

	~CComplex() {}

	CComplex& operator =(const CComplex& c) {
		re = c.re;
		im = c.im;
		return *this;
	}

	double getRe() {
		return re;
	}

	void setRe(double re1) {
		re = re1;
	}

	double getIm() {
		return im;
	}

	void setIm(double im1) {
		im = im1;
	}

	double getModul() {
		return sqrt(re * re + im * im);
	}

	int operator ==(CComplex& c1) {
		return ((re == c1.re) && (im == c1.im));
	}

	CComplex pow2() {
		CComplex rez;
		rez.re = powl(re * 1.0, 2) - powl(im * 1.0, 2);
		rez.im = 2.0 * re * im;
		return rez;
	}

	friend CComplex operator+(const CComplex& c1, const CComplex& c2);
	friend CComplex operator*(CComplex& c1, CComplex& c2);

	void print(FILE* f) {
		fprintf(f, "%.20f%+.20f i", re, im);
	}

private:
	double re, im;
};

CComplex operator +(const CComplex& c1, const CComplex& c2)
{
	CComplex rez(c1.re + c2.re, c1.im + c2.im);
	return rez;
}

CComplex operator *(CComplex& c1, CComplex& c2)
{
	CComplex rez(c1.re * c2.re - c1.im * c2.im,
		c1.re * c2.im + c1.im * c2.re);
	return rez;
}

class Mandelbrot {
public:
	Mandelbrot()
	{
		m.nriter = NRITER_MD;
		m.modmax = MODMAX_MD;
	}

	~Mandelbrot() {}

	void setmodmax(double v) {
		assert(v <= MODMAX_MD);
		m.modmax = v;
	}

	double getmodmax() {
		return m.modmax;
	}

	void setnriter(int v) {
		assert(v <= NRITER_MD);
		m.nriter = v;
	}

	int getnriter() {
		return m.nriter;
	}

	int isIn(CComplex& x)
	{
		CComplex z;
		int rez = 0;
		for (int i = 0; i < m.nriter; i++) {
			z = z * z + x;
			if (z.getModul() > 2) {
				rez = i + 1;
				break;
			}
		}
		return rez;
	}

	void display(double xmin, double ymin, double xmax, double ymax)
	{
		glPushMatrix();
		glLoadIdentity();
		glBegin(GL_POINTS);

		for (double x = xmin; x <= xmax; x += RX_MD) {
			for (double y = ymin; y <= ymax; y += RY_MD) {
				CComplex z(x, y);
				int r = isIn(z);
				if (r == 0) {
					glColor3f(0.0, 0.0, 0.0);
					glVertex3d(x / 2.0, y / 2.0, 0);
				}
				else {
					glColor3f(r * 0.1, r * 0.5, r * 0.1);
					glVertex3d(x / 2.0, y / 2.0, 0);
				}
			}
		}

		glEnd();
		glPopMatrix();
	}

private:
	struct SDate {
		CComplex c;
		int nriter;
		double modmax;
	} m;
};

class C2coord {
public:
	C2coord() {
		m.x = m.y = 0;
	}

	C2coord(double x, double y) {
		m.x = x;
		m.y = y;
	}

	C2coord(const C2coord& p) {
		m.x = p.m.x;
		m.y = p.m.y;
	}

	C2coord& operator =(C2coord& p) {
		m.x = p.m.x;
		m.y = p.m.y;
		return *this;
	}

	int operator ==(C2coord& p) {
		return ((m.x == p.m.x) && (m.y == p.m.y));
	}

protected:
	struct SDate
	{
		double x, y;
	} m;
};

class CPunct : public C2coord {
public:

	CPunct() : C2coord(0.0, 0.0) {}

	CPunct(double x, double y) : C2coord(x, y) {}

	CPunct& operator =(const CPunct& p) {
		m.x = p.m.x;
		m.y = p.m.y;
		return *this;
	}

	void getxy(double& x, double& y) {
		x = m.x;
		y = m.y;
	}

	int operator ==(CPunct& p) {
		return ((m.x == p.m.x) && (m.y == p.m.y));
	}

	void marcheaza() {
		glBegin(GL_POINTS);
		glVertex2d(m.x, m.y);
		glEnd();
	}

	void print(FILE* fis) {
		fprintf(fis, "(%+f,%+f)", m.x, m.y);
	}

};

class CVector : public C2coord {
public:
	CVector() : C2coord(0.0, 0.0) {
		normalizare();
	}

	CVector(double x, double y) : C2coord(x, y) {
		normalizare();
	}

	CVector& operator =(CVector& p) {
		m.x = p.m.x;
		m.y = p.m.y;
		return *this;
	}

	int operator ==(CVector& p) {
		return ((m.x == p.m.x) && (m.y == p.m.y));
	}

	CPunct getDest(CPunct& orig, double lungime) {
		double x, y;
		orig.getxy(x, y);
		CPunct p(x + m.x * lungime, y + m.y * lungime);
		return p;
	}

	void rotatie(double grade) {
		double x = m.x;
		double y = m.y;
		double t = 2 * (4.0 * atan(1.0)) * grade / 360.0;
		m.x = x * cos(t) - y * sin(t);
		m.y = x * sin(t) + y * cos(t);
		normalizare();
	}

	void deseneaza(CPunct p, double lungime) {
		double x, y;
		p.getxy(x, y);
		glColor3f(1.0, 0.1, 0.1);
		glBegin(GL_LINE_STRIP);
		glVertex2d(x, y);
		glVertex2d(x + m.x * lungime, y + m.y * lungime);
		glEnd();
	}

	void print(FILE* fis) {
		fprintf(fis, "%+fi %+fj", C2coord::m.x, C2coord::m.y);
	}

private:
	void normalizare() {
		double d = sqrt(C2coord::m.x * C2coord::m.x + C2coord::m.y * C2coord::m.y);
		if (d != 0.0) {
			C2coord::m.x = C2coord::m.x * 1.0 / d;
			C2coord::m.y = C2coord::m.y * 1.0 / d;
		}
	}
};

class Imagine1 {
public:
	static int nivel;

	void deseneaza(CPunct colt, CVector vec, double lungime, int nivel) {
		if (nivel == -1) {
			return;
		}

		else {

			double x, y;
			colt.getxy(x, y);

			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++) {
					if (i == 1 && j == 1)
						continue;
					CPunct p(x + i * lungime / 3.0, y + j * lungime / 3.0);
					deseneaza(p, vec, lungime / 3.0, nivel - 1);
				}
			deseneaza_single(colt, vec, lungime);
		}
	}

	void afisare(double lungime, int nivel) {
		CPunct colt(-lungime / 2.0, -lungime / 2.0);
		CVector vec(1.0, 0.0);

		//desenez patrat exterior mare
		for (int i = 1; i <= 4; ++i)
		{
			vec.deseneaza(colt, lungime);
			colt = vec.getDest(colt, lungime);
			vec.rotatie(90);
		}

		//desenez celelalte nivele
		deseneaza(colt, vec, lungime, nivel);
	}

	void deseneaza_single(CPunct colt, CVector vec, double lungime) {
		double x, y;
		colt.getxy(x, y);
		CPunct aux(x + lungime / 3.0, y + lungime / 3.0);

		for (int i = 1; i <= 4; ++i) {
			vec.deseneaza(aux, lungime / 3.0);
			aux = vec.getDest(aux, lungime / 3.0);
			vec.rotatie(90);
		}
	}
};
int Imagine1::nivel = 0;

class Imagine2 {
public:
	static int nivel;

	void deseneaza(CPunct p, CVector vec, double lungime, int nivel) {
		if (nivel == -1) {
			return;
		}

		else {
			vec.deseneaza(p, lungime);
			p = vec.getDest(p, lungime);

			vec.rotatie(-45);
			vec.deseneaza(p, lungime * 3.0);

			CPunct p1 = vec.getDest(p, lungime * 3.0);
			deseneaza(p1, vec, lungime / 3.0, nivel - 1);

			vec.rotatie(90);
			vec.deseneaza(p, lungime * 3.0);
			p = vec.getDest(p, lungime * 3.0);
			vec.rotatie(15);
			vec.deseneaza(p, lungime * 3.0);

			p1 = vec.getDest(p, lungime * 3.0);
			deseneaza(p1, vec, lungime / 3.0, nivel - 1);

			vec.rotatie(-60);
			vec.deseneaza(p, lungime * 3.0);
			p = vec.getDest(p, lungime * 3.0);
			vec.rotatie(30);
			vec.deseneaza(p, lungime * 1.5);

			p1 = vec.getDest(p, lungime * 1.5);
			deseneaza(p1, vec, lungime / 3.0, nivel - 1);

			vec.rotatie(-120);
			vec.deseneaza(p, lungime * 1.5);

			p1 = vec.getDest(p, lungime * 1.5);
			deseneaza(p1, vec, lungime / 3.0, nivel - 1);
		}

	}

	void afisare(double lungime, double nivel) {
		CPunct st(-1.0 / 3.0, 1);
		CVector vec(0, -1.0);
		deseneaza(st, vec, lungime, nivel);
	}

};
int Imagine2::nivel = 0;

class Imagine3 {
public:
	static int nivel;

	void drawer(int rotatie, CPunct& starting_point, CVector& starting_vector, double lungime) {
		starting_vector.rotatie(rotatie);
		starting_vector.deseneaza(starting_point, lungime);
	}

	void afisare(double lungime, int nivel)
	{
		CVector starting_vector(0.0, -1.0);
		CPunct starting_point(-0.25, 0.5);

		drawImag3(lungime, nivel, starting_point, starting_vector, 1);
	}

	void drawImag3(double lungime, int nivel, CPunct& starting_point, CVector starting_vector, int sw)
	{
		if (nivel == 0) {
			starting_vector.deseneaza(starting_point, lungime * 2);
		}

		else if (nivel == 1)
		{
			double firstRotation = sw * 60;
			double secondRotation = -sw * 60;

			drawer(firstRotation, starting_point, starting_vector, lungime);

			starting_point = starting_vector.getDest(starting_point, lungime);

			drawer(secondRotation, starting_point, starting_vector, lungime);

			starting_point = starting_vector.getDest(starting_point, lungime);

			drawer(secondRotation, starting_point, starting_vector, lungime);

			starting_point = starting_vector.getDest(starting_point, lungime);
		}
		else
		{
			double firstRotation = sw * 60;
			double secondRotation = -sw * 60;

			starting_vector.rotatie(firstRotation);
			drawImag3(lungime / 2, nivel - 1, starting_point, starting_vector, -sw);
			starting_vector.rotatie(secondRotation);
			drawImag3(lungime / 2, nivel - 1, starting_point, starting_vector, sw);
			starting_vector.rotatie(secondRotation);
			drawImag3(lungime / 2, nivel - 1, starting_point, starting_vector, -sw);
		}
	}
};
int Imagine3::nivel = 0;

void Display1() {
	Mandelbrot md;;
	md.setnriter(30);
	md.display(-2, -2, 2, 2);
}
void Display2() {
	Imagine1 firstImg;
	firstImg.afisare(1, Imagine1::nivel);
	Imagine1::nivel++;
}
void Display3() {
	Imagine2 secondImg;
	secondImg.afisare(0.1, Imagine2::nivel);
	printf("%d", Imagine2::nivel);
	Imagine2::nivel++;
}

void Display4() {
	Imagine3 secondImg;
	secondImg.afisare(0.1, Imagine3::nivel);
	printf("%d", Imagine3::nivel); 
	Imagine3::nivel++;

}

void Init(void) {

	glClearColor(1.0, 1.0, 1.0, 1.0);

	glLineWidth(1);

	//   glPointSize(3);

	glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
	switch (prevKey) {
	case '1':
		glClear(GL_COLOR_BUFFER_BIT);
		Display1();
		Imagine3::nivel = 0;
		Imagine2::nivel = 0;
		Imagine1::nivel = 0;
		break;
	case'2':
		glClear(GL_COLOR_BUFFER_BIT);
		Display2();
		Imagine3::nivel = 0;
		Imagine2::nivel = 0;
		break;
	case'3':
		glClear(GL_COLOR_BUFFER_BIT);
		Display3();
		Imagine3::nivel = 0;
		Imagine1::nivel = 0;
		break;
	case'4':
		glClear(GL_COLOR_BUFFER_BIT);
		Display4();
		Imagine2::nivel = 0;
		Imagine1::nivel = 0;
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
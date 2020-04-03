#include "gpc3.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <glut.h>

// dimensiunea ferestrei in pixeli
#define dim 500
#define PI 3.1415926535897932384626433832795


unsigned char prevKey;

class GrilaCarteziana
{
    float distx, disty;
    float xOrigine = -0.9;
    float yOrigine = -0.9;
    int linii, coloane;

public:
    GrilaCarteziana(int liniiArg, int ColsArg) { deseneazaGrila(liniiArg, ColsArg); }

    void deseneazaGrila(int liniiA, int coloaneA)
    {
        linii = liniiA;
        coloane = coloaneA;
        distx = 1.8 / float(coloane);
        disty = 1.8 / float(linii);
        float xC = xOrigine, yC = yOrigine;

        glColor3f(0, 0, 0); // rosu

        for (int i = 0; i <= coloane; i++)
        {
            glBegin(GL_LINES);
            glVertex2d(xC, -0.9);
            glVertex2d(xC, 0.9);
            xC += distx;
            glEnd();
        }


        for (int j = 0; j <= linii; j++)
        {
            glBegin(GL_LINES);
            glVertex2d(-0.9, yC);
            glVertex2d(0.9, yC);
            yC += disty;
            glEnd();
        }

        glFlush();

    }

    bool writePixel(int i, int j)
    {
        if (i<0 || j<0 || i > linii || j> coloane)
            return false;
        float CoordPixelx = xOrigine + float(i) * distx;
        float CoordPixely = yOrigine + float(j) * disty;


        DeseneazaPixelRotund(CoordPixelx, CoordPixely);
        return true;
    }

    void DeseneazaPixelRotund(float CoordPixelx, float CoordPixely)
    {

        const int sides = 20;

        glColor3f(0.27, 0.45, 0.44);
        glBegin(GL_LINE_LOOP);

        for (float radius = 0; radius < 0.035; radius += 0.0000005)
            for (int a = 0; a < 360; a += 360 / sides)
            {
                float heading = a * PI / 90;
                glVertex2f(CoordPixelx + cos(heading) * radius, CoordPixely + sin(heading) * radius);
            }

        glEnd();

        glFlush();
    }

    void AfisareSegmentDreapta3(float x0, float y0, float xn, float yn)
    {
        if (x0 > xn) // porneasca de la stanga catre dreapta
        {
            float temp = x0;
            x0 = xn;
            xn = temp;
        }
        float coordInceputx = xOrigine + x0 * distx;
        float coordInceputy = yOrigine + y0 * disty;
        float coordFinalx = xOrigine + xn * distx;
        float coordFinaly = yOrigine + yn * disty;

        glColor3f(1, 0, 0);
        glBegin(GL_LINES);
        glVertex2f(coordInceputx, coordInceputy);
        glVertex2f(coordFinalx, coordFinaly);
        glEnd();

        float dy,dx;

        if (xn > x0)
            dx = xn - x0;
        else
            dx = x0 - xn;
   
        if ( yn > y0)
         dy = yn - y0;
        else  
         dy = y0 - yn;

        float d = 2 * dy - dx;
        float dE = 2 * dy;
        float dNE = 2 * (dy - dx);
        float x = x0, y = y0;
        writePixel(x0, y0);

        if (y0 <= yn) // de jos in sus

        {
            while (x < xn)
            {
                if (d <= 0)
                {
                    printf("Alegem E\n");
                    /* alegem E */
                    d += dE;
                    x = x + 1;

                }
                else
                {
                    printf("Alegem N-E\n");
                    /* alegem NE */
                    d += dNE;
                    x++;
                    y++;

                }

                writePixel(x, y);
            }
        }
        else // de sus in jos
        {
            writePixel(x, y + 1);
            writePixel(x, y - 1);

            while (x < xn)
            {
                if (d <= 0)
                {
                    printf("Alegem E\n");
                    /* alegem E */
                    d += dE;
                    x = x + 1;

                }
                else
                {
                    printf("Alegem S-E\n");
                    /* alegem SE */
                    d += dNE;
                    x++;
                    y--;

                }

                writePixel(x, y);
                writePixel(x, y+1);
                writePixel(x, y-1);


            }
        }
    }
};

void Display1() {
    GrilaCarteziana gC = GrilaCarteziana(15,15);
    gC.AfisareSegmentDreapta3(0, 0, 15, 7);
    gC.AfisareSegmentDreapta3(0,15,15,10);


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
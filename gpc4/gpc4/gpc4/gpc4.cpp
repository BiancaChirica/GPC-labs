#include "gpc4.h"

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
        glLineWidth(1);

        linii = liniiA;
        coloane = coloaneA;
        distx = 1.8 / float(coloane);
        disty = 1.8 / float(linii);
        float xC = xOrigine, yC = yOrigine;

        glColor3f(0, 0, 0); 

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
        // linia i si coloana j 
        if (i<0 || j<0 || i > linii || j> coloane)
            return false;
        float CoordPixelx = xOrigine + float(i) * distx;
        float CoordPixely = yOrigine + float(j) * disty;


        DeseneazaPixelRotund(CoordPixelx, CoordPixely);
        return true;
    }

    void DeseneazaPixelRotund(float CoordPixelx, float CoordPixely)
    {
        // deseneaza pixel dupa coordonatele x si y
        const int sides = 20;

        glColor3f(0.27, 0.45, 0.44);
        glBegin(GL_LINE_LOOP);

        float razaPixel = 0;
        if (linii < 20)
            razaPixel = 0.035;
        else
            razaPixel = 1.0/ float(linii + 10) ;


        for (float radius = 0; radius < razaPixel; radius += 0.0000005)
            for (int a = 0; a < 360; a += 360 / sides)
            {
                float heading = a * PI / 90;
                glVertex2f(CoordPixelx + cos(heading) * radius, CoordPixely + sin(heading) * radius);
            }

        glEnd();

        glFlush();
    }

    void DeseneazaCercRosu(float radius)
    {
           const int sides = 70;

            glColor3f(1, 0, 0);
            glLineWidth(3);
            glBegin(GL_LINE_LOOP);

            
                for (int a = 0; a < 360; a += 360 / sides)
                {
                    float heading = a * PI / 90;
                    glVertex2f( cos(heading) * radius, sin(heading) * radius);
                }

            glEnd();

            glFlush();
            glLineWidth(1);
        
    }

    void AfisareCerc4(float raza)
    {
        raza = raza * disty;


        DeseneazaCercRosu(raza);

        // deseneaza un punct in mijloc pentru a vedea mai bine mijlocul
        float xMij = xOrigine + (linii / 2 )* distx;
        float yMij = yOrigine + (coloane) / 2 * disty;
        DeseneazaPixelRotund(xMij, yMij);

        
            float x = xMij + raza, y = yMij;
            float d = 1*distx - raza;
            float dN = 3*distx, dNE = -2 * raza + 5*disty;

    
            AfisarePuncteCerc3(x, y);
      

            while (x  > y)
            {
                if (d > 0)
                {
                    printf("NE ");
                    d += dNE;
                    dNE += 4*disty;
                    dN += 2*distx;
                    x = x - distx;
                   
                }
                else
                {
                    printf("N ");
                    d += dN;
                    dNE += 2*disty;
                    dN += 2*disty;
                 
                }
                y = y + disty;
               
                AfisarePuncteCerc3(x, y);
            }
    }


    void AfisarePuncteCerc3(float x , float y)
    {
        DeseneazaPixelRotund(x, y);
        DeseneazaPixelRotund(x - distx, y);
        DeseneazaPixelRotund(x + distx, y);
               
    }


    void elipsa(int x0, int y0, int x, int y) {
        glColor3f(1, 0, 0);
        glLineWidth(3);
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 360; i += 10) {
            double unghi = i * PI / 180;
            glVertex2d(xOrigine + (cos(unghi) * x + x0) * distx, xOrigine + (sin(unghi) * y + y0) * distx);
        }
        glLineWidth(3);
        glEnd();
    }
    void AfisarePuncteElipsa(int x0, int y0, int x, int y)
    {
        for (int i = x0; i >= x0 - x; i--) {
            writePixel(i, y0 - y);
        }
    }
    void AfisareElipsa(int x0, int y0, int a, int b)
    {
        int x = 0, y = b;
        double fxpyp = 0.0;
        double deltaE, deltaSE, deltaS;
        AfisarePuncteElipsa(x0, y0, x, y);

        //regiunea 1
        while (a * a * (y - 0.5) > b* b* (x + 1)) {
            deltaE = b * b * (2 * x + 1);
            deltaSE = b * b * (2 * x + 1) + a * a * (-2 * y + 1);
            if (fxpyp + deltaE <= 0.0) {
                // E este in interior
                fxpyp += deltaE;
                x++;
                AfisarePuncteElipsa(x0, y0, x, y);
            }
            else if (fxpyp + deltaSE <= 0.0) {
                // Se este in interior
                fxpyp += deltaSE;
                x++; y--;
                AfisarePuncteElipsa(x0, y0, x, y);
            }
        }

        //regiunea 2
        while (y > 0) {
            deltaSE = b * b * (2 * x + 1) + a * a * (-2 * y + 1);
            deltaS = a * a * (-2 * y + 1);
            if (fxpyp + deltaSE <= 0.0) {
                // SE este in interior
                fxpyp += deltaSE;
                x++; y--;
            }
            else {
                // S este in interior
                fxpyp += deltaS;
                y--;
            }
            AfisarePuncteElipsa(x0, y0, x, y);
        }
    }

};


void Display1() {
    GrilaCarteziana gC = GrilaCarteziana(30, 30);
    gC.AfisareCerc4(13);  // raza 13 celule din grila

}

void Display2() {
    GrilaCarteziana gC = GrilaCarteziana(26, 26);
    gC.elipsa(13, 7, 13, 7);
    gC.AfisareElipsa(13, 7, 13, 7);


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
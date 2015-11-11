#ifdef _WIN32
//define something for Windows (32-bit and 64-bit, this part is common)
   #ifdef _WIN64
      //define something for Windows (64-bit only)
   #endif
#elif __APPLE__
    #include "TargetConditionals.h"
    #include <GLUT/glut.h>
    #if TARGET_IPHONE_SIMULATOR
    // iOS Simulator
    #elif TARGET_OS_IPHONE
    // iOS device
    #elif TARGET_OS_MAC
    // Other kinds of Mac OS
    #else
    #   error "Unknown Apple platform"
    #endif
#elif __linux__
// linux
#include <GL/glut.h>
#elif __unix__ // all unices not caught above
    // Unix
#elif defined(_POSIX_VERSION)
    // POSIX
#else
#   error "Unknown compiler"
#endif

#include <iostream>
#include <cmath>

#define SIZE(x) sizeof(x)/sizeof(Point*)

using namespace std;

class Point
{
protected:
    float x, y;

    void drawCircle(float cx, float cy, float r, int n)
    {
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < n; ++i)
        {
            float theta = 2.0f * 3.1415926f * float(i) / float(n);
            float x = r * cosf(theta);
            float y = r * sinf(theta);
            glVertex2f(x + cx, y + cy);
        }
        glEnd();
    }

public:
    Point(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    void setCoords(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    float *getCoords()
    {
        float *coords = (float*) malloc(2);
        coords[0] = this->x;
        coords[1] = this->y;
        return coords;
    }

    bool checkBoundaries(float x, float y, int w, int h)
    {
        return x > 0 && x < w && y > 0 && y < h;
    }

    void draw(int mode = 0)
    {
        switch (mode)
        {
            case 0:
                glColor3f(1.0f, 0.0f, 0.0f);
                break;
            case 1:
                glColor3f(0.0f, 1.0f, 0.0f);
                break;
            default:
                break;
        }
        Point::drawCircle(this->x, this->y, 2, 4);
    }
};

class Line
{
    Point *p1, *p2;
    float a, b, c;

public:
    Line(Point *p1, Point *p2)
    {
        this->p1 = p1;
        this->p2 = p2;
    }

    virtual ~Line()
    {
        delete p1, p2;
    }

    void setPoints(Point *p1, Point *p2)
    {
        this->p1 = p1;
        this->p2 = p2;
    }

    Point **getPoints()
    {
        Point **points = (Point**) malloc(2);
        points[0] = this->p1;
        points[1] = this->p2;
        return points;
    }

    void calculateCoefs()
    {
        this->a = this->p1->getCoords()[1] - this->p2->getCoords()[1];
        this->b = this->p2->getCoords()[0] - this->p1->getCoords()[0];
        this->c = (-1.0f) * ((this->p1->getCoords()[0] * this->p2->getCoords()[1]) -
                             (this->p2->getCoords()[0] * this->p1->getCoords()[1]));
    }

    float *getCoefs()
    {
        float *coefs = (float*) malloc(3);
        coefs[0] = this->a;
        coefs[1] = this->b;
        coefs[2] = this->c;
        return coefs;
    }

    void draw()
    {
        glColor3f(1.0f,0.0f, 0.0f);
        glBegin(GL_LINES);
        glVertex2fv(this->p1->getCoords());
        glVertex2fv(this->p2->getCoords());
        glEnd();
    }
};

class Interception : public Point
{
public:
    Interception(Line *l1, Line *l2) : Point(-1, -1)
    {
        this->calcInterception(l1, l2);
    }

    void calcInterception(Line *l1, Line *l2)
    {
        float d, dx, dy;
        l1->calculateCoefs();
        l2->calculateCoefs();

        d = (l1->getCoefs()[0] * l2->getCoefs()[1]) - (l1->getCoefs()[1] * l2->getCoefs()[0]);
        dx = (l1->getCoefs()[2] * l2->getCoefs()[1]) - (l1->getCoefs()[1] * l2->getCoefs()[2]);
        dy = (l1->getCoefs()[0] * l2->getCoefs()[2]) - (l1->getCoefs()[2] * l2->getCoefs()[0]);

        if (d != 0.0f)
        {
            Point::x = abs(dx / d);
            Point::y = abs(dy / d);
        }
    }

    // counter clockwise funtion
    bool ccw(Point *p1, Point *p2, Point *p3)
    {
        return (p3->getCoords()[1] - p1->getCoords()[1]) * (p2->getCoords()[0] - p1->getCoords()[0]) >
               (p2->getCoords()[1] - p1->getCoords()[1]) * (p3->getCoords()[0] - p1->getCoords()[0]);
    }

    // checks if the the interception exists
    bool checkExistance(Point **points)
    {
        return ccw(points[0], points[2], points[3]) != ccw(points[1], points[2], points[3]) &&
               ccw(points[0], points[1], points[2]) != ccw(points[0], points[1], points[3]);
    }

};

static int width, height;

static Point *p1 = new Point(100,200);
static Point *p2 = new Point(500,200);
static Point *p3 = new Point(400,300);
static Point *p4 = new Point(400,100);
static Line *l1 = new Line(p1, p2);
static Line *l2 = new Line(p3, p4);
static Interception *i = new Interception(l1, l2);

static Point *points[4] = {p1, p2, p3, p4};
static Point *sel;

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    p1->draw();
    p2->draw();
    p3->draw();
    p4->draw();
    l1->draw();
    l2->draw();
    if (i->checkExistance(points))
        i->draw(1);     // 1 means that this is the interception (different color)
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    width = w;
    height = h;

    glViewport (0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
    glMatrixMode(GL_MODELVIEW);
    glutPostRedisplay();
}

void timer(int id)
{
    glutPostRedisplay();
    glutTimerFunc(id, timer, 0);
}

void keyboard(unsigned char c, int x, int y)
{
    switch (c)
    {
        case 27:
            delete p1, p2, p3, p4, l1, l2, i, points, sel;
            exit(0);
        default:
            break;
    }
}

float dist(Point *p1, Point *p2)
{
    float dx = powf(p2->getCoords()[0] - p1->getCoords()[0], 2);
    float dy = powf(p2->getCoords()[1] - p1->getCoords()[1], 2);
    return sqrt(abs(dx - dy));
}

void mouse(int b, int s, int x, int y)
{
    switch (b)
    {
        case GLUT_LEFT_BUTTON:
            Point *temp = new Point(x,y);
            if (s == GLUT_DOWN)
            {
                for (int i = 0; i < SIZE(points); ++i)
                {
                    if (dist(points[i], temp) <= 5.0f)
                    {
                        sel = points[i];
                        break;
                    }
                }
            }
            else if (s == GLUT_UP)
            {
                sel = NULL;
                delete temp;
            }
            break;
    }
}

void motion(int x, int y)
{
    if (sel != NULL && sel->checkBoundaries(x, y, width, height))
    {
        sel->setCoords(x, y);
        i->calcInterception(l1, l2);
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(600, 400);
    glutInitWindowPosition(0, 0);
    glutCreateWindow(argv[0]);

    glutDisplayFunc(draw);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(motion);
    glutTimerFunc(500, timer, 0);

    glutMainLoop();

    return 0;
}
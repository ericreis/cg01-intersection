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


/*
 * Macro SIZE(v): returns the size of a given vector of points
 */
#define SIZE(v) sizeof(v)/sizeof(Point*)

using namespace std;

/*
 * Class Point: used to store important attributes of a point and perform
 * operation with them. All methods used to draw, set/get coordinates and
 * checking are done here.
 */
class Point
{
protected:
    float x, y;

    /*
     * Function that draws a circle on screen.
     * Given the position cx, cy on screen, it draws a "circle" of radius r
     * using a set of n lines.
     */
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

    /*
     * Constructor for the class Point.
     * Sets the initial value of the point's coordinates.
     */
    Point(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    /*
     * Method that sets the coordinates to a new value.
     */
    void setCoords(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    /*
     * Method that gets the coordinates of the point.
     * Returns a vector of points (float*)
     */
    float *getCoords()
    {
        float *coords = (float*) malloc(2);
        coords[0] = this->x;
        coords[1] = this->y;
        return coords;
    }

    /*
     * Method used to check whether the point is in the screen or not.
     * This is used to don't let the point be out of screen, even if the
     * cursor is.
     */
    bool checkBoundaries(float x, float y, int w, int h)
    {
        return x > 0 && x < w && y > 0 && y < h;
    }

    /*
     * Method used to make the drawing process of the point using OpenGL.
     * The mode parameter is used to indicate the state of the point:
     * 0 is a normal point
     * 1 is the intersection point
     */
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

/*
 * Class Line: used to store important attributes of a line and perform
 * operation with them. All methods used to draw, set/get points and
 * calculations are done here.
 */
class Line
{
    Point *p1, *p2;
    float a, b, c;

public:

    /*
     * Constructor for the class Line.
     * Sets the initial points used to define the line.
     */
    Line(Point *p1, Point *p2)
    {
        this->p1 = p1;
        this->p2 = p2;
    }

    /*
     * Destructor of the class Line.
     * Destroys the pointers used in the instance since they'll
     * not have any more use. This prevents memory leak.
     */
    virtual ~Line()
    {
        delete p1, p2;
    }

    /*
     * Method that sets new points to the line.
     */
    void setPoints(Point *p1, Point *p2)
    {
        this->p1 = p1;
        this->p2 = p2;
    }

    /*
     * Method that gets the points used in the line.
     * Returns a vector of pointers to points (float**)
     */
    Point **getPoints()
    {
        Point **points = (Point**) malloc(2);
        points[0] = this->p1;
        points[1] = this->p2;
        return points;
    }

    /*
     * Method used to calculated the coefficients of the line's equation.
     * General Equation of a straight line is given by:
     * Ax + By + C = 0
     * This method sets those A, B and C coefficients.
     */
    void calculateCoeffs()
    {
        this->a = this->p1->getCoords()[1] - this->p2->getCoords()[1];
        this->b = this->p2->getCoords()[0] - this->p1->getCoords()[0];
        this->c = (-1.0f) * ((this->p1->getCoords()[0] * this->p2->getCoords()[1]) -
                             (this->p2->getCoords()[0] * this->p1->getCoords()[1]));
    }

    /*
     * Method that gets the coefficients of the equation.
     * Returns a vector of coefficients (float*)
     */
    float *getCoeffs()
    {
        float *coeffs = (float*) malloc(3);
        coeffs[0] = this->a;
        coeffs[1] = this->b;
        coeffs[2] = this->c;
        return coeffs;
    }

    /*
     * Method used to make the drawing process of the line using OpenGL.
     * We use the points of the line to delimiter it.
     */
    void draw()
    {
        glColor3f(1.0f,0.0f, 0.0f);
        glBegin(GL_LINES);
        glVertex2fv(this->p1->getCoords());
        glVertex2fv(this->p2->getCoords());
        glEnd();
    }
};

/*
 * Class Interception: inherits all the methods from Point.
 * Adds the method to calculate th intersection of two given lines
 */
class Interception : public Point
{
public:

    /*
     * Constructor of the class Interception.
     * It uses the constructor of point as the base constructot.
     */
    Interception(Line *l1, Line *l2) : Point(-1, -1)
    {
        this->calcInterception(l1, l2);
    }

    /*
     * Method used to calculate the interception of two given lines.
     * Those lines are passed as parameters.
     * We use Cramer's Rule to calculate the intersection.
     * Since we have a linear system of the form:
     * A1x + B1y + C1 = 0   ->  |A1 B1| |x| = |-C1|
     * A2x + B2y + C2 = 0       |A2 B2| |y|   |-C2|
     * By the Cramer's Rule we can affirm that:
     * x = (C1B2 - B1C2) / (A1B2 - B1A2)
     * y = (A1C2 - C1A2) / (A1B2 - B1A2)
     */
    void calcInterception(Line *l1, Line *l2)
    {
        float d, dx, dy;
        l1->calculateCoeffs();
        l2->calculateCoeffs();

        d = (l1->getCoeffs()[0] * l2->getCoeffs()[1]) - (l1->getCoeffs()[1] * l2->getCoeffs()[0]);
        dx = (l1->getCoeffs()[2] * l2->getCoeffs()[1]) - (l1->getCoeffs()[1] * l2->getCoeffs()[2]);
        dy = (l1->getCoeffs()[0] * l2->getCoeffs()[2]) - (l1->getCoeffs()[2] * l2->getCoeffs()[0]);

        if (d != 0.0f)
        {
            Point::x = abs(dx / d);
            Point::y = abs(dy / d);
        }
    }

    /*
     * CounterClockWise function.
     * Method used to determine if 3 given points are in clockwise orientation
     */
    bool ccw(Point *p1, Point *p2, Point *p3)
    {
        return (p3->getCoords()[1] - p1->getCoords()[1]) * (p2->getCoords()[0] - p1->getCoords()[0]) >
               (p2->getCoords()[1] - p1->getCoords()[1]) * (p3->getCoords()[0] - p1->getCoords()[0]);
    }

    /*
     * Method used to determine whether the intersection exists or not.
     * It uses the CounterClockWise function 3 points by 3.
     * If they are in clockwise orientation that means the intersection exists.
     */
    bool checkExistance(Point **points)
    {
        return ccw(points[0], points[2], points[3]) != ccw(points[1], points[2], points[3]) &&
               ccw(points[0], points[1], points[2]) != ccw(points[0], points[1], points[3]);
    }

};

/*
 * Static values used to initialize the system.
 * The dimensions of the screen, the points, lines and interception
 * been used, the vector of the used points and a pointer to the
 * points selected by the user with the click.
 */
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

/*
 * Function that treats the OpenGL DisplayCallback.
 * This draws all content to the screen.
 */
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

/*
 * Function that treats the OpenGL ReshapeCallback.
 * This remodel the coordinate system and treats the window dimensions.
 */
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

/*
 * Function that treats the OpenGL TimerCallback.
 * This function is used to make the program continous
 * by calling the glutPostRedisplay().
 */
void timer(int id)
{
    glutPostRedisplay();
    glutTimerFunc(id, timer, 0);
}

/*
 * Function that treats the OpenGL KeyboardCallback.
 * This treats the keyboard input of the user.
 * <Esc> button to quit.
 */
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

/*
 * Function that calculates the distance beetween
 * two given points (Point)
 */
float dist(Point *p1, Point *p2)
{
    float dx = powf(p2->getCoords()[0] - p1->getCoords()[0], 2);
    float dy = powf(p2->getCoords()[1] - p1->getCoords()[1], 2);
    return sqrt(abs(dx - dy));
}

/*
 * Function that treats the OpenGL MouseCallback.
 * This treats the left button click of the user.
 * When the user click near a point it marks that
 * point as the selected point.
 */
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

/*
 * Function that treats the OpenGL MotionCallback.
 * This treats the movement of the user's mouse while
 * it's been pressed.
 * This is used to move the selected point.
 */
void motion(int x, int y)
{
    if (sel != NULL && sel->checkBoundaries(x, y, width, height))
    {
        sel->setCoords(x, y);
        i->calcInterception(l1, l2);
    }
}

/*
 * Main funtion of the program.
 * Sets Up the Window (size, position, title)
 * Sets Up the callbacks that we'll be used.
 */
int main(int argc, char **argv)
{
    glutInit(&argc, argv);                              // initialize glut and gl
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);       // double buffering and RGB
    glutInitWindowSize(600, 400);                       // initial window size
    glutInitWindowPosition(0, 0);                       // initial window position
    glutCreateWindow(argv[0]);                          // create window

    /* BEGIN CALLBACKS */
    glutDisplayFunc(draw);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMotionFunc(motion);
    glutTimerFunc(500, timer, 0);
    /* END CALLBACKS */

    glutMainLoop();                                     // turn control over to glut

    return 0;
}
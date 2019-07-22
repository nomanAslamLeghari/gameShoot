/*********
   CTIS164 - Template Source Program
----------
STUDENT : Noman Aslam
SECTION : 02
HOMEWORK: 01
----------
PROBLEMS:
----------
ADDITIONAL FEATURES: 1)color of moving plane is different from the stationary one
                     2)Color of moving plane when total no. is odd is changing constantly
					 3)A plane can be deleted by putting pointer and pressing the del button on the keyboard
					 4)At the top it shows the record of total planes,moving planes and stationary planes
*********/
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  900
#define WINDOW_HEIGHT 600

#define SIZE 10 //max number of planes

#define TIMER_PERIOD  20 // Period for the timer.
#define TIMER_ON         0 // 0:disable timer, 1:enable timer

#define D2R 0.0174532


//point contains x and y
typedef struct {

	int x, y;

}point;

//plane type contains a point p and the state of that certain plane
typedef struct {
	point p;
	bool state;
}plane_t;


plane_t *plane;//to be used for a dynamic array of planes
point p;



int count = 0,//for counting number of planes
	moving = 0,//for the moving planes
	color = 0;//for changing the color of planes when there are odd of number of planes

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, const char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}





//these checking code for triangle are taken from internet
float areaT(point p1, point p2, point p3) {

	return abs((p1.x*(p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y))) / 2.0;
}

//this method checks if a point lies inside a triangle or not
bool checkTriangle(point p1, point p2, point p3, point p) {

	float AreaT = areaT(p1, p2, p3);
	float A1 = areaT(p, p2, p3);
	float A2 = areaT(p1, p, p3);
	float A3 = areaT(p1, p2, p);

	return AreaT == (A1 + A2 + A3);

}


//area of rectangle
float areaR(point p1, point p2, point p3) {
	return abs((p2.x - p1.x + (p2.y - p1.y))*(p3.x - p2.x + (p3.y - p2.y)));
}


//this function checks if a point lies inside a circle or not
bool checkRectangle(point p1, point p2, point p3, point p4, point p) {
	float area1 = areaR(p1, p2, p3);
	float A1 = areaT(p, p2, p3);
	float A2 = areaT(p1, p, p4);
	float A3 = areaT(p1, p2, p);
	float A4 = areaT(p3, p4, p);

	return area1 == (A1 + A2 + A3 + A4);
}

//checks for the line
bool checkLine(point p1,point p2,point p) {

	return p1.y >= p.y && p2.y <= p.y && p.x == p2.x;

}



//checks if a shape p lies inside my shape
bool checkShape(point p, point pc) {
	point p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18, p19, p20, p21, p22, p23, p24, p25, p26, p27, p28;
	p1.x = pc.x;
	p1.y = pc.y - 15;
	p2.x = pc.x - 80;
	p2.y = pc.y;
	p3.x = pc.x;
	p3.y = pc.y + 15;
	p4.x = pc.x + 10;
	p4.y = pc.y + 85;
	p5.x = pc.x + 30;
	p5.y = pc.y + 85;
	p6.x = pc.x + 30;
	p6.y = pc.y + 15;
	p7.x = pc.x + 30;
	p7.y = pc.y - 15;
	p8.x = pc.x + 30;
	p8.y = pc.y - 85;
	p9.x = pc.x + 10;
	p9.y = pc.y - 85;
	p10.x = pc.x + 70;
	p10.y = pc.y - 15;
	p11.x = pc.x + 70;
	p11.y = pc.y + 15;
	p12.x = pc.x - 60;
	p12.y = pc.y - 4;
	p13.x = pc.x - 65;
	p13.y = pc.y - 35;
	p14.x = pc.x - 80;
	p14.y = pc.y - 35;
	p15.x = pc.x - 80;
	p15.y = pc.y + 35;
	p16.x = pc.x - 65;
	p16.y = pc.y + 35;
	p17.x = pc.x - 60;
	p17.y = pc.y + 4;
	p18.x = pc.x + 10;
	p18.y = pc.y + 15;
	p19.x = pc.x + 10;
	p19.y = pc.y - 15;
	p20.x = pc.x + 70;
	p20.y = pc.y + 5;
	p21.x = pc.x + 70;
	p21.y = pc.y - 5;
	p22.x = pc.x + 80;
	p22.y = pc.y;
	p23.x = pc.x + 75;
	p23.y = pc.y;
	p24.x = pc.x + 75;
	p24.y = pc.y + 15;
	p25.x = pc.x + 75;
	p25.y = pc.y - 15;
	p26.x = pc.x - 65;
	p26.y = pc.y + 3;
	p27.x = pc.x - 65;;
	p27.y = pc.y - 3;
	p28.x = pc.x - 65;
	p28.y = pc.y;

	return checkLine(p25,p24,p)||checkTriangle(p1, p2, p3, p) || checkTriangle(p1, p9, p19, p) || checkRectangle(p19, p9, p8, p7, p) || checkTriangle(p3, p4, p18, p) || checkRectangle(p4, p5, p6, p18, p) || checkRectangle(p3, p1, p10, p11, p) || checkTriangle(p16, p17, p26, p) || checkRectangle(p2, p15, p16, p28, p) || checkTriangle(p12, p13, p27, p) || checkRectangle(p2, p14, p13, p28, p) || checkTriangle(p20, p21, p22, p);

}


//displays the background
void display_background() {
	// label
	glColor3f(0, 0, 0);


	glColor3ub(165, 42, 42);
	glBegin(GL_POLYGON);
	glVertex2f(-250,290);
	glVertex2f(-140,290);
	glVertex2f(-140,240);
	glVertex2f(-250,240);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(-80, 250);
	glVertex2f(80, 250);
	glVertex2f(80, 230);
	glVertex2f(-80, 230);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(120, 300);
	glVertex2f(400, 300);
	glVertex2f(400, 210);
	glVertex2f(120, 210);
	glEnd();


	glLineWidth(2);
	glColor3ub(0,0,0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-250, 290);
	glVertex2f(-140, 290);
	glVertex2f(-140, 240);
	glVertex2f(-250, 240);
	glEnd();

	glColor3ub(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-80, 250);
	glVertex2f(80, 250);
	glVertex2f(80, 230);
	glVertex2f(-80, 230);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(120, 300);
	glVertex2f(400, 300);
	glVertex2f(400, 210);
	glVertex2f(120, 210);
	glEnd();

	glColor3f(0, 0, 0);

	vprint(-240, 270, GLUT_BITMAP_8_BY_13, "Noman Aslam");
	vprint(-240, 250, GLUT_BITMAP_8_BY_13, "21701085");
	vprint(-70, 240, GLUT_BITMAP_9_BY_15, " - Homework-1 -");
	vprint(120, 240, GLUT_BITMAP_9_BY_15, " No of Planes:%d",count);
	vprint(270, 280, GLUT_BITMAP_9_BY_15, " Moving:%d",moving);
	vprint(270, 220, GLUT_BITMAP_9_BY_15, " Not Moving: %d",count - moving);
	vprint(270,-280, GLUT_BITMAP_9_BY_15, "(%d,%d)", p.x,p.y);

}


//draws my shape
void displayShape(int x, int y,int num) {


	//shape 1
	glBegin(GL_TRIANGLES);
	glVertex2f(x, y + 15);
	glVertex2f(x - 80, y);
	glVertex2f(x, y - 15);
	glEnd();


	//shape 2
	glBegin(GL_POLYGON);
	glVertex2f(x - 60, y + 4);
	glVertex2f(x - 65, y + 35);
	glVertex2f(x - 80, y + 35);
	glVertex2f(x - 80, y);
	glEnd();


	//shape 3
	glBegin(GL_POLYGON);
	glVertex2f(x - 60, y - 4);
	glVertex2f(x - 65, y - 35);
	glVertex2f(x - 80, y - 35);
	glVertex2f(x - 80, y);
	glEnd();

	//shape 4
	glBegin(GL_POLYGON);
	glVertex2f(x, y + 15);
	glVertex2f(x + 10, y + 85);
	glVertex2f(x + 30, y + 85);
	glVertex2f(x + 30, y + 15);
	glEnd();


	//shape 5
	glBegin(GL_POLYGON);
	glVertex2f(x, y - 15);
	glVertex2f(x + 10, y - 85);
	glVertex2f(x + 30, y - 85);
	glVertex2f(x + 30, y - 15);
	glEnd();

	//shape 6
	glBegin(GL_POLYGON);
	glVertex2f(x, y + 15);
	glVertex2f(x + 70, y + 15);
	glVertex2f(x + 70, y - 15);
	glVertex2f(x, y - 15);
	glEnd();

	//shape 7
	glBegin(GL_TRIANGLES);
	glVertex2f(x + 70, y + 5);
	glVertex2f(x + 80, y);
	glVertex2f(x + 70, y - 5);
	glEnd();

	//shape 8
	glLineWidth(1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x + 75, y);
	glVertex2f(x + 75, y + 15);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(x + 75, y);
	glVertex2f(x + 75, y - 15);
	glEnd();

	glColor3ub(255, 0, 255);

	//shape 9
	circle(x + 45, y, 15);

	glColor3ub(0,0,0);
	vprint(x, y, GLUT_BITMAP_9_BY_15,"%02d",num);

}


//
// To display onto window using OpenGL commands
//


//displays all
void display() {
	//
	// clear window to black
	//
	glClearColor(.52, .802, .98, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	int n = 0;
	for (int i = 0; i < count; i++) {

		//checks if a plane is moving or not
		if ((plane + i)->state) {

			//changes color when odd number of planes are there
			if (color == 0) {
				glColor3ub(255, 0, 0);
				color = 1;
			}
			else {
				glColor3ub(255, 255, 0);
				color = 0;
			}
		}


		//non-moving color
		else
			glColor3ub(200, 20, 140);
		//displays the shape on every iteration
		displayShape((plane + i)->p.x, (plane + i)->p.y,i+1);
	}

	display_background();

	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	point p;

	p.x = x - winWidth / 2;
	p.y = winHeight / 2 - y;
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);


	//deletes a shape when the del button on keyboard is pressed
	if (key == 127) {
		int index = -1;
		for (int i = 0; i < count; i++) {
			if (checkShape(p, (plane + i)->p)) {
				index = i;
			}

		}
		if (index != -1) {
			for (int i = index; i < count - 1; i++) {
				*(plane + i) = *(plane + i + 1);
			}
			count--;
			moving--;
		}
	}


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{

	// exit when ESC is pressed.
	if (key == 27)
		exit(0);


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.
	point p;
	//x1 - winWidth / 2
		//   y2 = winHeight / 2 - y1
	p.x = x - winWidth / 2;
	p.y = winHeight / 2 - y;


	//draws a shape if left mouse button is pressed
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
		bool yes = false;
		for (int i = 0; i < count; i++) {
			if (checkShape(p, (plane + i)->p))
				yes = true;
		}

		//planes does not exceed the max size
		if (!yes && count < SIZE) {
			(plane + count)->p = p;
			(plane + count)->state = false;
			count++;
		}
	}

	//the shape moves or stops when right mouse button is pressed
	else if (button == GLUT_RIGHT_BUTTON && stat == GLUT_DOWN) {
		int index = -1;//for checking if click is on a shape
		for (int i = 0; i < count; i++) {
			//click on a shape return index
			if (checkShape(p, (plane + i)->p)) 
				index = i;

		}

		if (index != -1) {

			//change of the plane that lies inside the clicked part
			if ((plane + index)->state) {
				(plane + index)->state = false;
				moving--;
			}
			else {
				(plane + index)->state = true;
				moving++;
			}

		}
	}
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.
	
	//for mouse coordinates
	p.x = x - winWidth / 2;
	p.y = winHeight / 2 - y;

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//TIMER_ON == 1
void onTimer(int v) {

	// Write your codes here.


	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	for (int i = 0; i < count; i++) {
		//increments the shape if its state is true so it keeps the plane moving
		if ((plane + i)->state) {
			(plane + i)->p.x += TIMER_PERIOD/10;
		}

		//if plane crosses the screen it starts from the start again
		if ((plane + i)->p.x - 200 > winWidth / 2)
			(plane + i)->p.x -= winWidth + 200;
	}


	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("HomeWork-1 by Noman Aslam");

	plane = (plane_t*)malloc(sizeof(plane_t)*SIZE);

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);

	Init();
	free(plane);
	glutMainLoop();
}
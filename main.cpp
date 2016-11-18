/*
 * jthomework2
 * by cww97 2016/11/17
 */
#include <windows.h>
#include <vector>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <GL/glut.h>
using namespace std;
static void idle(void){glutPostRedisplay();}
static void resize(int width, int height){
	const float ar = (float)width / (float)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

struct point {
	float x, y, z;
	point() {}
	point(float x, float y, float z) :x(x), y(y), z(z) {}
};
vector<point> points;
struct surface {
	int a[5];
	surface() {}
	surface(int x, int y, int z, int t){
		a[0] = x; a[1] = y; a[2] = z; a[3] = t;
	}
};
vector<surface>surfaces;

void readpoint(){
	double x, y, z;
	freopen("2.txt","r",stdin);
    for(int i; ~scanf("%d",&i);) {
        scanf("%lf%lf%lf",&x,&y,&z);
        points.push_back(point(x, y, z));
    }
    fclose(stdin);
}

void readsurface(){
	freopen("1.dat","r",stdin);
	int pointNum, edgeNum;
	scanf("%d%d",&pointNum,&edgeNum);
	int x,y,z,t;
	for(int n; ~scanf("%d", &n);) {
		scanf("%d%d%d%d",&x,&y,&z,&t);
		surfaces.push_back(surface(x, y, z, t));
	}
	fclose(stdin);
}


void linkPoint(point p1, point p2) {
	glColor3f(0, 0, 50);
	glVertex3f(p1.x, p1.y, p1.z);
	glColor3f(0, 0, 50);
	glVertex3f(p2.x, p2.y, p2.z);
}

void display(void){
	const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
	const double a = t*10.0;
	//glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glTranslated(-2.4, -0.6, -12);
	glRotated(60, 1, 0, 0);
	glRotated(a, 0, 0, 1);
	//here we go
	glScalef(0.1, 0.1, 0.1);
	glBegin(GL_LINES);
	for (int i = 0; i < surfaces.size(); i++){
		surface su = surfaces[i];
		vector<point> po;
		for (int j = 0; j < 3; j++) {
			po.push_back(points[su.a[j]-1]);
		}
		bool threePoints = 1;
		if (su.a[3] != 0) threePoints= 0;
		linkPoint(po[0], po[1]);
		linkPoint(po[0], po[2]);
		linkPoint(po[1], po[2]);
		if (!threePoints) {
			po.push_back(points[su.a[3] - 1]);
			for (int j = 0; j < 3; j++) {
				linkPoint(po[3], po[j]);
			}
		}
	}
	glEnd();
	glPopMatrix();
	glutSwapBuffers();
}

/* Program entry point */
int main(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(10, 10);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("GLUT Shapes");
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glClearColor(1, 1, 1, 1);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	//we work here
    readpoint();
	readsurface();
	display();
	glutMainLoop();
	return EXIT_SUCCESS;
}

#include<stdio.h>
#include<stdlib.h>

#include <math.h>

#include <GL/glut.h>

#include <vector>

/*
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
*/

#define WIDTH 600
#define HEIGHT 600

#define INTERVAL 25

#define RADIAN 57.29578

int POS_X, POS_Y;
float X=WIDTH/2.0f,Y=HEIGHT/2.0f;

typedef struct Rect {
    float Left, Right, Bottom, Top;
} Rect;

void Draw_Rect(Rect *Rct) {
    glBegin(GL_POLYGON);
    glVertex2f(Rct->Left, Rct->Bottom);
    glVertex2f(Rct->Right, Rct->Bottom);
    glVertex2f(Rct->Right, Rct->Top);
    glVertex2f(Rct->Left, Rct->Top);
    glEnd();
}

class c_Particle {
public:
	float x,y;
    float vx, vy;
    float Angle;
	float Alpha,Size;
	float Alpha_Offset,Size_Offset;
	Rect Rct;

    c_Particle(float x, float y, float vx, float vy, float Angle) {
    	this->x=x;
    	this->y=y;
    	this->vx=vx;
    	this->vy=vy;
    	this->Angle=Angle;
    	this->Alpha=1.0f;
    	this->Size=10.0f;
    	this->Alpha_Offset=0.01f+rand()%100/10000.0f;
    	this->Size_Offset=0.1f+rand()%100/1000.0f;
    }
    void Update_Rect() {
        Rct.Left = -Size;
        Rct.Right = Size;
        Rct.Bottom = -Size;
        Rct.Top = Size;
    }
    void Draw() {
    	glLoadIdentity();
    	glColor4f(1.0f,0.8f,0.43f,Alpha);
    	glTranslatef(x,y,0.0f);
    	glRotatef(Angle,0.0f,0.0f,1.0f);
        Draw_Rect(&Rct);
    }
    bool Update() {
        x += vx;
        y += vy;
        Alpha-=Alpha_Offset;
        Size-=Size_Offset;
        Update_Rect();
        if (Alpha<=0.0f||Size<=0.0f)
        	return false;
        return true;
    }
};

std::vector<c_Particle *> Particle;

void Display() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (c_Particle *particle : Particle)
        particle->Draw();
    glutSwapBuffers();
}

void Resize(int x, int y) {
    glutPositionWindow(POS_X, POS_Y);
    glutReshapeWindow(WIDTH, HEIGHT);
}

void Timer(int value) {
	float Angle;
	for (int i=0;i<6;i++){
		Angle=i*60.0f/RADIAN;
		Particle.push_back(new c_Particle(X,Y,0.8f*cos(Angle),0.8f*sin(Angle),rand()%360));
	}
    auto i=Particle.begin();
    while (i!=Particle.end()){
    	if (!(*i)->Update()){
    		delete (*i);
    		i=Particle.erase(i);
		}else
		i++;
	}
    glutPostRedisplay();
    glutTimerFunc(INTERVAL, Timer, 0);
}

void Init_GL(){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, WIDTH, HEIGHT);
    gluOrtho2D(0, WIDTH, HEIGHT, 0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Passive(int x, int y){
	X=x;
	Y=y;
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    POS_X = (glutGet(GLUT_SCREEN_WIDTH) - WIDTH) >> 1;
    POS_Y = (glutGet(GLUT_SCREEN_HEIGHT) - HEIGHT) >> 1;
    glutInitWindowPosition(POS_X, POS_Y);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Particle");
    Init_GL();
    glutDisplayFunc(Display);
    glutTimerFunc(0, Timer, 0);
    glutPassiveMotionFunc(Passive);
    glutMainLoop();
    return 0;
}

#include<bits/stdc++.h>
#include <GL/glut.h>
using namespace std;
#define num_particles 210
#define num_bullets 20
int freq = 100,fq=0,bl=0;
class Particle{
public:
    double x,y;
    double mass, vel;
    Particle(){
        x=0;    y=0;
        mass=1; vel=1;
    }
    Particle(double a, double b){
        x=a; y=b;
    }
    void move(double dt){
        y = y - vel*dt;
    }
};
class SpaceShip{
public:
    double x,y;
    SpaceShip(){
        x=-25;y=-300;
    }
    void draw(){
        glBegin(GL_POLYGON);

        glVertex2f(x+25,y+60);
        glVertex2f(x+50,y);
        glVertex2f(x+30,y+15);
        glVertex2f(x+20,y+15);
        glVertex2f(x,y);
        //glVertex2f(x+30,y+40);
        //glVertex2f(x+20,y+40);
        //glVertex2f(x,y);
        glEnd();
        glFlush();
    }
};
SpaceShip ship;
class Bullet:public SpaceShip{
public:
    double x,y;
    //SpaceShip sp;
    Bullet(){
        x=-2.5; y=-230;
    }
    void advance(){
        x = ship.x+22.5;
        y+=20;
        if(y>500) y=-220;
    }
    void draw(){
        glBegin(GL_POLYGON);
        glVertex2f(x,y);
        glVertex2f(x+5,y);
        glVertex2f(x+5,y+10);
        glVertex2f(x,y+10);
        glEnd();
        glFlush();
    }
};
Bullet bullet[num_bullets];
Particle particles[num_particles];
void init(void)
{       glClear(GL_COLOR_BUFFER_BIT);
        glClearColor (1.0, 1.0, 1.0, 1.0);
        gluOrtho2D (-500,500 ,-500,500);
        int i,base=10;
        for(i=0;i<num_particles;i++){
            particles[i].x=base+(i%30)*25 - 400;
            particles[i].y=particles[i].y - (i/30)*40 + 400;
        }
}
void display(void){
    int i;
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(20);
    glColor3f(1.0,0,0);
    ship.draw();
    glBegin(GL_POINTS);
    for(i=0;i<num_particles;i++){
        glVertex2f(particles[i].x, particles[i].y);
    }
    glEnd();
    for(i=0;i<bl;i++){
        bullet[i].draw();
    }
    glutSwapBuffers();
    //glFlush();
}
void Idle(void){
    fq++;
    if(fq==10000){
        fq=0;
        for(int i=0;i<num_particles;i++)
            particles[i].move(0.25);
        bl = min(bl+1,num_bullets);
        for(int i=0;i<bl;i++)
            bullet[i].advance();
        glutPostRedisplay();
    }
}
void SpecialKey(int key, int x, int y)
{

switch (key) {
    case GLUT_KEY_LEFT:
        ship.x-=5;
        break;
    case GLUT_KEY_RIGHT:
        ship.x+=5;
        break;
    }
    if(ship.x+25<-500) ship.x+=1000;
    else if(ship.x+25>500) ship.x-=1000;
    /*for(int i=0;i<num_bullets;i++){
        bullet[i].x=ship.x+20+2.5;
    }*/
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGB);
    glutInitWindowSize (1362, 750);
    glutInitWindowPosition (0, 0);
    glutCreateWindow ("Space Fighter");
    init ();
    glutSpecialFunc(SpecialKey);
    glutDisplayFunc(display);
    glutIdleFunc(Idle);
    glutMainLoop();
    return 0;
}

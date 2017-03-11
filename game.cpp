#include<bits/stdc++.h>
#include <GL/glut.h>
#include<time.h>
#include <IL/il.h>
#include <unistd.h>
#include <pthread.h>
using namespace std;
#define num_particles 150
#define num_bullets 20
pthread_t pth;
volatile int stopsound=0;
int freq = 100,fq=0,bl=0,bpos=0,active_particles=num_particles;
class Particle{
public:
    int active;
    double x,y;
    double mass, vel;
    Particle(){
        active=1;
        x=0;    y=0;
        mass=1; vel=1;
    }
    Particle(double a, double b){
        active=1;
        x=a; y=b;
    }
    void reset(){
        active=0;
        active_particles--;
    }
    void move(double dt){
        y = y - vel*dt;
    }
};
class SpaceShip{
public:
    double x,y;
    SpaceShip(){
        x=-40;y=-350;
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
    int active;
    //SpaceShip sp;
    Bullet(){
        active=1;
        x=ship.x+22.5; y=ship.y+75;
    }
    void advance(double a){
        y+=a;
        if(y>500){
            active=1;
            x = ship.x+22.5;
            y = ship.y+75;
         }
    }
    void reset(){
        active=0;
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
void *playfunc(void *arg){
    //while(!stopsound)
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    system("cvlc shoot.wav");
}
void timer(int val){
    int i,j,yc=35;
    for( i=0;i<num_particles;i++)
        if(particles[i].active)
            particles[i].move(1);
    bl = min(bl+1,num_bullets);
    for( i=0;i<bl;i++){
        bullet[i].advance(yc);
        // bullet[i].advance(25);
    }
    //pthread_create(&pth,NULL,collision,NULL);
    //collision();
    if(!active_particles) {printf("You Won The Game\n");exit(1);}

    glutTimerFunc(50, timer, 0);
}
void collision(int val){
    int i,j;
    double miny=-400;
    for(i=0;i<bl;i++){
        if(!bullet[i].active) continue;
        for(j=0;j<num_particles;j++){
            if(!particles[j].active) continue;
                if(bullet[i].x<particles[j].x+10 && bullet[i].x>particles[j].x-10 &&bullet[i].y+10>particles[j].y-10)
                {
                    bullet[i].reset();
                    particles[j].reset();
                }
        else if(bullet[i].x+5<particles[j].x+10 && bullet[i].x+5>particles[j].x-10 &&bullet[i].y+10>particles[j].y-10)
        {
            bullet[i].reset();
            particles[j].reset();
        }
        //if(particles[j].active) miny = min(miny, particles[j].y-10);
        }
    }
    glutTimerFunc(20, collision, 0);
}


void init(void)
{       glClear(GL_COLOR_BUFFER_BIT);
        //pthread_create(&pth,NULL,playfunc,NULL);
        //LoadTexture();
         glEnable(GL_TEXTURE_2D);
        glClearColor (1.0, 1.0, 1.0, 1.0);
        glOrtho (-500,500 ,-500,500,0,100);
        int i,base=10;
        glMatrixMode(GL_PROJECTION);     // Make a simple 2D projection on the entire window
         glLoadIdentity();
        // glOrtho(0.0, 500, 500, 0.0, 0.0, 100.0);

         glMatrixMode(GL_MODELVIEW);    // Set the matrix mode to object modeling

         glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
         glClearDepth(0.0f);
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the window
        for(i=0;i<num_particles;i++){
            particles[i].x=base+(i%30)*25 - 400;
            particles[i].y=particles[i].y - (i/30)*40 + 400;
        }
        for(i=0;i<num_bullets;i++){

        }

}
int LoadImage(char *filename)
{
    ILuint    image;
    ILboolean success;
    ilGenImages(1, &image);    /* Generation of one image name */
    ilBindImage(image);
     ilLoadImage(filename); /* Binding of image name */
     ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    return image;
}
void display(void){
    int i,width=-1000,height=1000;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPointSize(20);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_TEXTURE_2D);
    glColor3f(1,1,1);
    /* Draw a fullscreen mapped quad */
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2i(-500, -500);
        glTexCoord2i(0, 1); glVertex2i(-500, 500);
        glTexCoord2i(1, 1); glVertex2i(500, 500);
        glTexCoord2i(1, 0); glVertex2i(500, -500);
    glEnd();
    glFlush();
    //glBindTexture(GL_TEXTURE_2D,0);
    glDisable(GL_TEXTURE_2D);
    glColor3f(1.0,0,0);
    ship.draw();
    glBegin(GL_POINTS);
    for(i=0;i<num_particles;i++){
        if(particles[i].active)
        glVertex2f(particles[i].x, particles[i].y);
    }
    glEnd();
    glColor3f(0.4,0.5,0.1);
    for(i=0;i<bl;i++){
        if(bullet[i].active)
            bullet[i].draw();
    }
    glutSwapBuffers();
    //glFlush();
}
void Idle(void){
    fq++;
    int yc=35;
    if(fq==10000){
        fq=0;
        glutPostRedisplay();
    }
}
void SpecialKey(int key, int x, int y)
{

switch (key) {
    case GLUT_KEY_LEFT:
        ship.x-=25;
        break;
    case GLUT_KEY_RIGHT:
        ship.x+=25;
        break;
    }
    if(ship.x+25<-500) ship.x+=1000;
    else if(ship.x+25>500) ship.x-=1000;
    /*for(int i=0;i<num_bullets;i++){
        bullet[i].x=ship.x+20+2.5;
    }*/
}

int main(int argc, char *argv[]) {
    GLuint texid;
    int    image;
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGB);
    glutInitWindowSize (1362, 750);
    glutInitWindowPosition (0, 0);
    glutCreateWindow ("Space Fighter");
    glEnable( GL_TEXTURE_2D );
    init ();
    glutSpecialFunc(SpecialKey);
    glutDisplayFunc(display);
    glutIdleFunc(Idle);
    collision(0);
    timer(0);
    ilInit();
    image = LoadImage("texture.png");
       glGenTextures(1, &texid); /* Texture name generation */
       glBindTexture(GL_TEXTURE_2D, texid); /* Binding of texture name */
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear interpolation for magnification filter */
       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear interpolation for minifying filter */
       glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
        0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData()); /* Texture specification */

    glutMainLoop();
    stopsound=1;
    //pthread_cancel(pth);
    return 0;
}

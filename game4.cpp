#include<bits/stdc++.h>
#include <GL/glut.h>
#include<time.h>
#include <IL/il.h>
#include <unistd.h>
#include <pthread.h>
using namespace std;
#define num_particles 20
#define num_bullets 15
int load_texture(char *img, int i);
void exit_prog(int);
pthread_t pth,pth1;
volatile int stopsound=0,game_menu,pause_game=0,exit_game=0;
int freq = 100,fq=0,fq2,bl=0,bpos=0,active_particles=num_particles,mainw,menuw,pid;
int active_option=0;
double px[]={-480,-480,-480,-480,-480}, py[]={200,100,0,-100,-200},transp[]={1,0.2,0.2,0.2,0.2};
GLuint tex[10];
int level_th[]= {20,30,50,65,90,100};
int time_th[]=  {20,17,14,13,10,8};
int cur_score=0,high_score;
int time_left=10;
void show_score();
void show_name(string name);
void show_timer(int i);
void show_highscore();

int highest_scorer;
   char highest_scorer_name[100];
int high_score_flag=0,help_flag=0;   

void  	play_sound(void *arg);

string your_name="SHUBHAM";
int new_game=0;
int load_game=0;
int already_exist=0;
int does_not_exist=0;
int fire=1;

class user{

public:

    string name;
    int level;
    int high_score;

    user(){
    name = "";
    level=0;high_score=0;
    }

    bool newuser(string nme){
      name = nme;

      ifstream infile1("Entry.txt");
        int a,b;
        string n;
        while(!infile1.eof()){
        //std::cout<<"Infinite\n";
        infile1>>n;

        if(infile1.eof()){  break; }
        infile1>>a;
        infile1>>b;
        if(n==name){ return false; }

      }
      infile1.close();

      ofstream infile("Entry.txt",ios::app);
      infile<<name<<endl;
      infile<<level<<endl;
      infile<<high_score<<endl;
      infile.close();
      return true;
    }

    bool loaduser(string nme){
        ifstream infile("Entry.txt");
        int a,b;
        string n;
        while(!infile.eof()){
        //std::cout<<"Infinite\n";
        infile>>n;

        if(infile.eof()){  break; }
        infile>>a;
        infile>>b;
        //std::cout<<n<<" "<<a<<" "<<b<<"\n";

        if((n==nme)){ std::cout<<"Found\n"; name = nme;level=a;high_score=b; infile.close(); return true; }

        }
        infile.close();
    return false;
    }


    bool update(){

        fstream infile;
        infile.open("Entry.txt");
        int a,b;
        string n;

        while(!infile.eof()){
        infile>>n;
        if((n==name)){
        int pos = infile.tellp();
        infile>>a;
        infile>>b;

        char buff[2000]={'\0'};
        int count=0;
        while(!infile.eof()){  infile.get(buff[count]); if(infile.eof()) break;  count++; }
                infile.clear();
        infile.seekp(pos,ios::beg);

        infile<<'\n'<<level<<endl;
        infile<<high_score;

        for(int i=0;i<count;i++)
        infile.put(buff[i]);

        infile.close(); return true; }


        }

        infile.close();
    return false;

    }


    void display(){

        std::cout<<"Name : "<<name<<endl;
        std::cout<<"Level : "<<level<<endl;
        std::cout<<"High Score : "<<high_score<<endl;

    }


};


user obj;
char name[40];


void idle(int x){

static int y=0;
if(new_game==0 && load_game==0)
return ;

char string[40] = "Enter Your Name";


glBegin(GL_LINE_STRIP);
        glColor4f(1.0, 1.0, 1.0,1);
glVertex2f(-120,135);
glVertex2f(50,135);
glVertex2f(50,80);
glVertex2f(-120,80);
glVertex2f(-120,135);

glEnd();

glBegin(GL_LINE_STRIP);
        glColor4f(1.0, 1.0, 1.0,1);
glVertex2f(-120,35);
glVertex2f(50,35);
glVertex2f(50,-20);
glVertex2f(-120,-20);
glVertex2f(-120,35);

glEnd();



GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
        glColor4f(1.0, 1.0, 1.0,1);
        glRasterPos2f (-100, 100);
         for (int i = 0; string[i] != '\0'; i++)
        glutBitmapCharacter(font_style, string[i]);

        glColor4f(1.0, 1.0, 1.0,1);
        glRasterPos2f(-100,0);
         for (int i = 0; name[i] != '\0'; i++)
        glutBitmapCharacter(font_style, name[i]);

        glColor4f(1.0, 1.0, 1.0,0);
    if(y%20==0){ glutBitmapCharacter(font_style, '|'); y=0; }
    y++;
    glFlush();

//std::cout<<"user prog "<<game_user<<"\n";

//glutTimerFunc(100,idle,x^1);

}

void already(int x){

static int y=0;
if(new_game==0 && load_game==0)
return ;

char string[40] = "Name is Already Exist ,Try Another";


GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
        glColor4f(1.0, 0, 0,1);
        glRasterPos2f (-100, -100);
         for (int i = 0; string[i] != '\0'; i++)
        glutBitmapCharacter(font_style, string[i]);


}


void does(int x){

static int y=0;
if(new_game==0 && load_game==0)
return ;

char string[] = "This name Doensn't Exist";


GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_24;
        glColor4f(1.0, 0, 0,1);
        glRasterPos2f (-100, -100);
         for (int i = 0; string[i] != '\0'; i++)
        glutBitmapCharacter(font_style, string[i]);


}






class Particle{
public:
    int active;
    double x,y,width,height,m;
    double mass, vel;
    Particle(){
        active=1;width=30;height=50;
        x=0;    y=0; m=(rand()%500-250)/250.0;
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
        if(m!=0) x = x-1/m;
    }
    void draw(){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,tex[2]);
        glClearColor (1.0, 1.0, 1.0, 1.0);
        glColor4f(1,1,1,1);
        glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2i(x,y-height);
        glTexCoord2i(0, 1); glVertex2i(x-width, y-height);
        glTexCoord2i(1, 1); glVertex2i(x-width, y);
        glTexCoord2i(1, 0); glVertex2i(x, y);
        glEnd();
        glFlush();
        glDisable(GL_TEXTURE_2D);
    }
};


class SpaceShip{
public:
    double x,y,width,height;
    SpaceShip(){
        x=-40;y=-350;width=50;height=90;
    }
    void draw(){
//  load_texture("spaceship.png");
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,tex[1]);
  glClearColor (1.0, 1.0, 1.0, 1.0);
  glColor4f(1,1,1,1);
  /* Draw a fullscreen mapped quad */
  glBegin(GL_QUADS);

      glTexCoord2i(0, 0); glVertex2i(x,y);
      glTexCoord2i(0, 1); glVertex2i(x, y-height);
      glTexCoord2i(1, 1); glVertex2i(x-width, y-height);
      glTexCoord2i(1, 0); glVertex2i(x-width, y);
  glEnd();

        glFlush();
        glDisable(GL_TEXTURE_2D);
    }
};

SpaceShip ship;

class Bullet:public SpaceShip{
public:
    double x,y,width,height;
    int active;
    //SpaceShip sp;
    Bullet(){
        active=1;width=15;height=30;
        x = ship.x-18;
        y = ship.y+10;
    }
    void advance(double a){
        y+=a;

    }
    void reset(){
        active=0;
    }
    void draw(){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,tex[4]);
      glClearColor (1.0, 1.0, 1.0, 1.0);
       glColor4f(1,1,1,1);
        /* Draw a fullscreen mapped quad */
        glBegin(GL_QUADS);

            glTexCoord2i(0, 0); glVertex2i(x,y);
            glTexCoord2i(0, 1); glVertex2i(x, y-height);
            glTexCoord2i(1, 1); glVertex2i(x-width, y-height);
            glTexCoord2i(1, 0); glVertex2i(x-width, y);
        glEnd();/*
        glBegin(GL_POLYGON);
        glVertex2f(x,y);
        glVertex2f(x+5,y);
        glVertex2f(x+5,y+10);
        glVertex2f(x,y+10);
        glEnd();*/
        glFlush();
    }
};
//Bullet bullet[num_bullets];
vector< Bullet > bullet;
vector< Particle > particles;

//Particle particles[num_particles];
void *playfunc(void *arg){
    //while(!stopsound)
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    //system("cvlc shoot.wav");
    system("canberra-gtk-play -f bull.wav");
}


void *destroy(void *arg){
    //while(!stopsound)
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    //system("cvlc shoot.wav");
    system("canberra-gtk-play -f Explosion+3.wav");
}

void *coll(void *arg){
    //while(!stopsound)
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    //system("cvlc shoot.wav");
    system("canberra-gtk-play -f Explosion+1.wav");
}


void *countdown(void *arg){

    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    system("canberra-gtk-play -f 5to1.wav");
    pthread_exit(NULL);
}

void *timeout(void *arg){

    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    system("canberra-gtk-play -f timeout.wav");
    pthread_exit(NULL);
}

void timer(int val){
	if (exit_game||pause_game) return;
    int i,j,yc=35;
    if(!game_menu&&!(new_game==1||load_game==1)){

    for( i=0;i<particles.size()&&!game_menu&&!(new_game==1||load_game==1);i++){

        if(particles[i].active)
            particles[i].move(4);
            if(particles[i].y>500 || particles[i].y<-500)
            {
                cout<<particles.size()<<endl;
                particles.erase(particles.begin()+i);
            }// bullet[i].advance(25);
            else if(particles[i].x>500 || particles[i].x<-500){
                particles.erase(particles.begin()+i);
            }
             if(exit_game) return;
        }
        if(exit_game) return;
    for( i=0;i<bullet.size()&&!game_menu&&!(new_game==1||load_game==1);i++){
        // if(!fire) return;
        bullet[i].advance(yc);
        if(bullet[i].y>500 || bullet[i].y<-500)
        {
            bullet.erase(bullet.begin()+i);
        }// bullet[i].advance(25);

        }
       // if(!active_particles) {printf("You Won The Game\n");exit(0);}
    }
    //pthread_create(&pth,NULL,collision,NULL);
    //collision();
    if (!exit_game  && !game_menu)
    glutTimerFunc(30, timer, 0);
}
void fire_bullet(){

if(!fire)
return;

Bullet blt;
bullet.push_back(blt);
pthread_create(&pth,NULL,playfunc,NULL);
}

void new_particle(){
    //cout<<"\nin new";
    Particle prt;
    particles.push_back(prt);
}
void collision(int val){
    if (exit_game||pause_game) return;
    int i,j;
    double miny=-400;
    for(i=0;i<bullet.size()&&!game_menu&&!(new_game==1||load_game==1);i++){ if (exit_game) return;
        if(!bullet[i].active) continue;
        for(j=0;j<particles.size()&&!game_menu&&!(new_game==1||load_game==1);j++){ if (exit_game) return;
            if(!particles[j].active) continue;
            if(bullet[i].x-bullet[i].width<particles[j].x &&bullet[i].x-bullet[i].width > \
                    particles[j].x-particles[j].width && bullet[i].y>particles[j].y-particles[j].height && \
                bullet[i].y< particles[j].y)
                {
                    //bullet[i].reset();
                    pthread_create(&pth,NULL,coll,NULL);
                    bullet.erase(bullet.begin()+i);
                    particles.erase(particles.begin()+j);
                    cur_score++;
                //    particles[j].reset();
                }
        else if(bullet[i].x<particles[j].x &&bullet[i].x > \
            particles[j].x-particles[j].width && bullet[i].y>particles[j].y-particles[j].height && \
        bullet[i].y< particles[j].y)
        {

        //    bullet[i].reset();
        //    particles[j].reset();
            
            pthread_create(&pth,NULL,coll,NULL);
            bullet.erase(bullet.begin()+i);
            particles.erase(particles.begin()+j);
            cur_score++;
        }
        //if(particles[j].active) miny = min(miny, particles[j].y-10);
        }
        }
        
        double ship_radius = sqrt(ship.width*ship.width+ship.height*ship.height)/2;
        double particle_radius = sqrt(particles[0].width*particles[0].width+particles[0].height*particles[0].height)/2;
        
        for(j=0;j<particles.size()&&!game_menu&&!(new_game==1||load_game==1);j++){
        if (exit_game) return;
        
     	double dx = (ship.x-ship.width/2) - (particles[j].x-particles[j].width/2);
     	double dy = (ship.y-ship.width/2) - (particles[j].y-particles[j].width/2);
        double dist_between_centres = sqrt(dx*dx + dy*dy);
        if(dist_between_centres +40 <= ship_radius+particle_radius){
        	pthread_create(&pth,NULL,destroy,NULL);
        	fire=0;
        	bullet.clear();
        	exit_prog(0);
        	return;
        	}
        }
        
        //No need for below collision procedure
       /* for(j=0;j<particles.size()&&!game_menu&&!(new_game==1||load_game==1);j++){if (exit_game) return;
            if(!particles[j].active) continue;
                if(ship.x-ship.width+20<particles[j].x &&ship.x-ship.width+20 > \
                    particles[j].x-particles[j].width && ship.y>particles[j].y-particles[j].height && \
                   ship.y<particles[j].y)
                {
                    fire=0;
                    bullet.clear();
                //    time_left=-100
                    exit_prog(0);
                    return;
                }
        else if(ship.x-20<particles[j].x &&ship.x-20 > \
            particles[j].x-particles[j].width && ship.y>particles[j].y-particles[j].height && \
           ship.y<particles[j].y)
        {
            fire=0;
            //time_left=-100;
            bullet.clear();
            exit_prog(0);
            return;
        }
        //if(particles[j].active) miny = min(miny, particles[j].y-10);
        }
		*/
    if(!exit_game && !game_menu)
    glutTimerFunc(30, collision, 0);
}


void init(void)
{
        //pthread_create(&pth,NULL,playfunc,NULL);
        //LoadTexture();
       // pthread_create(&pth1,NULL,play_sound,NULL);
       //First Play Background Music
       
       play_sound(NULL);
        if(!exit_game){
            glClear(GL_COLOR_BUFFER_BIT);
         int tx =load_texture("wallpapers-nebula-space-galaxy-spiral-desktop-images-80753.jpg", 0);
        tx = load_texture("spaceship.png", 1);
         tx = load_texture("a11.png", 2);
         tx = load_texture("aest.jpg", 3);
         tx=load_texture("lasergun.png",4);
         tx=load_texture("help1.png",5);
         tx=load_texture("FBOG.jpg",6);
        }
         glEnable(GL_TEXTURE_2D);
        glClearColor (1.0, 1.0, 1.0, 1.0);
        glOrtho (-500,500 ,-500,500,0,100);
        glDisable(GL_TEXTURE_2D);
        int i,base=20;
        glMatrixMode(GL_PROJECTION);     // Make a simple 2D projection on the entire window
         glLoadIdentity();
         glEnable(GL_BLEND);
           glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // glOrtho(0.0, 500, 500, 0.0, 0.0, 100.0);

         glMatrixMode(GL_MODELVIEW);    // Set the matrix mode to object modeling

         glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
         glClearDepth(0.0f);
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the window
        for(i=0;i<num_particles;i++){
            new_particle();
            particles[i].x=base+(i%10)*60 - 400;
            particles[i].y=particles[i].y - (i/10)*70 + 700;
        }

        //timer(0);
        //collision(0);

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
void show_background(int i){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,tex[i]);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0); glVertex2i(-500, -500);
        glTexCoord2i(0, 1); glVertex2i(-500, 500);
        glTexCoord2i(1, 1); glVertex2i(500, 500);
        glTexCoord2i(1, 0); glVertex2i(500, -500);
    glEnd();
    glFlush();
    glDisable(GL_TEXTURE_2D);
}


void draw_option(int i){
    glBegin(GL_POLYGON);
    glColor4f(0.1,0.5,0.65,transp[i]);
    glVertex2f(px[i],py[i]);
    glVertex2f(px[i]+330,py[i]);
    glVertex2f(px[i]+330,py[i]-70);
    glVertex2f(px[i],py[i]-70);
    glEnd();

    glFlush();
}
void draw_font(string s,double x,double y){

    glRasterPos2f(x, y);
    for (int i = 0; i < s.size(); i++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
  }
}



void show_score()
{
glColor3f(1,1,0);
draw_font("SCORE",-400,400);
draw_font(to_string(cur_score),-380,350);

}

void play_sound(void * arg)
{
   pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
   pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
   
   pid =fork();
   if(pid==0){
        system("canberra-gtk-play --loop=1000 -f pirate.wav");
        exit(0);
       }
       
       
	//glutTimerFunc(30, play_sound, 0);
	
}

void show_timer(int i)
{
if (exit_game||pause_game) return;
time_left=i;
cout<<i<<" ";
if(i<=0){
bullet.clear();
fire=0;}

if(exit_game || game_menu) { time_left=0;return;  }
if(i==-1)
  {exit_game=1;
  //volatile int j=0;
  //while(j<100000){ fire=0;  bullet.clear();  j++;}
  //glutPostRedisplay();
  if(cur_score >= level_th[obj.level]) {  pthread_create(&pth,NULL,timeout,NULL);  exit_prog(1);}
  else{       pthread_create(&pth,NULL,timeout,NULL);
    exit_prog(0); }
  return ;
  }
//glColor3f(0.8,0.2,0.2);
//draw_font("TIME LEFT",380,250);
//draw_font(to_string(i),380,240);

//if (!exit_game  && !game_menu)
glutTimerFunc(1000,show_timer, i-1);

}

void show_name(string name){
glColor3f(1,1,0);
//draw_font("NAME",380,150);
draw_font(name,0,400);


}
void show_highscore()
{
//obj.high_score=max(obj.high_score,cur_score);


high_score=max(obj.high_score, cur_score);

glColor3f(1,1,0);

draw_font("HIGH SCORE",-200,400);

draw_font(to_string(high_score),-160,350);

}
void show_filehigh()
{
   fstream file;
   file.open("Entry.txt",ios::in|ios::out);
   char str1[100];
   int z,x;
   char str2[100];
   char str3[100];
   char str4[100];
   char str5[100];
   char str6[100];
   file.getline(str1,100);
   file.getline(str2,100);
   file.getline(str3,100);
   x=atoi(str3);
   while(file)
   {
     file.getline(str4,100);
     file.getline(str5,100);
     file.getline(str6,100);
     z=atoi(str6);
     if(z>x)
       {
       cout<<x<<"j"<<z<<endl;
       strcpy(str1,str4);
       x=z;
       }
   }
   fstream file1;
   file1.open("entry2.txt",ios::out);
   file1<<str1<<" ";
   file1<<x<<endl;
   strcpy(highest_scorer_name,str1);
   highest_scorer=x;
   //draw_font(str1,400,400);
   //draw_font(to_string(x),400,350);
   //glutSwapBuffers();
   

}





void show_menu(){
    show_background(3);
    draw_option(0);
    glColor3f(0.8,0.2,0.2);
    draw_font("New Game",px[0]+125,py[0]-45);
    draw_option(1);
    glColor3f(0.8,0.2,0.2);
    draw_font("Load Game",px[1]+122,py[1]-45);
    draw_option(2);
    glColor3f(0.8,0.2,0.2);
    draw_font("Quit Game",px[2]+120,py[2]-45);
    draw_option(3);
    glColor3f(0.8,0.2,0.2);
    draw_font("Help",px[3]+150,py[3]-45);
    draw_option(4);
    glColor3f(0.8,0.2,0.2);
    draw_font("High Score",px[4]+120,py[4]-45);
}
void display(void){
    int i,width=-1000,height=1000;
    //if(pause_game) return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.4,0.5,0.1);
    if (game_menu) {
        show_menu();
        if(high_score_flag==1)
        {
        draw_font("Name",300,400);
        draw_font("Highscore",380,400);
        draw_font(highest_scorer_name,300,350);
         draw_font(to_string(highest_scorer),410,350);
         for(i=0;i<10;i++){
      	   draw_font(".",315,300-20*i);
      	 	draw_font(".",415,300-20*i);
      	 	}
      	 if(highest_scorer_name!=obj.name){
		 	draw_font(obj.name,300,0);
        	draw_font(to_string(high_score),410,0);
        	  }
        }
        
    }
   
    else if(new_game){
         //glClear(GL_COLOR_BUFFER_BIT);
         show_background(6);
         idle(0);
         high_score_flag=0;
         glColor3f(0.5,1,0.4);
         draw_font("Press del for Game menu",-120,300);
         if(already_exist){
            already(0);
         }
    }
    else if(load_game){
         glClear(GL_COLOR_BUFFER_BIT);
         show_background(6);
         idle(0);
         high_score_flag=0;
         glColor3f(0.5,1,0.4);
         draw_font("Press del for Game menu",-120,300);
         if(does_not_exist){
            does(0);
    }
    }
    else if(help_flag){
            
			show_background(5);
			glColor3f(0.5,1,0.4);
         draw_font("Press del for Game menu",200,400);
			glutSwapBuffers();
			return;
		}
    
    else{

        show_background(0);
        show_highscore();
        show_score();
        your_name = obj.name;
        show_name(your_name);
        
        
        glColor3f(1,1,0);
        draw_font("LEVEL",200,400);
        
        draw_font(to_string(obj.level),223,350);
         
         glColor3f(1,1,0);
         
         draw_font("TIME LEFT",400,400);
         draw_font(to_string(time_left),435,350);


    }
    GLuint tx1,tx2;

    if(!game_menu&&!(new_game==1||load_game==1))
    ship.draw();
    for(i=0;i<particles.size()&&!game_menu&&!(new_game==1||load_game==1);i++){
        if(particles[i].active)
        {
            particles[i].draw();
            //cout <<"\nanot active";
        }
        else{
        //    cout <<"\nnot active";
        }
        //glVertex2f(particles[i].x, particles[i].y);
    }
    if(exit_game) return;
    glColor3f(0.4,0.5,0.1);
    for(i=0;i<bullet.size()&&!game_menu&&!(new_game==1||load_game==1);i++){

        if(bullet[i].active)
            bullet[i].draw();
    }
    glutSwapBuffers();
    //glFlush();
}
void Idle(void){
    fq++;
    fq2++;
    int yc=35;
    if(fq==1000){
        fq=0;
        if(!exit_game&&!pause_game)
        glutPostRedisplay();
    }
    if(fq2==10e3 - obj.level*1000 && !exit_game){
        fq2=0;
        if(!game_menu&&!(new_game==1||load_game==1||help_flag) && !pause_game){
        new_particle();
        int i=particles.size()-1;
        particles[i].x=rand()%1000-500;
        particles[i].y=500;
        if(ship.x==particles[i].x) particles[i].m=10e10;
        else
        particles[i].m = (ship.y-ship.height/2-particles[i].y+particles[i].height/2) \
        /(ship.x-ship.width/2-particles[i].x+particles[i].width/2);
        }
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
    case GLUT_KEY_UP:
        if(game_menu){
        transp[active_option]=0.2;
        active_option-=1;
        active_option+=5;
        active_option%=5;
        transp[active_option]=1;
        show_menu();
        }
        else{
            if(fire&&!pause_game&&!exit_game&&!game_menu)
            fire_bullet();
        }
        break;
    case GLUT_KEY_DOWN:
    if(game_menu){
        transp[active_option]=0.2;
        active_option+=1;
        active_option%=5;
        transp[active_option]=1;
        show_menu();
        }
        break;

    }
    if(ship.x+25<-500) ship.x+=1000;
    else if(ship.x+25>500) ship.x-=1000;
    /*for(int i=0;i<num_bullets;i++){
        bullet[i].x=ship.x+20+2.5;
    }*/
}
int firsttime=1;
int load_texture(char * img,int i){
    GLuint    image;
      GLuint texid;
      //cout<<img;
  image = LoadImage(img);
     glGenTextures(1, &tex[i]); /* Texture name generation */
     glBindTexture(GL_TEXTURE_2D,tex[i]); /* Binding of texture name */
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear interpolation for magnification filter */
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear interpolation for minifying filter */

     glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT),
      0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData()); /* Texture specification */
       ilDeleteImages(1, &image);
       firsttime=0;
       //cout<<tex[i]<<"\n";
       return tex[i];
}

void destruct(){
	//First Kill Background Music
 	string s1 = "kill "+to_string(pid);
 	string s2 = "kill "+to_string(pid+2);
	cout<<s1;
	system(s1.c_str());
	system(s2.c_str());
    stopsound=1;

}

void key_func(unsigned char key,int x,int y){
cout<<key;
if(key=='p' && !game_menu && !(new_game||load_game||high_score_flag)){
 pause_game = 1-pause_game;
 if(!pause_game){

 show_timer(time_left);
 timer(0);
 collision(0);
 }
 else{
	 draw_font("PAUSE",-20,100);
	 glFlush();
 	 glutSwapBuffers();
 }
 }

if(key==13&&!pause_game){  // on Enter key

//std::cout<<"Enter"<<endl;

if(active_option==0 && game_menu){
    game_menu=0;
    new_game=1;
    timer(0);
    collision(0);
    std::cout<<"enteres new\n";

}

else if(active_option==1 && game_menu){
    std::cout<<"enteres load\n";
    game_menu=0;
    load_game=1;
    timer(0);
    collision(0);
}
else if(active_option==2 && game_menu){
 destruct();
 exit(0);
}

else if(active_option==3 && game_menu)
{
// show_filehigh();
 help_flag=1;
 game_menu=0;
}

else if(active_option==4 && game_menu)
{
 show_filehigh();
 high_score_flag=1;

}

else if(game_menu==0 && new_game==1 ){

if(strlen(name)==0)
return ;


if(obj.newuser(name))
new_game=0;
else{already_exist=1; return ;}

show_timer(time_th[obj.level]);
}

else if(game_menu==0 && load_game==1){

if(strlen(name)==0)
return ;

//std::cout<<"In load Game\n";
//std::cout<<name<<"\n";
if(obj.loaduser(name))
load_game=0;
else
{does_not_exist=1; return ;}
show_timer(time_th[obj.level]);
}

//glutPostRedisplay();
}

else if(((key>=65 && key<=90) || (key>=97 && key<=122)   ) && (new_game || load_game) ){
    name[strlen(name)]=key;
}
else if(key==32 && !game_menu && !new_game && !load_game && !exit_game&&!pause_game){
    if(!fire) return;
    fire_bullet();
}
else if (key=='r' && exit_game==1){
    //glClear(GL_COLOR_BUFFER_BIT);
    new_game=0;
    load_game=0;

    game_menu=1;
    active_option=0;
    sleep(1);
    particles.clear();
    bullet.clear();
    //init();
    for(int i=0;i<num_particles;i++){
        new_particle();
        particles[i].x=20+(i%10)*60 - 400;
        particles[i].y=particles[i].y - (i/10)*70 + 700;
    }
    exit_game=0;

}
 

//else if(GLUT_ACTIVE_SHIFT){

 if(key==127 && !game_menu){
new_game=0;load_game=0;game_menu=1;pause_game=0;
high_score_flag=0;help_flag=0;
}

//cout<<"gfgfgf\n";
//}


else if(key==8 && (new_game || load_game)){

if(strlen(name)>0)
name[strlen(name)-1]='\0';

}

//cout<<"fire\n";

}

void reset_to_menu(){


    active_option=0;
    transp[0]=1;
    for(int i=1;i<4;i++) transp[i]=0.2;
    cur_score=0;
   // level=0;
   new_game=0;load_game=0;game_menu=1;
    obj.high_score=0;
    obj.level=0;
     pthread_create(&pth,NULL,countdown,NULL);
    sleep(5);
    fire=1;
    pause_game=0;
    particles.clear();
    bullet.clear();
    //init();

    for(int i=0;i<num_particles;i++){
        new_particle();
        particles[i].x=20+(i%10)*60 - 400;
        particles[i].y=particles[i].y - (i/10)*70 + 700;
        particles[i].active =1;
    }
exit_game=0;

printf("Come to End!!!!!!!!!!!\n");
}



void level_continue(){

    cur_score=0;
     pthread_create(&pth,NULL,countdown,NULL);
    sleep(5);
    
    //pthread_join(pth , NULL);
    
    fire=1;
    pause_game=0;
    particles.clear();
    bullet.clear();

    for(int i=0;i<num_particles;i++){
        new_particle();
        particles[i].x=20+(i%10)*60 - 400;
        particles[i].y=particles[i].y - (i/10)*70 + 700;
        particles[i].active =1;
    }
    
    timer(0);
    collision(0);
    exit_game=0;
    show_timer(time_th[obj.level]);
}


void exit_prog(int flag)
{   char cntr[]="543210";
	//pause_game=1;
    exit_game=1;
    bullet.clear();

    //sleep(1);

    if(!flag){

        if(obj.high_score < cur_score)
        obj.high_score = cur_score;
        obj.update();

	    glColor3f(1,0,0);
        draw_font("You Lose the game..",-100,200);
        
        draw_font("You didn't Score Enough",-130,100);
        
        draw_font("Wait for 5 seconds..",-100,0);
        glFlush();
        glutSwapBuffers();
        
        reset_to_menu();
    }
    else{
   	    glColor3f(0,1,0);

   	    if(obj.high_score < cur_score)
        obj.high_score = cur_score;obj.level++;
        obj.update();
        draw_font("You win the game..",-100,200);
        int ofs=0;
        if(cur_score>=obj.high_score){
        glColor3f(0,1,1);
        draw_font("Wooah You Scored New High Score",-160,100);
        glColor3f(0,1,0);
        ofs=100;
        }
        draw_font("Wait for 5 seconds..",-100,100-ofs);       
        glFlush();
        glutSwapBuffers();

        level_continue();
    
    }
    
//    active_option=0;
//    transp[0]=1;
//    for(int i=1;i<4;i++) transp[i]=0.2;
//    game_menu=1;
//    cur_score=0;
//    level=0;
//    obj.high_score=0;
//    obj.level=0;
//    sleep(4);
//    time_left=10;
//    fire=1;
//    particles.clear();
//    bullet.clear();

//    for(int i=0;i<num_particles;i++){
//        new_particle();
//        particles[i].x=20+(i%10)*60 - 400;
//        particles[i].y=particles[i].y - (i/10)*70 + 700;
//        particles[i].active =1;
//    }
//    
//    timer(0);
//    collision(0);
//    exit_game=0;
//    show_timer(time_th[obj.level]);




    printf("\nExiting from game \n");

}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE| GLUT_RGB);
    glutInitWindowSize (1362, 750);
    glutInitWindowPosition (0, 0);
    menuw = glutCreateWindow ("Game Menu");
    game_menu=1;
  //  mainw =  glutCreateWindow ("Play Game");
    glutSetWindow(menuw);
    glEnable( GL_TEXTURE_2D );
    ilInit();
    init ();
    glutKeyboardFunc(key_func);
    glutSpecialFunc(SpecialKey);
    glutDisplayFunc(display);
    glutIdleFunc(Idle);


    glutMainLoop();
    string s1 = "kill "+to_string(getpid()+3);
	cout<<s1;
	//while(!stopsound);
	system(s1.c_str());
    stopsound=1;
    //pthread_cancel(pth);
    return 0;
}

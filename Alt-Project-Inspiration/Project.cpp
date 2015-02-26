#include <windows.h>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <mmsystem.h> 

#include "glut.h"
#include "gunship.h"
#include "ground.h"
#include "ring.h"
#include "cloud.h"
#include "textureLoader.h"
#include "Camera.h"

using namespace std;

float local_x[3];
float local_y[3];
float local_z[3];

//gunship rotations and movements
static float x_angle=0.0f;//pitch
static float y_angle=0.0f;//yaw
static float z_angle=0.0f;//roll

static float x_position=0.0f;
static float y_position=0.0f;
static float z_position=0.0f;

static float gunshipMovementInc = 0.05;//how fast to move it forward.

static float x_increment = 0.0f;
static float y_increment = 0.0f;
static float z_increment = 0.0f;

static bool animationMove = false;
static float pi = 3.14159265;

//static float shipx = 0.0,shipy=0.0,shipz=0.0;
//camera variables
static bool camMove = false;
static float camAngle = 0.0f;
static float r = 3.0f,yrot =30, camx,camz;

static float camX = 0.0f;
static float camY = 0.0f;
static float camZ = 0.0f;


static float xrot=0.0f,angle=0.0f;

static float camDistance = 5.0f;

//model display mode
static bool modelMode = false;
static bool firstPersonView = false;

//lighting and effects variables
static bool theSunOn = true;
static GLfloat atmoColor[4] = {.8,.8,1,1};
static float fogIntense = 1.0f;
static float blurSamples = 3.0f;
//static GLfloat noAmbient[] = {0.0, 0,0, 0.2, 1.0};
//static GLfloat headlightColor[] = {1.0,1.0,1.0,1.0};

//texture info
static GLuint *grassText, *rockText, grassT, rockT;

//explosions
static int explosionTime = 0;
static float explosionForce = 0;
static bool animationExplosion = false;
//window info
GLsizei winWidth = 500,winHeight = 500;
static int mainWindow,fpsWindow;
static int frame=0,time,timebase=0;
static int font = (int)GLUT_BITMAP_8_BY_13;
static int bitmapHeight=13;
static char s[30];


//gunship 1
Gunship myGunship;

//the floor
//Ground theGround;

//rings
vector<Ring* > rings;
Ring aRing, bRing, cRing, dRing, eRing;
static bool ringsSet = false;
//a few clouds
vector<Cloud* > clouds;
Cloud aCloud, bCloud, cCloud;

//Camera
Camera myCamera;

//collision boxes
vector<BoundingBox*> bumpables;

//scoring
static int score = 0, remaining = 5;
//incase pi isn't around
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void init(void)
{
	glGenTextures(1,&grassT);
	LoadBMP(grassT, "rock.jpg");
	
	//grassT = LoadTextureRAW("grass", 1, 256, 256);
	//grassText = &grassT;
	//rockT = LoadTextureRAW("rock", 1, 1024, 1024);
	//rockText = &rockT;

	camX = camDistance*(sin(pi/180*camAngle)) + x_position;
	//camY = camDistance*(sin(PI/180*deg3CamRotateZ)) + transY;
	camZ = camDistance*(cos(pi/180*camAngle)) + z_position;
}
void setRings()
{
	rings.push_back( &aRing);
	rings.push_back( &bRing);
	rings.push_back( &cRing);
	rings.push_back( &dRing);
	rings.push_back( &eRing);

	clouds.push_back( &aCloud);
	clouds.push_back( &bCloud);
	clouds.push_back( &cCloud);
	
	aRing.setBox(0,0,-30);
	bRing.setBox(0,0,-90);
	cRing.setBox(30,0,-60);
	dRing.setBox(30,0,40);
	eRing.setBox(-30,0,120);
	
	aCloud.setBox(0,0,-60);
	bCloud.setBox(30,0,0);
	cCloud.setBox(0,0,60);
}

void enable(void)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	GLfloat globalAmbient[] = {0.5f,0.5f,0.5f,1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);
}

void winReshapeFcn(GLint newWidth,GLint newHeight)
{
	float ratio = winWidth/winHeight;
	glViewport(0,0,newWidth,newHeight);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(50.0,ratio,1,1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	myCamera.lookAt();


}

void playSound(void)
{
	PlaySound(L"8bit.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}
void animateExplosion()//Blows up the chopper.
{
	if(explosionTime < 100);
		explosionTime++;

	explosionForce = explosionTime*.025;

	glutPostRedisplay();
}
void animateMove()//Moves the helicopter in the direction that it is pointing.
{
	if(animationMove)
	{
		z_increment = gunshipMovementInc*(cos(pi/180*y_angle)*cos(pi/180*x_angle));
		y_increment = gunshipMovementInc*(sin(pi/180*x_angle));
		x_increment = gunshipMovementInc*(sin(pi/180*y_angle)*cos(pi/180*x_angle));

		x_position -= x_increment;
		y_position += y_increment;
		z_position -= z_increment;
		myGunship.setPosition(-x_increment,y_increment,-z_increment);
		myCamera.setCamera(camX, y_position+2, camZ, x_position, y_position, z_position);
	}
}
void checkCollision()
{
	for(unsigned int i = 0; i < rings.size(); ++i)
	{
		BoundingBox *gunBox = myGunship.getGunBox();
		if((myGunship.getGunBox())->collision( rings[i]->getMainBox()))//is inside a ring
		{
			if(!(myGunship.getGunBox())->completeCollisionTest( *rings[i]->getSubBoxes()))//always reading false?
			{
				if(!rings[i]->getDone())//is not touching the ring
				{
					score++;
					remaining--;
					//PlaySound(L"levelup.wav", NULL, SND_FILENAME | SND_ASYNC);
					cout << "Current Score = " << score << endl;
					rings[i]->setDone(true);
					switch(remaining)
					{
					case 5:
						cout << "5 left\n";
						PlaySound(L"5left.wav", NULL, SND_FILENAME | SND_ASYNC);
						break;
					case 4:
						cout << "4 left\n";
						PlaySound(L"4left.wav", NULL, SND_FILENAME | SND_ASYNC);
						break;
					case 3:
						cout << "3 left\n";
						PlaySound(L"3left.wav", NULL, SND_FILENAME | SND_ASYNC);
						break;
					case 2:
						cout << "2 left\n";
						PlaySound(L"2left.wav", NULL, SND_FILENAME | SND_ASYNC);
						break;
					case 1:
						cout << "1 left\n";
						PlaySound(L"1left.wav", NULL, SND_FILENAME | SND_ASYNC);
						break;
					case 0:
						cout << "You're done\n";
						PlaySound(L"Congratz.wav", NULL, SND_FILENAME | SND_ASYNC);
						system("PAUSE");
						exit(1);
						break;
					default:
						break;
					}

				}
			}
			else
			{
				/*PlaySound(L"bomb.wav", NULL, SND_FILENAME | SND_ASYNC);
				cout << "crash\n";*/
				//system("PAUSE");
				//exit(1);
			}
		}
	}
	for(unsigned int i = 0; i < clouds.size(); ++i)
	{
		/*if( ((clouds[i]->getMainBox())->collisionB( &(myGunship.getGunBox())->min)) || 
			((clouds[i]->getMainBox())->collisionB( &(myGunship.getGunBox())->max)) )	*/
	
		if((myGunship.getGunBox())->collision( clouds[i]->getMainBox()))
		{
			PlaySound(L"bomb.wav", NULL, SND_FILENAME | SND_ASYNC);
			cout <<"crash\n";
			score--;
			animationExplosion = true;
			if(animationExplosion)
				glutIdleFunc(animateExplosion);
			//(clouds[i]->setCrash(true));
			
			myGunship.currentLocation.setDimension(-1.5,1.5, -.5, .5, -.925,.925);
			/*x_increment = 0.0f;
			y_increment = 0.0f;
			z_increment = 0.0f;
			camX = 0.0f;
			camY = 0.0f;
			camZ = 0.0f;
			x_position=0.0f;
			y_position=0.0f;
			z_position=0.0f;
			*/
			animationMove = false;
			
		//	glutIdleFunc(display);
			myCamera.setCamera(camX, y_position+2, camZ, x_position, y_position, z_position);
		//}	//system("PAUSE");
		//exit(1);
		}
	}
	if(y_position<=-9.5)
	{
		PlaySound(L"bomb.wav", NULL, SND_FILENAME | SND_ASYNC);
		cout <<"crash\n";
		score--;
		animationExplosion = true;
		if(animationExplosion)
			glutIdleFunc(animateExplosion);
		//(clouds[i]->setCrash(true));
		
		myGunship.currentLocation.setDimension(-1.5,1.5, -.5, .5, -.925,.925);
		/*x_increment = 0.0f;
		y_increment = 0.0f;
		z_increment = 0.0f;
		camX = 0.0f;
		camY = 0.0f;
		camZ = 0.0f;
		x_position=0.0f;
		y_position=0.0f;
		z_position=0.0f;
		*/
		animationMove = false;
		
	//	glutIdleFunc(display);
		myCamera.setCamera(camX, y_position+2, camZ, x_position, y_position, z_position);
	}

}
void display(void)
{
	animateMove();
	camX = camDistance*(sin(pi/180*camAngle)) + x_position;
	//camY = camDistance*(sin(PI/180*deg3CamRotateZ)) + transY;
	camZ = camDistance*(cos(pi/180*camAngle)) + z_position;

	myCamera.setCamera(camX, y_position+2, camZ, x_position, y_position, z_position);
	myCamera.lookAt();

	glClearColor(0.0,0.0,1.0,1.0);//blue sky

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(modelMode==true)
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL );
	/*
	the sun
	*/
	GLfloat theSun[] = {0.0,30.0,0.0,0.0};
	GLfloat theSunColor[] = {.9,1.0,0.1,0.0};
	glLightfv(GL_LIGHT0,GL_POSITION,theSun);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,theSunColor);
	if(theSunOn == true)
	{
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
	}
	else
		glDisable(GL_LIGHT0);

	/*
	turn on stuff
	*/
	enable();

	/*
	draw ground
	*/
	glPushMatrix();
		Ground *theGround = new Ground(grassT, rockT);
		theGround->drawGround();
	glPopMatrix();

	/*
	first ring
	*/
	glPushMatrix();
		int zmove = -15;
		glTranslatef(0,0,-30);
		glRotatef(90,1,0,0);
		aRing.drawRing();
	glPopMatrix();
	glPushMatrix();
		//zmove = -30;
		glTranslatef(0,0,-90);
		glRotatef(90, 1, 0,0);
		bRing.drawRing();
	glPopMatrix();
	glPushMatrix();
		//zmove = -7;
		glTranslatef(30,0,-60);
		glRotatef(90, 1, 0,0);
		cRing.drawRing();
	glPopMatrix();
	glPushMatrix();
		//zmove = 5;
		//int xmove = 5;
		glTranslatef(30,0,40);
		glRotatef(90, 1, 0,0);
		dRing.drawRing();
	glPopMatrix();
	glPushMatrix();
		//zmove = -10;
		//xmove = -10;
		glTranslatef(-30,0,120);
		glRotatef(90, 1, 0,0);
		glRotatef(90,0,1,0);
		eRing.drawRing();
	glPopMatrix();
	/*
	the gunship
	*/
	if(animationExplosion == false)
	{
		glPushMatrix();
			glTranslatef(x_position, y_position, z_position);
			glRotatef(x_angle,1,0,0);
			glRotatef(y_angle,0,1,0);
			glRotatef(z_angle,0,0,1);
			myGunship.drawGunship();
		glPopMatrix();
	}
	else
	{
		glPushMatrix();
			glTranslatef(x_position, y_position, z_position);
			glRotatef(x_angle,1,0,0);
			glRotatef(y_angle,0,1,0);
			glRotatef(z_angle,0,0,1);
			myGunship.drawExplosion(explosionForce);
		glPopMatrix();
	}
	/*
	some clouds
	*/
	glPushMatrix();
		glTranslatef(0,0,-60);
		aCloud.drawCloud();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(30,0,0);
		glRotatef(90,1,1,0);
		bCloud.drawCloud();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0,0,60);
		glRotatef(45,1,0,1);
		cCloud.drawCloud();
	glPopMatrix();
	/*
	check if you're in a ring
	*/
	if(ringsSet==false)
	{
		setRings();
		ringsSet=true;
	}
	checkCollision();
	glFlush();
}
void keyboard (unsigned char key, int x, int y)
{
	switch(key)
	{
	//rotate model
	case 'w':
		if(x_angle>-45)
			//x_angle-=1;
			myGunship.setRotate(-1.0,0.0,0.0);
		//orientMe(1,0);
		break;
	case 's':
		if(x_angle<45)
			myGunship.setRotate(1.0,0.0,0.0);
			//x_angle+=1;
		//orientMe(-1,0);
		break;
	case 'a':
		myGunship.setRotate(0.0,1.0,0.0);
		//y_angle+=1;	
		break;
	case 'd':
		myGunship.setRotate(0.0,-1.0,0.0);
		//y_angle-=1;	
		break;
	case 'q':
		if(z_angle<45)
			myGunship.setRotate(0.0,0.0,1.0);
			//z_angle+=1;
		break;
	case 'e':
		if(z_angle>-45)
			myGunship.setRotate(0.0,0.0,-1.0);
			//z_angle-=1;
		break;
	//solid to wire and vise versa
	case 'M':
		if(modelMode==true)
			modelMode=false;
		else
			modelMode=true;
		glutPostRedisplay();
		break;
	//turn on the propellers
	case '1':
		if(myGunship.getPropellerState())
			myGunship.setPropeller(false);
		else
			myGunship.setPropeller(true);
		break;
	//start flying
	case '2':
		myGunship.setMovement(false);
		//zeye-=propellerRotateAngleInc;
		//zlook-=propellerRotateAngleInc;
		//init();
		break;
	//fog toggle
	case 'F':
		glEnable(GL_FOG);
		glFogfv(GL_FOG_COLOR, atmoColor);
		glFogi(GL_FOG_MODE,GL_EXP);
		glFogi(GL_FOG_DENSITY,fogIntense);
		break;
	case 'f':
		glDisable(GL_FOG);
		break;
	//toggle the sun
	case 'l':
		if(theSunOn==true)
			theSunOn=false;
		else
			theSunOn=true;
		break;
    case 32://Move the Helicopter
       

		animationMove = !animationMove;

        if (animationMove);
            //glutIdleFunc(animateMove);
        else;
			//glutIdleFunc(NULL);

		glutPostRedisplay();

        break;

	//exit
	case 27:
	exit(0);
		break;
	default:
		break;
	}
}

void releasekey(unsigned char key, int x, int y)
{
	switch(key)
	{
	case '2':
		myGunship.setMovement(false);
		break;
	case '3':
		camMove = false;
		break;
	default:
		//myGunship.setRotate(0.0,0.0,0.0);
		/*x_angle=x_angle;
		y_angle=y_angle;
		z_angle=z_angle;*/
		break;
	}
}

void processSpecialKeys(int key, int x, int y)
{
	float moveAmount = 2.5f;
    switch(key)
    {
		case GLUT_KEY_RIGHT: //Steer Right

		y_angle += -moveAmount;
		camAngle += -moveAmount;

        glutPostRedisplay();
        break;


		case GLUT_KEY_LEFT: //Steer Left

		y_angle += moveAmount;
		camAngle += moveAmount;

        glutPostRedisplay();
        break;

		case GLUT_KEY_DOWN://Rotate down.

		if(x_angle > -45.0)  
		{   
			x_angle += -moveAmount;

			glutPostRedisplay();
		}
		break;
	
		case GLUT_KEY_UP://Rotate up.

		if(x_angle < 45.0)  
		{   
			x_angle += moveAmount;

			glutPostRedisplay();
		}
		break;
	}
	
}
void fpsDisp(void)
{/*
	if(time-timebase>1000)
	{
		double fps=frame*1000.0/(time-timebase);
		sprintf(s,"FPS:%4.2f",fps);
		timebase = time;
		frame = 0;
	}*/
}


int main(int argc, char** argv)
{
	PlaySound(L"Intro.wav", NULL, SND_FILENAME | SND_ASYNC);
	//playSound();
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DEPTH| GLUT_RGB);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(winWidth,winHeight);	
	mainWindow = glutCreateWindow ("Game");
	
	

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(winReshapeFcn);

	glutKeyboardFunc(keyboard);
	//glutKeyboardUpFunc(releasekey);
	glutSpecialFunc(processSpecialKeys);
	init();
	
	glutMainLoop();
	return 0;
}

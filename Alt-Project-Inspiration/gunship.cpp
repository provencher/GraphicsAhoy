#include "gunship.h"
#include "glut.h"
#include <math.h>

//movement variables
float gunshipMovementX = 0.0f;
float gunshipMovementY = 0.0f;
float gunshipMovementZ = 0.0f;

float gunshipMovementInc = .0025f;//how fast to move it forward.
bool movingFor = false;

float	x_angle=0.0f,
		y_angle=0.0f,
		z_angle=0.0f;

float	x_position = 0.0f,
		y_position = 0.0f,
		z_position = 0.0f;

static float pi = 3.14159265;

//propeller actions
bool propellerRotating = true;
float propellerRotateAngle = 0.0f;
float propellerRotateAngleInc = 0.05f;
float blurSamples = 4.0f;
//some headlight stuff
GLfloat noAmbient[] = {0.0, 0,0, 0.2, 1.0};
GLfloat headlightColor[] = {1.0,1.0,1.0,1.0};
float headlightCut = 45.0,headlightExp = 50.0;
//material variables
bool materialStatus = false;
Gunship::Gunship()
{
	currentLocation.setDimension(-1.5,1.5, -.5, .5, -.925,.925);
	//gunship();
}
void Gunship::drawGunship()
{
	glPushMatrix();
		if(materialStatus==true)
		{//brass
			GLfloat diffuse[] = {1.0f,1.0f,1.0f,1.0f};
			GLfloat mat_ambient[] = {0.329412f,0.329412,0.027451f,1.0f};
			GLfloat mat_diffuse[] = {0.780392f, 0.568627f, 0.113725f, 1.0f};
			GLfloat mat_specular[] = {0.992157f, 0.941176f, 0.807843f, 1.0f};
			GLfloat mat_shininess = 27.8;
			glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
			glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
		}
		else
		{//copper
			GLfloat diffuse[] = {1.0f,1.0f,1.0f,1.0f};
			GLfloat mat_ambient[] = {0.19125f,0.0735f,0.0225f,1.0f};
			GLfloat mat_diffuse[] = {0.7038f, 0.27048f, 0.0828f, 1.0f};
			GLfloat mat_specular[] = {0.256777f, 0.137622f, 0.086014f, 1.0f};
			GLfloat mat_shininess = .1;
			glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
			glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
		}
		gunship();
	glPopMatrix();
}
void Gunship::propeller(float length, float height, float width)
{
	length=length*.75;
	height=height*.4;
	glPushMatrix();
		glColor3f(1.0,0.0,1.0);//pink!
		//1
		glPushMatrix();
		//	glTranslatef(length/2,0,0);
			glScalef(length, height, width);
			glutSolidCube(1);
		glPopMatrix();
		glColor3f(.5,.65,.2);
		glPushMatrix();
			glTranslatef(-length/2,0,0);
			glScalef(.05,.05,.05);
			glutSolidCube(1);
		glPopMatrix();
	glPopMatrix();	
}
void Gunship::propellers(float length, float width, float height)
{
	/*
	the for loop is used to put things on the accumulation buffer to make it 
	appear to be blurring as the propellers spin
	*/
	for(unsigned int i = 0; i < blurSamples; ++i)
	{
		glRotatef(propellerRotateAngle,0,0,1);
		glPushMatrix();	

			glPushMatrix();
				glTranslatef(length/2,0,0);
				propeller(length,height,width);
			glPopMatrix();

			glPushMatrix();
				glRotatef(120,0,0,1);
				glTranslatef(length/2,0,0);
				propeller(length,height,width);
			glPopMatrix();

			glPushMatrix();	
				glRotatef(240,0,0,1);
				glTranslatef(length/2,0,0);
				propeller(length,height,width);
			glPopMatrix();


			glColor3f(1,.5,0);
			
			
			glPushMatrix();
				GLUquadricObj *center;
				center = gluNewQuadric();
				//gluQuadricDrawStyle(center,GLU_FILL);
				gluCylinder(center,width,width,height,15,1);
			glPopMatrix();


		glPopMatrix();
		if(propellerRotating==true)
			propellerRotateAngle+=propellerRotateAngleInc;
		else
			propellerRotateAngle=propellerRotateAngle;
		/*if(i==0)
			glAccum(GL_LOAD, 1.0f/blurSamples);
		else
			glAccum(GL_ACCUM, 1.0f/blurSamples);*/
		glAccum(GL_ACCUM, 1.0f/blurSamples);
	}
	glAccum(GL_RETURN, 1.0);
}
void Gunship::rotor(float radius,float rotation)
{
	float height = .15;
	float detail = 1000;
	float cylRadius = radius;
	glRotatef(rotation,0,1,0);
	glPushMatrix();
		GLUquadricObj *cylinder; 
		glPushMatrix();
			cylinder = gluNewQuadric();
			//gluQuadricDrawStyle(cylinder,GLU_FILL);
			gluCylinder(cylinder,cylRadius,cylRadius,height,detail,5);	
		glPopMatrix();
		//inserting propellers into the rotor, animations to be added later
		//flipping the second one upside down will make the propellers spin in oposite directions
		glPushMatrix();	
			glTranslatef(0,0,height/2);			
			propellers(cylRadius,.04,.04);
		glPopMatrix();
		glPushMatrix();
			glRotatef(180,0,1,0);
			glRotatef(100,0,0,1);
			glTranslatef(0,0,-height/2);
			propellers(cylRadius,.04,.04);
		glPopMatrix();
	
			
	glPopMatrix();

}

void Gunship::tail()
{
	glPushMatrix();
		GLUquadricObj *tail; 
		glPushMatrix();
			glRotatef(-90,1,0,0);
			tail = gluNewQuadric();
			//gluQuadricDrawStyle(tail,GLU_LINE);
			gluCylinder (tail,.25,.125,.45,50,8);	
		glPopMatrix();
		GLUquadricObj *tailB;
		glPushMatrix();
			glTranslatef(0,.45,0);
			glRotatef(-90,1,0,0);
			tailB = gluNewQuadric();
			//gluQuadricDrawStyle(tailB,GLU_LINE);
			gluCylinder (tailB,.125,.025,.45,50,1);
		glPopMatrix();
	/*	GLUquadricObj *cross;
		glPushMatrix();
			glTranslatef(-.25,.9,0);
			glRotatef(90,0,0,1);
			glRotatef(90,1,0,0);
			cross = gluNewQuadric();
			//gluQuadricDrawStyle(cross,GLU_LINE);
			gluCylinder(cross,.125,.125,.5,250,1);
		glPopMatrix();*/
		glPushMatrix();
			glTranslatef(0,.9,0);
			glScalef(.5,.2,.01);
			glutSolidCube(1);
		glPopMatrix();
		glColor3f(0,0,0);
		/*
		i put two small spheres on the top parts of the back of the tail
		they're emissive to appear to be the rear lights on the gunship
		*/
		GLUquadricObj *breakLightA, *breakLightB;
		glPushMatrix();
			glTranslatef(.25,1.1,0.2);
			GLfloat emission[] = {1.0,0.0,0.0,1.0};
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
			breakLightA = gluNewQuadric();
			gluSphere(breakLightA,.01,32,32);
			GLfloat noemission[] = {0.0,0.0,0.0,1.0};
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noemission);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(.25,.9,0);
			glScalef(.01,.4,.4);
			glutSolidCube(1);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-.25,1.1,0.2);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
			breakLightB = gluNewQuadric();
			gluSphere(breakLightB,.01,32,32);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noemission);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-.25,.9,0);
			glScalef(.01,.4,.4);
			glutSolidCube(1);
		glPopMatrix();
	glPopMatrix();
}

void Gunship::cockpit(double scale)
{
	glPushMatrix();
		/*
		put two spotlights onto the front of the cockpit
		*/
		glPushMatrix();
			//headlight 1
			GLfloat lightPos[] = {0.0,0.0,0.0,1.0};//local source at origin
			GLfloat lightLook[] = {0.0,0.0,1.0};//looking at positive z
			
			glLightfv(GL_LIGHT1, GL_AMBIENT, noAmbient);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, headlightColor);
			glLightfv(GL_LIGHT1, GL_POSITION, lightPos);

			glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
			glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.0f);
			glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.0f);

			glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightLook);
			glLightf(GL_LIGHT1, GL_SPOT_CUTOFF,headlightCut);
			glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,headlightExp);
			

			glBegin(GL_TRIANGLE_FAN);
				glVertex3f(0,0,0);//0
				glVertex3f(0,3*scale,-5*scale);//1
				glVertex3f(1*scale,0,-5*scale);//2
				glVertex3f(0,-2*scale,-5*scale);//3
			glEnd();
		glPopMatrix();

		glPushMatrix();
			//headlight 2
			GLfloat light2Pos[] = {-3*scale,0,0,1};
			GLfloat light2Look[] = {0.0,1.0,0.0};//looking at positive z
			glLightfv(GL_LIGHT2, GL_AMBIENT, noAmbient);
			glLightfv(GL_LIGHT2, GL_DIFFUSE, headlightColor);
			glLightfv(GL_LIGHT2, GL_POSITION, light2Pos);

			glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1.0f);
			glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.0f);
			glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.0f);

			glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light2Look);
			glLightf(GL_LIGHT2, GL_SPOT_CUTOFF,headlightCut);
			glLightf(GL_LIGHT2, GL_SPOT_EXPONENT,headlightExp);
						

			glBegin(GL_TRIANGLE_FAN);
				glVertex3f(-3*scale,0,0);
				glVertex3f(-3*scale,3*scale,-5*scale);
				glVertex3f(-4*scale,0,-5*scale);
				glVertex3f(-3*scale,-2*scale,-5*scale);
			glEnd();
		glPopMatrix();
		glPushMatrix();
			glBegin(GL_QUAD_STRIP);
				glVertex3f(0,0,0);//0
				glVertex3f(-3*scale,0,0);//1
				glVertex3f(0,3*scale,-5*scale);//2
				glVertex3f(-3*scale,3*scale,-5*scale);//3
				glVertex3f(0,3*scale,-7.5*scale);
				glVertex3f(-3*scale,3*scale,-7.5*scale);
				glEnd();
		glPopMatrix();
		glPushMatrix();
			glBegin(GL_QUAD_STRIP);
				glVertex3f(0,3*scale,-5*scale);//0
				glVertex3f(0,3*scale,-7.5*scale);//1
				glVertex3f(1*scale,0,-5*scale);//2
				glVertex3f(1*scale,0,-7.5*scale);//3
				glVertex3f(0,-2*scale,-5*scale);//4
				glVertex3f(0,-2*scale,-7.5*scale);//5
				glVertex3f(-3*scale,-2*scale,-5*scale);//6
				glVertex3f(-3*scale,-2*scale,-7.5*scale);//7
				glVertex3f(-4*scale,0,-5*scale);//8
				glVertex3f(-4*scale,0,-7.5*scale);//9
				glVertex3f(-3*scale,3*scale,-5*scale);//10
				glVertex3f(-3*scale,3*scale,-7.5*scale);
			glEnd();
		glPopMatrix();
		
			
	glPopMatrix();
}

void Gunship::leg()
{
	GLUquadricObj *cylinder;		
	glPushMatrix();
		glRotatef(90,1,00,0);
		cylinder = gluNewQuadric();
		//gluQuadricDrawStyle(cylinder,GLU_LINE);
		gluCylinder(cylinder,.01,.01,.5,6,1);
	glPopMatrix();
}
void Gunship::engine()
{
	glPushMatrix();
		glRotatef(90,1,0,0);
		GLUquadricObj *cylinder, *flame;
		glPushMatrix();
			glTranslatef(0,0,0);
			GLfloat emission[] = {1.0,1.0,1.0,1.0};
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
			flame = gluNewQuadric();
			gluDisk(flame,.00,.05,20,20);
			GLfloat noemission[] = {0.0,0.0,0.0,1.0};
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, noemission);
		glPopMatrix();
		
		glPushMatrix();
			
			cylinder = gluNewQuadric();
			//gluQuadricDrawStyle(cylinder,GLU_LINE);
			gluCylinder(cylinder,.05,.05,.3,20,3);
		glPopMatrix();
	glPopMatrix();
}
void Gunship::gunship(void)
{
	glPushMatrix();
		glPushMatrix();
			glRotatef(-270,1,0,0);//orient the gunship the right way
			glRotatef(180,0,1,0);
			//the main body
			glColor3f(1.0,0.0,0.0);
			GLUquadricObj *cylinder;
			glPushMatrix();
				glTranslatef(0,.25,0);
				glRotatef(90,1,0,0);
				cylinder = gluNewQuadric();
				//gluQuadricDrawStyle(cylinder,GLU_LINE);
				gluCylinder(cylinder,.25,.25,.75,25,1);
			glPopMatrix();
			//tail
			glPushMatrix();
				glTranslatef(0,.25,0);
				tail();
			glPopMatrix();
			//legs
			glPushMatrix();
				glTranslatef(.25,0,-.25);
				leg();
				//glScalef(.05,15,.5);
				//glutWireCube(.025);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-.25,0,-.25);
				leg();
				//glScalef(.05,15,.5);
				//glutWireCube(.025);
			glPopMatrix();
			//engines
			glPushMatrix();
				glTranslatef(-0.051,.2,.28);
				engine();
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.051,.2,.28);
				engine();
			glPopMatrix();
			//the two thingamaboppers that hold the rotors onto the jet
			glColor3f(.25,.5,.01);			
			glPushMatrix();
				GLUquadricObj *holder;
				glTranslatef(-.15,0,.1);
				glRotatef(-25,0,1,0);
				holder = gluNewQuadric();
				//gluQuadricDrawStyle(holder,GLU_LINE);
				gluCylinder(holder,.04,.04,.15,20,1);
			glPopMatrix();
			glPushMatrix();
				GLUquadricObj *holder2;
				glTranslatef(.15,0,.1);
				glRotatef(25,0,1,0);

				holder2 = gluNewQuadric();
				//gluQuadricDrawStyle(holder2,GLU_LINE);
				gluCylinder(holder2,.04,.04,.15,20,1);
			glPopMatrix();

			glColor3f(1.0,1.0,0);//yellow
			//Rotors
			//1
			double rotorAngle = 10;
			double rotorHeight = .3;
			glPushMatrix();
				glTranslatef(.8,0,rotorHeight);	
				glRotatef(-rotorAngle,0,1,0);
				rotor(.6,1);
			glPopMatrix();
			//2
			glPushMatrix();
				glTranslatef(-.8,0,rotorHeight);
				glRotatef(rotorAngle,0,1,0);
				rotor(.6,1);
			glPopMatrix();
			//cockpit 
			glPushMatrix();			
				glRotatef(90,1,0,0);
				glTranslatef(.14,-.03,1.16);
				cockpit(.09);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
	if(movingFor==true)
	{
		//gunshipMovement+=gunshipMovementInc;
		/*zeye-=gunshipMovementInc;//should move the camera with the model but...doesn't
		
		
		At(xeye,yeye,zeye,
					xlook,ylook,zlook,
					xup,yup,zup);*/
	}
		
}


void Gunship::setPropeller(bool toSwitch)
{
	propellerRotating = toSwitch;
}
bool Gunship::getPropellerState()
{
	return propellerRotating;
}
void Gunship::setMovement(bool state)
{
	movingFor=state;
}

void Gunship::setPosition(float x, float y, float z)
{
	currentLocation.changeLocation(x,y,z);
	x_position = x;
	y_position = y;
	z_position = z;
}

float Gunship::getXPosition()
{
	return x_position;
}

float Gunship::getYPosition()
{
	return y_position;
}

float Gunship::getZPosition()
{
	return z_position;
}
BoundingBox* Gunship::getGunBox()
{
	return &currentLocation;
}
void Gunship::setRotate(float x, float y, float z)
{
	x_angle+=x;
	y_angle+=y;
	z_angle+=z;
}

float Gunship::getRotateX()
{
	return x_angle;
}

float Gunship::getRotateY()
{
	return y_angle;
}

float Gunship::getRotateZ()
{
	return z_angle;
}
void Gunship::setMaterials(bool state)
{
	materialStatus=state;
}
bool Gunship::getMaterials()
{
	return materialStatus;
}

float Gunship::getGunshipMovementX()
{
	return gunshipMovementX;
}

float Gunship::getGunshipMovementY()
{
	return gunshipMovementY;
}

float Gunship::getGunshipMovementZ()
{
	return gunshipMovementZ;
}
void Gunship::drawExplosion(float i)
{
	glPushMatrix();
		glPushMatrix();
			glRotatef(-270-2*i,1,0,0);//orient the gunship the right way
			glRotatef(1805*i,0,1,0);
			//the main body
			glColor3f(1.0,0.0,0.0);
			GLUquadricObj *cylinder;
			glPushMatrix();
				glTranslatef(0+i,.25,0-3*i);
				glRotatef(90-20*i,1,0,0);
				cylinder = gluNewQuadric();
				//gluQuadricDrawStyle(cylinder,GLU_LINE);
				gluCylinder(cylinder,.25,.25,.75,25,1);
			glPopMatrix();
			//tail
			glPushMatrix();
				glTranslatef(0+3*i,.25+i,0+i*2);
				tail();
			glPopMatrix();
			//legs
			glPushMatrix();
				glTranslatef(.25+i,0,-.25+i*2);
				leg();
				//glScalef(.05,15,.5);
				//glutWireCube(.025);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-.25+i,0+i,-.25+i);
				leg();
				//glScalef(.05,15,.5);
				//glutWireCube(.025);
			glPopMatrix();
			//engines
			glPushMatrix();
				glTranslatef(-0.051-i,.2+i,.28+i*-3);
				engine();
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0.051+i,.2+i,.28+i);
				engine();
			glPopMatrix();
			//the two thingamaboppers that hold the rotors onto the jet
			glColor3f(.25,.5,.01);			
			glPushMatrix();
				GLUquadricObj *holder;
				glTranslatef(-.15,0,.1);
				glRotatef(-25+i*4,0,1,0);
				holder = gluNewQuadric();
				//gluQuadricDrawStyle(holder,GLU_LINE);
				gluCylinder(holder,.04,.04,.15,20,1);
			glPopMatrix();
			glPushMatrix();
				GLUquadricObj *holder2;
				glTranslatef(.15-i,0-i,.1-i);
				glRotatef(25-i,0,1,0);

				holder2 = gluNewQuadric();
				//gluQuadricDrawStyle(holder2,GLU_LINE);
				gluCylinder(holder2,.04,.04,.15,20,1);
			glPopMatrix();

			glColor3f(1.0,1.0,0);//yellow
			//Rotors
			//1
			double rotorAngle = 10;
			double rotorHeight = .3;
			glPushMatrix();
				glTranslatef(.8+i,0-3*i,rotorHeight+i);	
				glRotatef(-rotorAngle+i,0,1,0);
				rotor(.6,1);
			glPopMatrix();
			//2
			glPushMatrix();
				glTranslatef(-.8+i,0-i,rotorHeight+2*i);
				glRotatef(rotorAngle+i,0,1,0);
				rotor(.6,1);
			glPopMatrix();
			//cockpit 
			glPushMatrix();			
				glRotatef(90+i,1,0,0);
				glTranslatef(.14+i,-.03+2*i,1.16+3*i);
				cockpit(.09);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}
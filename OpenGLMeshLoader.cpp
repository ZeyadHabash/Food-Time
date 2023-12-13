
#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <math.h>
#include <glut.h>
#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <stdio.h>
using namespace std;
#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

int WIDTH = 640;
int HEIGHT = 480;

int timer = 3;
int score = 50;

float elapsedTime = 0.0f;

bool rise = true;
bool transition = false;
GLuint tex;
char title[] = "food time";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 500;

GLdouble laserPositionX = 0, laserPositionY = 2, laserPositionZ = 8;
GLdouble laserPositionX2 = 0, laserPositionY2 = 2, laserPositionZ2 = -6;

GLdouble laserOffsetX = -1, laserOffsetY = 0, laserOffsetZ = 0;
GLdouble laserCutoff = 30;
GLdouble laserExponent = 100;
GLdouble laserDirectionX = 1, laserDirectionY = 0, laserDirectionZ = 0;
GLdouble laserColour[] = { 0.0f, 1.0f, 0.0f };
GLdouble laserSpeed = 0.1;




float stopwatchX[4] = { -3, 4, 3, -3 };
float stopwatchY[4] = { 3, 3, 3, 3 };
float stopwatchZ[4] = { 5, 9, -5, -7 };
int stopwatchFlag[4] = { 1, 1, 1, 1 };


float hourGlassX[4] = { -3, 4, 9, -3 };
float hourGlassY[4] = { 1, 1, 1, 1 };
float hourGlassZ[4] = { 5, 9, -5, -7 };
int hourGlassFlag[4] = { 1, 1, 1, 1 };


GLdouble tornadoPositionX = 0, tornadoPositionY = 2, tornadoPositionZ = 8;
GLdouble tornadoPositionX2 = 0, tornadoPositionY2 = 2, tornadoPositionZ2 = -6;
GLdouble tornadoPositionX3 = 0, tornadoPositionY3 = 2, tornadoPositionZ3 = 3;


GLdouble tornadoDirectionX = 1, tornadoDirectionY = 0, tornadoDirectionZ = 0;


GLdouble stopwatchAng = 0;
GLdouble hourGlassAng = 0;
GLdouble tornadoAng = 0;
GLdouble shawermaAng = 0;

bool colTor1 = false;
bool colTor2 = false;
bool colTor3 = false;


GLdouble timeMachineAng = 0;
bool moveLaserleft = false;

bool isThirdPerson = true;

bool scene2 = true; // to check if we are in scene 2 or not

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

Vector Eye(20, 5, 20);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);


class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(const Vector3f& v) const {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(const Vector3f& v) const {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(const float n) const {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(const float n) const {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};


class Camera {
public:
	Vector3f eye, center, up;
	Camera(float eyeX = 1.0f, float eyeY = 6.0f, float eyeZ = 4.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void changeView(float playerX, float playerY, float playerZ, float playerAngle) {
		if (isThirdPerson) {
			if (playerAngle == 180) {
				eye = Vector3f(playerX, playerY + 5.0f, playerZ + 2.7f);
				center = Vector3f(playerX, playerY + 2.5f, playerZ - 5.0f);
			}
			else if (playerAngle == 90) {
				eye = Vector3f(playerX - 2.7f, playerY + 5.0f, playerZ);
				center = Vector3f(playerX + 5.0f, playerY + 2.5f, playerZ);
			}
			else if (playerAngle == 0) {
				eye = Vector3f(playerX, playerY + 5.0f, playerZ - 2.7f);
				center = Vector3f(playerX, playerY + 2.5f, playerZ + 5.0f);
			}
			else if (playerAngle == -90) {
				eye = Vector3f(playerX + 2.7f, playerY + 5.0f, playerZ);
				center = Vector3f(playerX - 5.0f, playerY + 2.5f, playerZ);
			}
			up = Vector3f(0.0f, 1.0f, 0.0f);
		}
		else {
			eye = Vector3f(playerX, playerY + 5.0f, playerZ);
			if (playerAngle == 180) {
				//center = Vector3f(playerX, playerY + 2.5f, playerZ - 5.0f);
				center = Vector3f(playerX, playerY + 2.5f, playerZ - 5.0f);

			}
			else if (playerAngle == 90) {
				//center = Vector3f(playerX + 2.0f, playerY + 4.5f, playerZ);
				center = Vector3f(playerX + 5.0f, playerY + 2.5f, playerZ);

			}
			else if (playerAngle == 0) {
				//center = Vector3f(playerX, playerY + 2.5f, playerZ + 5.0f);
				center = Vector3f(playerX, playerY + 2.5f, playerZ + 5.0f);

			}
			else if (playerAngle == -90) {
				//center = Vector3f(playerX - 2.0f, playerY + 4.5f, playerZ);
				center = Vector3f(playerX - 5.0f, playerY + 2.5f, playerZ);

			}
			up = Vector3f(0.0f, 1.0f, 0.0f);
		}
	}

	void look() {
		gluLookAt(
			eye.x, eye.y, eye.z,
			center.x, center.y, center.z,
			up.x, up.y, up.z
		);
	}

};



Camera camera;
float playerX = 0.0;
float playerY = 0.15;
float playerZ = 0.0;
float playerAngle = 0;

void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look();
	camera.changeView(playerX, playerY, playerZ, playerAngle);
}


int cameraZoom = 0;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;
Model_3DS model_2b;
Model_3DS model_laser;
Model_3DS model_shawerma;
Model_3DS model_timeMachine;
Model_3DS model_stopwatch;
Model_3DS model_tornado;
Model_3DS model_hourglass;
// Textures
GLTexture tex_ground;
GLTexture tex_futurefloor;
GLTexture tex_sand;
GLTexture tex_sandtornado;
GLTexture tex_smooth;
GLTexture tex_metal;
GLTexture tex_sandglass;





//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };

	if (scene2)
	{
		ambient[0] = 0.5;
		ambient[1] = 0.5;
		ambient[2] = 0.5;
	}

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.7, 0.7, 0.7, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);



	if(!scene2)
	{
		// Laser light source (Spotlight)
		glEnable(GL_LIGHT1);

		GLfloat ambient1[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // Minimal ambient light
		glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);

		GLfloat diffuse1[] = { laserColour[0] * 10 , laserColour[1] * 10 , laserColour[2] * 10, 1.0f }; // laser colour
		glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);

		GLfloat specular1[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // White specular light
		glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);

		GLfloat light_position1[] = { laserPositionX + laserOffsetX, laserPositionY + laserOffsetY, laserPositionZ + laserOffsetZ, 1.0f };
		glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

		GLfloat light_direction1[] = { laserDirectionX, laserDirectionY, laserDirectionZ };
		glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light_direction1);

		// Set the cutoff angle for the spotlight
		glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, laserCutoff);

		// Set the exponent to control the spotlight falloff
		glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, laserExponent);

		GLfloat constantAttenuation = 1.0f;
		GLfloat linearAttenuation = 0.1f;
		GLfloat quadraticAttenuation = 0.01f;

		glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, constantAttenuation);
		glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, linearAttenuation);
		glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);




		glEnable(GL_LIGHT2);

		glLightfv(GL_LIGHT2, GL_AMBIENT, ambient1);

		glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse1);

		glLightfv(GL_LIGHT2, GL_SPECULAR, specular1);

		GLfloat light_position2[] = { laserPositionX2 + laserOffsetX, laserPositionY2 + laserOffsetY, laserPositionZ2 + laserOffsetZ, 1.0f };
		glLightfv(GL_LIGHT2, GL_POSITION, light_position2);

		glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_direction1);

		// Set the cutoff angle for the spotlight
		glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, laserCutoff);

		// Set the exponent to control the spotlight falloff
		glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, laserExponent);



		glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, constantAttenuation);
		glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, linearAttenuation);
		glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);


	}


}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing


	//glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	if (scene2) {
		glBindTexture(GL_TEXTURE_2D, tex_sand.texture[0]);	// Bind the ground texture
	}
	else {
		glBindTexture(GL_TEXTURE_2D, tex_futurefloor.texture[0]);	// Bind the ground texture
	}

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}


//if flag = 1, we add to the score --> collectables collision
// otherwise, we deduct from score --> obstacle collision
//otherwise deduct 




void checkCollision() {
	if (!scene2) {
		//collision with the stopwatch 
		for (int i = 0; i < 4; i++) {
			if (playerX >= stopwatchX[i] - 0.5 && playerX <= stopwatchX[i] + 0.5 &&
				playerZ >= stopwatchZ[i] - 0.5 && playerZ <= stopwatchZ[i] + 0.5)
				if (stopwatchFlag[i] == 1) {
					score += 10;
					stopwatchFlag[i] = 0;
					PlaySound(TEXT("stopwatch.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
		}

		//collisions with laser beam 
		if (playerX <= laserPositionX + 8 && playerX >= laserPositionX - 8)
		{
			if ((playerZ <= laserPositionZ + 0.5 && playerZ >= laserPositionZ - 0.5)
				|| (playerZ <= laserPositionZ2 + 0.5 && playerZ >= laserPositionZ2 - 0.5))
			{
				if (score >= 5)
					score -= 5;

				PlaySound(TEXT("laser.wav"), NULL, SND_FILENAME | SND_ASYNC);

				if (playerAngle == 0)
					playerZ -= 0.2;
				else
					playerZ += 0.2;

			}

		}
		//collision with timemachine
		if (playerZ >= 12.5 && playerX <= -5.2 && playerX >= -10.5) {
			if (!scene2) {
				PlaySound(TEXT("timemachine.wav"), NULL, SND_FILENAME | SND_ASYNC);
				transition = true;
			}
		}
	}
	else {
		//collision with the hourglass 
		for (int i = 0; i < 4; i++) {
			if (playerX >= hourGlassX[i] - 0.5 && playerX <= hourGlassX[i] + 0.5 &&
				playerZ >= hourGlassZ[i] - 0.5 && playerZ <= hourGlassZ[i] + 0.5)
				if (hourGlassFlag[i] == 1) {
					score += 10;
					hourGlassFlag[i] = 0;
					PlaySound(TEXT("stopwatch.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
		}

		colTor1 = false;
		colTor2 = false;
		colTor3 = false;

		//collisions with tornado
		bool torRotate = false;
		if ((playerZ <= tornadoPositionZ + 0.5 && playerZ >= tornadoPositionZ - 0.5
			&& playerX <= tornadoPositionX + 0.5 && playerX >= tornadoPositionX - 0.5))
			colTor1 = true;
		else if ((playerZ <= tornadoPositionZ2 + 0.5 && playerZ >= tornadoPositionZ2 - 0.5
			&& playerX <= tornadoPositionX2 + 0.5 && playerX >= tornadoPositionX2 - 0.5))
			colTor2 = true;
		else if
			((playerZ <= tornadoPositionZ3 + 0.5 && playerZ >= tornadoPositionZ3 - 0.5
				&& playerX <= tornadoPositionX3 + 0.5 && playerX >= tornadoPositionX3 - 0.5))
			colTor3 = true;

			//cout << "tornado" << endl;
			if (colTor1 || colTor2 || colTor3)
			{
				
				score -= 5;
				torRotate = true;
			}


			if (torRotate)
				playerAngle += 0.2;
				/*if (playerAngle == 0) {
					playerAngle += 0.3;
					playerZ -= 0.2;

				}
				else {
					playerAngle += 0.3;
					playerZ += 0.2;


				}*/

			

		
	}
}
//scene 1 methods
void print(float x, float y, float z, char* string)
{
	int len, i;

	//set the position of the text in the window using the x and y coordinates
	glRasterPos3f(x, y, z);

	//get the length of the string to display
	len = (int)strlen(string);

	//loop to display character by character
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, string[i]);
	}
}
void drawLaser(GLdouble laserPositionX, GLdouble laserPositionY, GLdouble laserPositionZ)
{
	glPushMatrix();
	glPushAttrib(GL_CURRENT_BIT); // Save the current color state
	// Enable color material
	glColor3f(laserColour[0], laserColour[1], laserColour[2]);
	glTranslatef(laserPositionX, laserPositionY, laserPositionZ);
	glScalef(0.1, 0.05, 0.05);
	model_laser.Draw();
	glPopAttrib(); // Restore the color state
	glPopMatrix();
}
void drawStopwatch(float x, float y, float z, int flag) {
	if (flag == 0)
		return;

	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(stopwatchAng, 0, 1, 0);
	glScalef(0.02, 0.02, 0.02);
	model_stopwatch.Draw();
	glPopMatrix();


}
void drawTimemachine() {
	glPushMatrix();
	glTranslatef(-8, 0.5, 16);
	glRotatef(timeMachineAng, 0, 1, 0);
	glScalef(0.02, 0.02, 0.02);
	model_timeMachine.Draw();
	glPopMatrix();

}

//scene 2 methods

void drawTornado(GLdouble tornadoPositionX, GLdouble tornadoPositionY, GLdouble tornadoPositionZ)
{
	glPushMatrix();
	//glPushAttrib(GL_CURRENT_BIT); // Save the current color state
	// Enable color material
	//glColor3f(laserColour[0], laserColour[1], laserColour[2]);
	glTranslatef(tornadoPositionX, tornadoPositionY, tornadoPositionZ);
	glRotatef(tornadoAng, 0, 1, 0);

	glScalef(0.02, 0.02, 0.02);

	model_tornado.Draw();
	//glPopAttrib(); // Restore the color state
	glPopMatrix();
}
void drawHourGlass(float x, float y, float z, int flag) {
	if (flag == 0)
		return;

	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(hourGlassAng, 0, 1, 0);
	glScalef(0.02, 0.02, 0.02);
	model_hourglass.Draw();
	glPopMatrix();


}

void DisplayScene1(void) {
	setupCamera();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




	//GLfloat lightIntensity[] = { 0.2, 0.05, 0.05, 1.0f };
	//GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
	InitLightSource();

	// Draw Ground
	RenderGround();


	// Draw 2b Model
	glPushMatrix();
	glTranslatef(playerX, playerY, playerZ);
	glRotatef(playerAngle, 0, 1, 0);
	glScalef(0.05, 0.05, 0.05);
	model_2b.Draw();
	glPopMatrix();

	// Draw laser Model
	drawLaser(laserPositionX, laserPositionY, laserPositionZ);
	drawLaser(laserPositionX2, laserPositionY2, laserPositionZ2);


	//Draw timeMachine model
	drawTimemachine();



	//draw stopwatches	
	for (int i = 0; i < 4; i++) {
		drawStopwatch(stopwatchX[i], stopwatchY[i], stopwatchZ[i], stopwatchFlag[i]);
	}

	//sky box
	glPushMatrix();

	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);


	glPopMatrix();


	//score of the game
	glColor3f(1, 1, 1);
	char* p0s[20];
	sprintf((char*)p0s, "Score: %d", score);
	
	print(camera.eye.x, 5.6, camera.eye.z, (char*)p0s);

	glutSwapBuffers();
	glFlush();
}

void DisplayScene2(void) {
	setupCamera();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




	//GLfloat lightIntensity[] = { 0.2, 0.05, 0.05, 1.0f };
	//GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
	InitLightSource();

	// Draw Ground
	RenderGround();

	// Draw 2b Model
	glPushMatrix();
	
	//if (colTor1)
	//{
	//	glTranslatef(-tornadoPositionX, -tornadoPositionX, -tornadoPositionX);
	//	glRotatef(playerAngle, 0, 1, 0);
	//	glTranslatef(tornadoPositionX, tornadoPositionX, tornadoPositionX);
	//}
	//if (colTor1) {
	//	//playerX -= tornadoPositionX;
	//	//playerY -= tornadoPositionY;
	//	//playerZ -= tornadoPositionZ;
	//		//glRotatef(playerAngle, 0, 1, 0);
	//		//playerX += tornadoPositionX;
	//		//playerY += tornadoPositionY;
	//		//playerZ += tornadoPositionZ;



	//}
	glTranslatef(playerX, playerY, playerZ);
	glRotatef(playerAngle, 0, 1, 0);
	glScalef(0.05, 0.05, 0.05);
	model_2b.Draw();
	glPopMatrix();


	// Draw tornado Model
	drawTornado(tornadoPositionX, tornadoPositionY, tornadoPositionZ);
	drawTornado(tornadoPositionX2, tornadoPositionY2, tornadoPositionZ2);
	drawTornado(tornadoPositionX3, tornadoPositionY3, tornadoPositionZ3);



	// Draw shawerma Model
	glPushMatrix();
	glTranslatef(10, 2, 0);
	glRotatef(shawermaAng, 0, 1, 0);
	glScalef(0.05, 0.05, 0.05);
	model_shawerma.Draw();
	glPopMatrix();

	//Draw Hour glass
	for (int i = 0; i < 4; i++) {
		drawHourGlass(hourGlassX[i], hourGlassY[i], hourGlassZ[i], hourGlassFlag[i]);
	}

	//sky box
	glPushMatrix();

	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);

	glPopMatrix();


	//score of the game
	glColor3f(1, 1, 1);
	char* p0s[20];
	sprintf((char*)p0s, "Score: %d", score);
	if(isThirdPerson)
	print(playerX, 5.6, playerZ, (char*)p0s);
	else
	print(camera.eye.x, camera.eye.y + 3.5, camera.eye.z, (char*)p0s);

	glutSwapBuffers();
	glFlush();
}

//=======================================================================
// Display Function
//=======================================================================
void Display(void)
{
	if (!scene2)
		DisplayScene1();
	else
		DisplayScene2();

}


void Keyboard(unsigned char key, int x, int y) {

	switch (key) {
	case 's':
		playerAngle = 180;
		if (playerZ > -17) {
			playerZ -= 0.2;
		}
		break;
	case 'd':
		playerAngle = -90;
		if (playerX > -17) {
			playerX -= 0.2;
		}
		break;
	case 'w':
		playerAngle = 0;
		if (playerZ < 17) {
			playerZ += 0.2;
		}
		break;
	case 'a':
		playerAngle = 90;
		if (playerX < 17) {
			playerX += 0.2;
		}
		break;
	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
		break;
	}
	camera.changeView(playerX, playerY, playerZ, playerAngle);

	cout << "playerX" << playerX << endl;
	cout << "playerY" << playerY << endl;
	cout << "playerZ" << playerZ << endl;


	//method to check for collisions with collectables/borders
	checkCollision();
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		isThirdPerson = !isThirdPerson;
		camera.changeView(playerX, playerY, playerZ, playerAngle);
		glutPostRedisplay();
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		scene2 = !scene2;
	}
}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	model_2b.Load("Models/2b-nier-automata/source/2brigged/2bniernew.3DS");
	model_shawerma.Load("Models/shawerma/shawerma.3DS");
	model_laser.Load("Models/futuristic-sci-fi-laser-barrier/source/laserBarrier.3DS");
	model_timeMachine.Load("Models/timeMachine/timeMachine.3ds");
	model_stopwatch.Load("Models/stopwatch/watch.3ds");
	model_tornado.Load("Models/tornado/tornado.3ds");
	model_hourglass.Load("Models/hourglass/hourglass.3ds");

	// Loading texture files
	tex_ground.Load("Textures/ground.bmp");
	tex_futurefloor.Load("Textures/futurefloor.bmp");
	tex_sand.Load("Textures/sand.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
}

void anim()
{
	if (!scene2)
		//laser animation
	{
		if (moveLaserleft)
		{
			laserPositionX += 0.01;
			laserPositionX2 += 0.01;

		}
		else
		{
			laserPositionX -= 0.01;
			laserPositionX2 -= 0.01;

		}

		if (laserPositionX >= 10 || laserPositionX <= -10)
		{
			moveLaserleft = !moveLaserleft;
			float temp = laserColour[0];
			laserColour[0] = laserColour[1];
			laserColour[1] = laserColour[2];
			laserColour[2] = temp;
		}


		//ttansition animation
		float animationDuration = 40.0f;
		if (transition && (elapsedTime < animationDuration)) {
			if (rise)
			{
				playerY += 0.05;
				elapsedTime += 0.01;

				if (playerY > 6)
					rise = false;
			}
			else
				playerAngle += 0.3;
			elapsedTime += 0.01;
		}
		else if ((elapsedTime > animationDuration))
		{
			playerX = 0.0;
			playerY = 0.15;
			playerZ = 0.0;
			playerAngle = 0;
			scene2 = true;

		}

		//stopwatch animation
		stopwatchAng += 0.1;

		//timemachine animation
		timeMachineAng += 0.1;

	}
	else {
		//shawerma animation
		shawermaAng += 0.1;

		// hourglass animation
		hourGlassAng += 0.1;

		// tornado animation
		tornadoAng += 0.1;

		if (colTor1 || colTor2 || colTor3)
			playerAngle += 0.5;
	}

	glutPostRedisplay();
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{


	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);

	glutCreateWindow(title);

	glutDisplayFunc(Display);

	glutKeyboardFunc(Keyboard);
	//glutMotionFunc(myMotion);

	glutIdleFunc(anim);

	glutMouseFunc(Mouse);

	//glutReshapeFunc(myReshape);


	myInit();

	LoadAssets();
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_NORMALIZE);
	//glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}
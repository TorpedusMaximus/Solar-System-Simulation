﻿#define _USE_MATH_DEFINES
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <thread>
using namespace std;

typedef float point3[3];
typedef float point9[9];

static GLfloat viewer[] = { 100.0, 500.0, 100.0 , 0.0, 200.0 , 0.0 };
static GLfloat azymuth = 0;
static GLfloat elevation = 0;
static GLfloat azymuth2 = 0;
static GLfloat elevation2 = 0;

static int x_pos_old = 0;
static int delta_x = 0;
static int y_pos_old = 0;
static int delta_y = 0;
static GLfloat pix2angle;

static GLint statusMiddle = 0;
static GLint statusLeft = 0;
static GLint statusRight = 0;

int r = 10;
int days[] = { 88,224,365,686,4333,10756,30707, 60223 };
int speed = 1;
long long day = 0;
time_t start;
int radius[] = { 57,108,149,227,778,1433,2872,4498 };
float planetSize[] = { 4.87, 12.1, 12.76, 6.79, 71, 60, 25, 24, 30 };
bool simulation = false;
float ring[3600];
int segments = 10000;
int rings[] = { 102,102.3,102.6,103,103.8,104,110,115,119,125,132,136,141,146,149,155,156,158,160,165 };
GLbyte* textures[10];
GLint ImWidth[10], ImHeight[10], ImComponents[10];
GLenum ImFormat[10];

bool show = true;
bool click = false;


void texture(int textureID);

void orbit(int planet) {
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 360; i++) {
		float angle = 1.0 * float(i) / float(360);
		float x = planet * cos(2 * M_PI * angle);
		float y = planet * sin(2 * M_PI * angle);

		glVertex3f(x, 0, y);
	}
	glEnd();
}

void orbits() {
	texture(9);
	for (int planet : radius) {
		orbit(planet);
	}
}

void sun() {
	texture(8);
	glutSolidSphere(planetSize[8], segments, segments);
}

void mercury() {
	float angle = 1.0 * day / days[0];
	float x = cos(2 * angle * M_PI) * radius[0];
	float y = -1 * sin(2 * angle * M_PI) * radius[0];

	glTranslatef(x, 0, y);
	texture(0);
	glutSolidSphere(planetSize[0], segments, segments);
	glTranslatef(-x, 0, -y);
}

void venus() {
	float angle = 1.0 * day / days[1];
	float x = cos(2 * angle * M_PI) * radius[1];
	float y = -1 * sin(2 * angle * M_PI) * radius[1];

	glTranslatef(x, 0, y);
	texture(1);
	glutSolidSphere(planetSize[1], segments, segments);
	glTranslatef(-x, 0, -y);
}

void earth() {
	float angle = 1.0 * day / days[2];
	float x = cos(2 * angle * M_PI) * radius[2];
	float y = -1 * sin(2 * angle * M_PI) * radius[2];

	glTranslatef(x, 0, y);
	texture(2);
	glutSolidSphere(planetSize[2], segments, segments);
	glTranslatef(-x, 0, -y);
}

void mars() {
	float angle = 1.0 * day / days[3];
	float x = cos(2 * angle * M_PI) * radius[3];
	float y = -1 * sin(2 * angle * M_PI) * radius[3];

	glTranslatef(x, 0, y);
	texture(3);
	glutSolidSphere(planetSize[3], segments, segments);
	glTranslatef(-x, 0, -y);
}

void jupiter() {
	float angle = 1.0 * day / days[4];
	float x = cos(2 * angle * M_PI) * radius[4];
	float y = -1 * sin(2 * angle * M_PI) * radius[4];

	glTranslatef(x, 0, y);
	texture(4);
	glutSolidSphere(planetSize[4], segments, segments);
	glTranslatef(-x, 0, -y);
}

void saturn() {
	float angle = 1.0 * day / days[5];
	float x = cos(2 * angle * M_PI) * radius[5];
	float y = -1 * sin(2 * angle * M_PI) * radius[5];

	glTranslatef(x, 0, y);
	texture(5);
	glutSolidSphere(planetSize[5], segments, segments);
	glTranslatef(-x, 0, -y);


	for (int i : rings) {
		glColor3f(1, 1, 1);
		glBegin(GL_LINE_LOOP);
		for (int ii = 0; ii < 3600; ii++) {
			float angle = 1.0 * float(ii) / float(3600);
			float xr = i * cos(2 * M_PI * angle);
			float yr = i * sin(2 * M_PI * angle);

			glVertex3f(xr + x, (i / 5) * ring[ii], yr + y);
		}
		glEnd();
	}


}

void uranus() {
	float angle = 1.0 * day / days[6];
	float x = cos(2 * angle * M_PI) * radius[6];
	float y = -1 * sin(2 * angle * M_PI) * radius[6];

	glTranslatef(x, 0, y);
	texture(6);
	glutSolidSphere(planetSize[6], segments, segments);
	glTranslatef(-x, 0, -y);
}

void neptune() {
	float angle = 1.0 * day / days[7];
	float x = cos(2 * angle * M_PI) * radius[7];
	float y = -1 * sin(2 * angle * M_PI) * radius[7];

	glTranslatef(x, 0, y);
	texture(7);
	glutSolidSphere(planetSize[7], segments, segments);
	glTranslatef(-x, 0, -y);
}

void zoom(bool zoom) {
	float x = (viewer[3] - viewer[0]);
	float y = (viewer[4] - viewer[1]);
	float z = (viewer[5] - viewer[2]);
	if (zoom) {
		x *= -1;
		y *= -1;
		z *= -1;
	}
	viewer[0] += x;
	viewer[1] += y;
	viewer[2] += z;
	viewer[3] += x;
	viewer[4] += y;
	viewer[5] += z;
}


void ukladSloneczny() {
	if (statusRight == 1) {
		zoom(delta_y > 0);
	}

	if (statusLeft == 1) {
		azymuth += delta_x * pix2angle * 0.01;
		elevation += delta_y * pix2angle * 0.01;
	}
	viewer[3] = 10 * cos(azymuth) * cos(elevation) + viewer[0];
	viewer[4] = 10 * sin(elevation) + viewer[1];
	viewer[5] = 10 * sin(azymuth) * cos(elevation) + viewer[2];

	orbits();
	sun();
	mercury();
	venus();
	earth();
	mars();
	jupiter();
	saturn();
	uranus();
	neptune();
}

void printDay() {
	const int characters = 80;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1000, 0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	texture(9);
	glRasterPos2i(10, 970);
	char printDay[characters];
	sprintf_s(printDay, " Day: %d", day);
	for (int i = 0; i < characters; ++i) {
		glColor3f(1, 1, 1);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, printDay[i]);
	}

	glRasterPos2i(10, 945);
	sprintf_s(printDay, " Speed: %d days/second", speed * 20);
	for (int i = 0; i < characters; ++i) {
		glColor3f(1, 1, 1);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, printDay[i]);
	}

	glRasterPos2i(10, 90);
	sprintf_s(printDay, " h - help");
	for (int i = 0; i < characters; ++i) {
		glColor3f(1, 1, 1);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, printDay[i]);
	}

	if (show) {
		glRasterPos2i(400, 600);
		sprintf_s(printDay, " u - increase speed");
		for (int i = 0; i < characters; ++i) {
			glColor3f(1, 1, 1);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, printDay[i]);
		}
		glRasterPos2i(400, 560);
		sprintf_s(printDay, " j - decrease speed");
		for (int i = 0; i < characters; ++i) {
			glColor3f(1, 1, 1);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, printDay[i]);
		}
		glRasterPos2i(400, 520);
		sprintf_s(printDay, " left mouse - move camera");
		for (int i = 0; i < characters; ++i) {
			glColor3f(1, 1, 1);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, printDay[i]);
		}
		glRasterPos2i(400, 480);
		sprintf_s(printDay, " right mouse - zoom");
		for (int i = 0; i < characters; ++i) {
			glColor3f(1, 1, 1);
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, printDay[i]);
		}
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(viewer[0], viewer[1], viewer[2], viewer[3], viewer[4], viewer[5], 0.0, 1.0, 0.0);

	if (click && show) {
		time_t now = clock();
		if ((now - start) / CLOCKS_PER_SEC >= 3) {
			show = false;
		}
	}

	ukladSloneczny();
	printDay();

	glutSwapBuffers();
}


void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {//sprawdzenie czy przycisniety zostal prawy klawisz
		x_pos_old = x;
		y_pos_old = y;
		statusRight = 1;        //ustawienie flagi przycisku   
	}
	else {
		statusRight = 0;		//ustawienie flagi przycisku  
	}

	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {//sprawdzenie czy przycisniety zostal lewy klawisz
		x_pos_old = x;
		y_pos_old = y;
		statusLeft = 1;    //ustawienie flagi przycisku        
	}
	else {
		statusLeft = 0;		//ustawienie flagi przycisku  
	}

	if (btn == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {//sprawdzenie czy przycisniety zostal scroll
		x_pos_old = x;
		y_pos_old = y;
		statusMiddle = 1;    //ustawienie flagi przycisku    
	}
	else {
		statusMiddle = 0;		//ustawienie flagi przycisku  
	}
	if (!click) {    //wylaczenie pomocy
		click = true;
		start = clock();
	}
	
	RenderScene();
}

void keys(unsigned char key, int x, int y)
{
	if (key == 'r') {
		viewer[0] = 0;
		viewer[1] = 100;
		viewer[2] = 0;
		viewer[3] = 0;
		viewer[4] = 0;
		viewer[5] = 0;

	}
	if (key == 'u') {
		speed++;
	}
	if (key == 'j') {
		speed--;
	}
	if (!click) {
		click = true;
		start = clock();
	}



	RenderScene();
}

void Motion(GLsizei x, GLsizei y)
{
	delta_x = x - x_pos_old;//zmiana pozycji x
	x_pos_old = x;

	delta_y = y - y_pos_old;//zmiana pozycji y
	y_pos_old = y;

	glutPostRedisplay();
}


GLbyte* LoadTGAImage(const char* FileName, GLint* ImWidth, GLint* ImHeight, GLint* ImComponents, GLenum* ImFormat)
{
#pragma pack(1)           
	typedef struct
	{
		GLbyte    idlength;
		GLbyte    colormaptype;
		GLbyte    datatypecode;
		unsigned short    colormapstart;
		unsigned short    colormaplength;
		unsigned char     colormapdepth;
		unsigned short    x_orgin;
		unsigned short    y_orgin;
		unsigned short    width;
		unsigned short    height;
		GLbyte    bitsperpixel;
		GLbyte    descriptor;
	}TGAHEADER;
#pragma pack(8)

	FILE* pFile;
	TGAHEADER tgaHeader;
	unsigned long lImageSize;
	short sDepth;
	GLbyte* pbitsperpixel = NULL;


	/*************************************************************************************/

	// Wartoi domyne zwracane w przypadku b喚du

	*ImWidth = 0;
	*ImHeight = 0;
	*ImFormat = GL_BGR_EXT;
	*ImComponents = GL_RGB8;

#pragma warning(suppress : 4996)
	pFile = fopen(FileName, "rb");
	if (pFile == NULL)
		return NULL;

	/*************************************************************************************/
	// Przeczytanie nag堯wka pliku 


	fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);


	/*************************************************************************************/

	// Odczytanie szerokoi, wysokoi i g喚bi obrazu

	*ImWidth = tgaHeader.width;
	*ImHeight = tgaHeader.height;
	sDepth = tgaHeader.bitsperpixel / 8;


	/*************************************************************************************/
	// Sprawdzenie, czy g喚bia spe軟ia za這穎ne warunki (8, 24, lub 32 bity)

	if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
		return NULL;

	/*************************************************************************************/

	// Obliczenie rozmiaru bufora w pami璚i


	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;


	/*************************************************************************************/

	// Alokacja pami璚i dla danych obrazu


	pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

	if (pbitsperpixel == NULL)
		return NULL;

	if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
	{
		free(pbitsperpixel);
		return NULL;
	}


	/*************************************************************************************/

	// Ustawienie formatu OpenGL


	switch (sDepth)

	{

	case 3:

		*ImFormat = GL_BGR_EXT;

		*ImComponents = GL_RGB8;

		break;

	case 4:

		*ImFormat = GL_BGRA_EXT;

		*ImComponents = GL_RGBA8;

		break;

	case 1:

		*ImFormat = GL_LUMINANCE;

		*ImComponents = GL_LUMINANCE8;

		break;

	};



	fclose(pFile);



	return pbitsperpixel;

}

void texture(int textureID) {//plik to nazwa wczytywanego pliku

	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents[textureID], ImWidth[textureID], ImHeight[textureID], 0, ImFormat[textureID], GL_UNSIGNED_BYTE, textures[textureID]);//zdefiniowanie tekstury
	glEnable(GL_CULL_FACE);//uruchomienie teksturowania jednostronnego
	glCullFace(GL_FRONT);//uruchomienie teksturowania frontu
	glEnable(GL_TEXTURE_2D);//uruchomienie tekstur
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//tryb teksturowania
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//sposob nakladania tekstur
}

void loadTextures() {
	const char* plik[] = {
		"mercury.tga",
		"venus.tga",
		"earth.tga",
		"mars.tga",
		"jupiter.tga",
		"saturn.tga",
		"uranus.tga",
		"neptune.tga",
		"sun.tga",
		"orbits.tga"
	};
	GLbyte* pBytes;

	int i = 0;
	for (auto sciezka : plik) {
		pBytes = new GLbyte;
		pBytes = LoadTGAImage(sciezka, &ImWidth[i], &ImHeight[i], &ImComponents[i], &ImFormat[i]);//wczytanie tekstury
		textures[i] = pBytes;
		i++;
	}

}

void material() {
	float MatAmbient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float MatDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float MatSpecular[] = { 0.1f, 0.1f, 0.0f, 0.1f };
	float MatShininess = 20;
	float black[] = { 0.4f,0.4f,0.4f,1.0f };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MatDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, MatSpecular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, MatShininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
}

void light() {
	GLfloat att_constant = { 1.0 };
	GLfloat att_linear = { 0.05 };
	GLfloat att_quadratic = { 0.001 };
	float LightAmbient[] = { 0.1f, 0.1f, 0.05f, 1.0f };
	float LightEmission[] = { 1.0f, 1.0f, 0.8f, 1.0f };
	float LightDiffuse[] = { 1.0f, 1.0f, 0.8f, 1.0f };
	float LightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float LightPosition[] = { 0.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

void MyInit()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	material();
	light();
	loadTextures();
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;//usatwienie wielkosci do obracania obiektu
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, 1.6, 1.0, 100000.0);

	if (horizontal <= 1.6 * vertical)
		glViewport(0, (vertical - horizontal / 1.6) / 2, 1.6 * vertical, vertical);

	else
		glViewport((horizontal - 1.6 * vertical) / 2, 0, 1.6 * vertical, vertical);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void dayByDay() {
	day += speed;
	Sleep(5);
	RenderScene();

	if (day >= LLONG_MAX - 2) {
		day = LLONG_MIN + 2;
	}
	if (day <= LLONG_MIN + 2) {
		day = LLONG_MAX - 2;
	}
}


void main(void)
{
	srand(time(NULL));
	start = clock();

	for (int i = 0; i < 3600; i++) {
		float angle = 1.0 * i / 3600;
		ring[i] = sin(2 * M_PI * angle);
	}

	cout << "" << endl;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1600, 1000);
	glutCreateWindow("Układ Słoneczny");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	MyInit();
	glutMouseFunc(Mouse);//"lapanie" akcji na przyciskach myszy
	glutMotionFunc(Motion);//"lapanie" ruchu myszki
	glutKeyboardFunc(keys);//"lapanie" akcji na klawiaturze
	glEnable(GL_DEPTH_TEST);
	glutIdleFunc(dayByDay);
	glutMainLoop();
}
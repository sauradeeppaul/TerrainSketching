#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <gl/glut.h>
#include <cmath>

int baseArray[5000][2];
int heightArray[5000][2];
int b = 0;
int h = 0;
int LEFT_BUTTON_PRESSED = 0;
int minHeight = 10000;
int maxHeight = -10000;

static float BASE_PLANE = -30.0;

static int BASE_MODE = 0;
static int HEIGHT_MODE = 1;
static int GENERATE_PERLIN = 2;
int Mode;

float angle = 0.0;
float lx = 0.0f, ly = 1.0f, lz = -1.0f;
float x = 0.0f, y = 1.0f, z = 5.0f;

static int DISPLACEMENT = 400;
static int Z_DISP = 290;

void init(void)
{
	Mode = BASE_MODE;
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	std::cout << "Current Mode: Base Mode";


	for (int i = 0; i < 5000; i++)
	{
		baseArray[i][0] = 0;
		baseArray[i][1] = 0;
		heightArray[i][0] = 0;
		heightArray[i][1] = 0;
	}
	glPointSize(2.0);
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, (float)w / (float)h, 0.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

float getWeight(int half, int j) {
	float d = float(float(half - j) / float(half));
	return std::pow(std::pow(d, 2) - 1, 2);
}

void renderHeightMap() {
	/*int H = 2 * h + 1;
	int** arr = new int*[H];
	for (int i = 0; i < H; i++)
		arr[i] = new int[H];*/

	float rColor;
	int half = h / 2;
	for (int i = 0; i < h-1; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);

		float lastW = getWeight(half, 0);
		float nextW = 0;

		for (int j = 0; j < h - 1; j++) {
			//std::cout << "color:" << rColor << "\n";
			//glColor3f(1 - rColor, rColor, 0);
			//int bottomColor[3] = { 153, 119, 65 };
			int bottomColor[3] = { 0, 255, 65 };
			int topColor[3] = { 94, 68, 26 };
			rColor = (abs((float)half - (float)j) / (float)half) * (float)(heightArray[j][1] - minHeight) / (float)(maxHeight - minHeight);
			//glColor3f(1 - rColor, rColor, 0.0);

			glColor3ub(bottomColor[0] + int(rColor*(topColor[0] - bottomColor[0])), 
				bottomColor[1] + int(rColor*(topColor[1] - bottomColor[1])), 
				bottomColor[2] + int(rColor*(topColor[2] - bottomColor[2])));

			//glVertex3f(heightArray[i][0], BASE_PLANE + (abs((float)(half-j))/(float)half) * heightArray[i][1], -290 - heightArray[j][0]);
		 	//glVertex3f(heightArray[i + 1][0], BASE_PLANE + (abs((float)(half - j - 1)) / (float)half) * heightArray[i + 1][1], -290  - heightArray[j + 1][0]);

			glVertex3f(heightArray[i][0], BASE_PLANE + (lastW * heightArray[i][1]), - Z_DISP - heightArray[j][0]);
			nextW = getWeight(half, j + 1);
			glVertex3f(heightArray[i + 1][0], BASE_PLANE + (nextW * heightArray[i + 1][1]), - Z_DISP - heightArray[j + 1][0]);
			lastW = nextW;
		}
	}
	glEnd();

	/*for (int i = 0; i < H; i++)
		delete[] arr[i];
	delete[] arr;*/
}

void drawPoints()
{
	if (Mode == GENERATE_PERLIN) {
		renderHeightMap();
		return;
	}
	glBegin(GL_POINTS);
	if (Mode == BASE_MODE) {
		glColor3f(0.0, 0.0, 1.0);
		for (int i = 0; i < b; i++)
		{
			glVertex3f(baseArray[i][0], baseArray[i][1], -1 * Z_DISP);
		}
	}
	else {
		glColor3f(1.0, 0.0, 0.0);
		for (int i = 0; i < h; i++)
		{
			glVertex3f(heightArray[i][0], heightArray[i][1], -1 * Z_DISP);

		}

		glColor3f(0.0, 0.0, 1.0);
		for (int i = 0; i < b; i++)
		{
			glVertex3f(baseArray[i][0], BASE_PLANE, baseArray[i][1] - Z_DISP);
		}

	}
	glEnd();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// Set the camera
	gluLookAt(x, y + ly, z,
		x + lx, y + ly, z + lz,
		0.0f, 1.0f, 0.0f);

	//glLoadIdentity();
	glPushMatrix();

	drawPoints();

	glPopMatrix();

	glutSwapBuffers();
}


void myMouseMove(int x, int y)
{
	if (LEFT_BUTTON_PRESSED == 1) {
		glPushMatrix();
		if (Mode == BASE_MODE) {
			//std::cout << "x:" << x << " y:" << y;
			baseArray[b][0] = x - DISPLACEMENT;
			baseArray[b++][1] = DISPLACEMENT - y;
		}
		else if (Mode == HEIGHT_MODE) {
			//std::cout << "x:" << x << " y:" << y;
			heightArray[h][0] = x - DISPLACEMENT;
			heightArray[h++][1] = DISPLACEMENT - y;
			minHeight = std::min(DISPLACEMENT - y, minHeight);
			maxHeight = std::max(DISPLACEMENT - y, maxHeight);
			//std::cout << maxHeight << " and " << minHeight;
		}
		glPopMatrix();
	}
}

void handleMouseClicks(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			LEFT_BUTTON_PRESSED = 1;
		}
		else if (state == GLUT_UP) {
			LEFT_BUTTON_PRESSED = 0;
		}
	}

	/*if (button == GLUT_RIGHT_BUTTON and state == GLUT_UP) {
		if (Mode == BASE_MODE) {
			std::cout << "Current Mode: Height Mode";
			Mode = HEIGHT_MODE;
		} else if (Mode == HEIGHT_MODE) {
			std::cout << "Current Mode: Height Mode";
			Mode = GENERATE_PERLIN;
		}
	}*/
}

void generateLandformVector() {
	int minX = 10000;
	int maxX = -10000;
	int minZ = 10000;
	int maxZ = -10000;

	for (int i = 0; i < b; ++i) {
		minX = std::min(baseArray[i][0], minX);
		maxX = std::max(baseArray[i][0], maxX);
		minZ = std::min(baseArray[i][1], minZ);
		maxZ = std::max(baseArray[i][1], maxZ);
	}
}

void handleKeyClicks(unsigned char Key, int xx, int yy) {
	float fr = 1.0f;

	switch (Key)
	{
	case 'm':
		if (Mode == BASE_MODE) {
			std::cout << "Current Mode: Height Mode";
			Mode = HEIGHT_MODE;
			//generateLandformVector();
		}
		else if (Mode == HEIGHT_MODE) {
			std::cout << "Current Mode: Height Mode";
			Mode = GENERATE_PERLIN;
		}
		else if (Mode == GENERATE_PERLIN) {
			std::cout << "Current Mode: Perlin";
			Mode = HEIGHT_MODE;
		}

	case 'a':
		angle -= 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case 'd':
		angle += 0.01f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case 'w':
		x += lx * fr;
		z += lz * fr;
		break;
	case 's':
		x -= lx * fr;
		z -= lz * fr;
		break;
	case 'r':
		y += ly * fr;
		break;
	case 'f':
		y -= ly * fr;
		break;
	default:
		break;
	}
}

void handleCameraRotation(int key, int xx, int yy) {
	float fr = 5.0f;

	switch (key) {
	case GLUT_KEY_LEFT:
		angle -= 0.1f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_RIGHT:
		angle += 0.1f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case GLUT_KEY_UP:
		x += lx * fr;
		z += lz * fr;
		break;
	case GLUT_KEY_DOWN:
		x -= lx * fr;
		z -= lz * fr;
		break;
	case GLUT_KEY_PAGE_UP:
		y += ly * fr;
		break;
	case GLUT_KEY_PAGE_DOWN:
		y -= ly * fr;
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Sketch Terrain");

	init();

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutMotionFunc(myMouseMove);
	glutMouseFunc(handleMouseClicks);
	glutKeyboardFunc(handleKeyClicks);
	glutSpecialFunc(handleCameraRotation);
	glutMainLoop();
	return 0;
}